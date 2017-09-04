#include "chessboard.h"
#include <cstring>

#define CELL_MOVE_CANDIDATE   1
#define CELL_SELECT_CANDIDATE 2
#define CELL_MOVE_TRACE       4

ChessBoard::ChessBoard(QWidget *parent)
	: QWidget(parent)
{
	setFixedSize(501, 501);
}

void ChessBoard::initBoard()
{
	std::memset(cell_status, 0, sizeof(cell_status));

	draughts = std::make_shared<Draughts>();
	for(int i = 0; i != 10; ++i)
	{
		for(int j = 0; j != 10; ++j)
		{
			if(!draughts->is_empty(i, j))
			{
				cells[i][j] = std::make_shared<ChessPiece>(this);
				cells[i][j]->setGeometry(getCellRect(i, j));
				cells[i][j]->setPieceInfo(draughts->get_info(i, j));
			} else cells[i][j] = nullptr;
		}
	}
}

QRect ChessBoard::getCellRect(int row, int col)
{
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
	for(auto piece_info : draughts->get_avail_chess(player))
		cell_status[piece_info.x][piece_info.y] |= CELL_MOVE_CANDIDATE;
	update();
}

void ChessBoard::markSelectedCandidates(int x, int y)
{
	for(auto piece_info : draughts->get_avail_move(x, y).second)
		cell_status[piece_info.x][piece_info.y] |= CELL_SELECT_CANDIDATE;
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
	/* only for TEST */
	static DraughtsInfo::Types types = DraughtsInfo::white;
	/* only for TEST */

	if(cell_status[x][y] == CELL_MOVE_CANDIDATE)
	{
		clearMarks(CELL_SELECT_CANDIDATE);
		markSelectedCandidates(x, y);
		cur_x = x, cur_y = y;
	} else if(cell_status[x][y] & CELL_SELECT_CANDIDATE) {
		clearMarks();
		moveChess(cur_x, cur_y, x, y);
	}

	/* only for TEST */
	else
	{
		if(types == DraughtsInfo::white)
			types = DraughtsInfo::black;
		else types = DraughtsInfo::white;
		markMoveCandidates(types);
	}
	/* only for TEST */
}

void ChessBoard::moveChess(int src_x, int src_y, int dest_x, int dest_y)
{
	auto trace = draughts->move(src_x, src_y, dest_x, dest_y);
	if(trace.empty()) return;

	if(trace.size() == 2)
	{
		// non-eating move
		cells[dest_x][dest_y] = cells[src_x][src_y];
		cells[src_x][src_y] = nullptr;

		cells[dest_x][dest_y]->moveAnimation( { getCellRect(dest_x, dest_y) }, 400);
	} else if(trace.size() > 2) {
		// eating move
		vector<QRect> move_seq;
		for(size_t i = 2; i < trace.size(); i += 2)
			move_seq.push_back(getCellRect(trace[i].x, trace[i].y));

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
	QBrush dark_cell_brush(qRgb(160, 160, 160)),
		   light_cell_brush(qRgb(200, 200, 200));

	for(int i = 0; i != 10; ++i)
		for(int j = 0; j != 10; ++j)
		{
			if((i ^ j) & 1)
				p.setBrush(dark_cell_brush);
			else p.setBrush(light_cell_brush);
			p.drawRect(getCellRect(i, j));
		}

	// draw special cells
	auto drawRectWithGradient = [&](QRect rect, QColor c_begin, QColor c_end, double depth)
	{
		QLinearGradient gradient;
		gradient.setColorAt(0, c_begin);
		gradient.setColorAt(depth, c_end);
		gradient.setColorAt(1, c_end);

		gradient.setStart(rect.topLeft());
		gradient.setFinalStop(rect.topRight());
		p.fillRect(rect, QBrush(gradient));

		gradient.setStart(rect.topRight());
		gradient.setFinalStop(rect.topLeft());
		p.fillRect(rect, QBrush(gradient));

		gradient.setStart(rect.topLeft());
		gradient.setFinalStop(rect.bottomLeft());
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
									 QColor(0, 0, 0, 0), 0.1);
			} else if(cell_status[i][j] & CELL_SELECT_CANDIDATE) {
				drawRectWithGradient(gradient_cell_rect,
									 QColor(255, 255, 255, 100),
									 QColor(255, 255, 255, 0), 0.1);
				p.setPen(QPen(Qt::yellow, 2));
				p.drawRect(cell_rect);
			} else if(cell_status[i][j] & CELL_MOVE_CANDIDATE) {
				p.setPen(QPen(Qt::green, 2));
				p.drawRect(cell_rect);
			}
		}

	QWidget::paintEvent(ev);
}
