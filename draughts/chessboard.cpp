#include "chessboard.h"
#include <cstring>

#define CELL_MOVE_CANDIDATE   1
#define CELL_SELECT_CANDIDATE 2
#define CELL_MOVE_TRACE       4

ChessBoard::ChessBoard(QWidget *parent)
	: QWidget(parent)
{
	setFixedSize(501, 501);
	std::memset(cells, 0, sizeof(cells));
	std::memset(cell_status, 0, sizeof(cell_status));
}

void ChessBoard::initBoard()
{
	std::memset(cell_status, 0, sizeof(cell_status));

	draughts = std::make_shared<Draughts>();
	for(int i = 0; i != 10; ++i)
	{
		for(int j = 0; j != 10; ++j)
		{
			if(cells[i][j]) delete cells[i][j];
			if(!draughts->is_empty(i, j))
			{
				cells[i][j] = new ChessPiece(this);
				cells[i][j]->show();
				cells[i][j]->setGeometry(getCellRect(i, j));
				cells[i][j]->setPlayer(player);
			} else cells[i][j] = nullptr;
		}
	}

	updatePieces();
}

void ChessBoard::startGame(DraughtsInfo::Types player)
{
	this->player = player;
	this->cur_player = DraughtsInfo::black;

	initBoard();

	if(cur_player == player)
		markMoveCandidates(player);
}

void ChessBoard::updatePieces()
{
	for(int i = 0; i != 10; ++i)
		for(int j = 0; j != 10; ++j)
			if(!draughts->is_empty(i, j) && cells[i][j])
				cells[i][j]->setPieceInfo(draughts->get_info(i, j));
}

QRect ChessBoard::getCellRect(int row, int col)
{
	if(player == DraughtsInfo::black)
		row = 9 - row, col = 9 - col;
	int cell_width = (width() - 1) / 10;
	int cell_height = (height() - 1) / 10;
	return QRect(cell_width * col, cell_height * row, cell_width, cell_height);
}

pair<int, int> ChessBoard::mapMouseToCell(QPoint mouse)
{
	for(int i = 0; i != 10; ++i)
		for(int j = 0; j != 10; ++j)
			if(getCellRect(i, j).contains(mouse, true))
				return { i, j };
	return { -1, -1 };
}

void ChessBoard::markMoveCandidates(DraughtsInfo::Types player)
{
	auto avail_chess = draughts->get_avail_chess(player);
	for(auto piece_info : avail_chess)
		cell_status[piece_info.x][piece_info.y] |= CELL_MOVE_CANDIDATE;
	update();
}

void ChessBoard::markAvailTraces()
{
	if(avail_traces.empty()) return;
	for(const DraughtsTrace& trace : avail_traces)
	{
		DraughtsInfo piece_info;
		if(trace.size() == 2)
			piece_info = trace.back();
		else piece_info = trace[2];
		cell_status[piece_info.x][piece_info.y] |= CELL_SELECT_CANDIDATE;
	}

	int x = avail_traces[0][0].x, y = avail_traces[0][0].y;
	cell_status[x][y] |= CELL_SELECT_CANDIDATE;
	update();
}

void ChessBoard::clearMarks(int cleard_mask)
{
	for(int i = 0; i != 10; ++i)
		for(int j = 0; j != 10; ++j)
			cell_status[i][j] &= ~cleard_mask;
	update();
}

void ChessBoard::cellClicked(int x, int y)
{
	if(cur_player != player)
		return;

	if(cell_status[x][y] == CELL_MOVE_CANDIDATE)
	{
		clearMarks(CELL_SELECT_CANDIDATE | CELL_MOVE_TRACE);
		avail_traces = draughts->get_avail_move(x, y).second;
		long_term_move = false;
		markAvailTraces();
		cur_x = x, cur_y = y;
	} else if(cell_status[x][y] & CELL_SELECT_CANDIDATE) {
		if(draughts->is_empty(x, y))
		{
			int trace_len = avail_traces.front().size();
			clearMarks();
			if(trace_len <= 3 && !long_term_move)
			{
				// one-step move
				vector<pair<int, int>> move_trace{{ cur_x, cur_y }, { x, y }};
				if(moveChess(move_trace))
					emit playerMove(move_trace);
			} else {
				// multi-step move
				long_term_move = true;
				DraughtsTrace sub_trace;
				for(auto it = avail_traces.begin(); it != avail_traces.end(); ++it)
				{
					DraughtsTrace& trace = *it;
					if(trace[2].x == x && trace[2].y == y)
					{
						sub_trace.assign(trace.begin(), trace.begin() + 3);
						trace.erase(trace.begin(), trace.begin() + 2);
						cur_move_trace.push_back({x, y});
					} else {
						it = avail_traces.erase(it);
					}
				}

				applyTrace(sub_trace);
				if(trace_len <= 3)
				{
					// last move, apply to Draughts
					cur_move_trace.insert(cur_move_trace.begin(), { cur_x, cur_y });
					if(moveChess(cur_move_trace))
						emit playerMove(cur_move_trace);
					long_term_move = false;
					cur_move_trace.clear();
				} else markAvailTraces();
			}
		}
	}
}

void ChessBoard::applyTrace(const DraughtsTrace &trace)
{
	if(trace.empty()) return;
	clearMarks(CELL_MOVE_TRACE);

	int src_x = trace.front().x, src_y = trace.front().y;
	int dest_x = trace.back().x, dest_y = trace.back().y;
	if(trace.size() == 2)
	{
		// non-eating move
		cells[dest_x][dest_y] = cells[src_x][src_y];
		cells[src_x][src_y] = nullptr;

		cell_status[dest_x][dest_y] = CELL_MOVE_TRACE;
		cell_status[src_x][src_y] = CELL_MOVE_TRACE;

		cells[dest_x][dest_y]->moveAnimation( { getCellRect(dest_x, dest_y) }, 400);
	} else if(trace.size() > 2) {
		// eating move
		vector<QRect> move_seq;
		cell_status[src_x][src_y] = CELL_MOVE_TRACE;
		for(size_t i = 2; i < trace.size(); i += 2)
		{
			move_seq.push_back(getCellRect(trace[i].x, trace[i].y));
			cell_status[trace[i].x][trace[i].y] = CELL_MOVE_TRACE;
		}

		for(size_t i = 1; i < trace.size(); i += 2)
		{
			qreal start = 1.0 * (i / 2) / (trace.size() / 2);
			cells[trace[i].x][trace[i].y]->fadeOut(start, 400);
		}

		cells[dest_x][dest_y] = cells[src_x][src_y];
		cells[src_x][src_y] = nullptr;

		cells[dest_x][dest_y]->moveAnimation(move_seq, 400);
	}
}

bool ChessBoard::moveChess(const vector<pair<int, int>>& move_trace)
{
	if(move_trace.size() < 2) return false;
	auto trace = draughts->move(move_trace);
	if(trace.empty()) return false;

	int src_x = move_trace.front().first, src_y = move_trace.front().second;
	int dest_x = move_trace.back().first, dest_y = move_trace.back().second;
	qDebug() << "MOVE " << src_x << " " << src_y << " " << dest_x << " " << dest_y;

	if(!long_term_move) applyTrace(trace);

	if(cur_player == DraughtsInfo::black)
		cur_player = DraughtsInfo::white;
	else cur_player = DraughtsInfo::black;

	if(cur_player == player)
		markMoveCandidates(player);

	updatePieces();

	if(draughts->get_avail_chess(cur_player).empty())
		emit noAvailChess();

	return true;
}

void ChessBoard::mouseReleaseEvent(QMouseEvent *ev)
{
	if(ev->button() == Qt::LeftButton)
	{
		auto pos = mapMouseToCell(ev->pos());
		if(pos.first >= 0)
			cellClicked(pos.first, pos.second);
	}

	QWidget::mouseReleaseEvent(ev);
}

void ChessBoard::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	// draw basic background
	static QBrush dark_cell_brush(QPixmap(":dark-bg"));
	static QBrush light_cell_brush(QPixmap(":light-bg"));

	for(int i = 0; i != 10; ++i)
		for(int j = 0; j != 10; ++j)
		{
			if((i ^ j) & 1)
				p.setBrush(dark_cell_brush);
			else p.setBrush(light_cell_brush);
			p.drawRect(getCellRect(i, j));
		}

	// draw special cells
	auto drawRectWithGradient = [&](QRect rect, QColor c_begin, QColor c_end, double depth1, double depth2)
	{
		QLinearGradient gradient;
		gradient.setColorAt(0, c_begin);
		gradient.setColorAt(depth1, c_end);
		gradient.setColorAt(1, c_end);

		gradient.setStart(rect.topLeft());
		gradient.setFinalStop(rect.topRight());
		p.fillRect(rect, QBrush(gradient));

		gradient.setStart(rect.topLeft());
		gradient.setFinalStop(rect.bottomLeft());
		p.fillRect(rect, QBrush(gradient));

		gradient = QLinearGradient();
		gradient.setColorAt(0, c_begin);
		gradient.setColorAt(depth2, c_end);
		gradient.setColorAt(1, c_end);

		gradient.setStart(rect.topRight());
		gradient.setFinalStop(rect.topLeft());
		p.fillRect(rect, QBrush(gradient));

		gradient.setStart(rect.bottomLeft());
		gradient.setFinalStop(rect.topLeft());
		p.fillRect(rect, QBrush(gradient));
	};

	for(int i = 0; i != 10; ++i)
		for(int j = 0; j != 10; ++j)
		{
			QRect cell_rect = getCellRect(i, j);
			QRect gradient_cell_rect = cell_rect.adjusted(0, 0, 1, 1);
			p.setBrush(QBrush());

			if(cell_status[i][j] & CELL_MOVE_TRACE)
			{
				drawRectWithGradient(gradient_cell_rect,
									 QColor(0, 0, 0, 200),
									 QColor(0, 0, 0, 0), 0.15, 0.1);
			} else if(cell_status[i][j] & CELL_SELECT_CANDIDATE) {
				drawRectWithGradient(gradient_cell_rect,
									 QColor(255, 255, 255, 100),
									 QColor(255, 255, 255, 0), 0.1, 0.1);
				p.setPen(QPen(Qt::yellow, 2));
				p.drawRect(cell_rect);
			} else if(cell_status[i][j] & CELL_MOVE_CANDIDATE) {
				p.setPen(QPen(Qt::green, 2));
				p.drawRect(cell_rect);
			}
		}

	QWidget::paintEvent(ev);
}
