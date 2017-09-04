#include "chessboard.h"

#define CELL_MOVE_CANDIDATE   1
#define CELL_SELECT_CANDIDATE 2
#define CELL_MOVE_TRACE       3

ChessBoard::ChessBoard(QWidget *parent)
	: QWidget(parent)
{
	setFixedSize(501, 501);

	for(int i = 0; i != 10; ++i)
		for(int j = 0; j != 10; ++j)
			cell_status[i][j] = 0;
}

QRect ChessBoard::getCellRect(int row, int col)
{
	int cell_width = (width() - 1) / 10;
	int cell_height = (height() - 1) / 10;
	return QRect(cell_width * col, cell_height * row, cell_width, cell_height);
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
			switch(cell_status[i][j])
			{
			case CELL_MOVE_TRACE:
				drawRectWithGradient(gradient_cell_rect,
									 QColor(0, 0, 0, 200),
									 QColor(0, 0, 0, 0), 0.1);
				break;
			case CELL_MOVE_CANDIDATE:
				p.setPen(QPen(Qt::green, 2));
				p.drawRect(cell_rect);
				break;
			case CELL_SELECT_CANDIDATE:
				drawRectWithGradient(gradient_cell_rect,
									 QColor(255, 255, 255, 100),
									 QColor(255, 255, 255, 0), 0.1);
				p.setPen(QPen(Qt::yellow, 2));
				p.drawRect(cell_rect);
				break;
			}
		}

	QWidget::paintEvent(ev);
}
