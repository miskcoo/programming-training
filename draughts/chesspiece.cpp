#include "chesspiece.h"
#include <QGraphicsOpacityEffect>

ChessPiece::ChessPiece(QWidget *parent) : QWidget(parent)
{
	opacity_effect = new QGraphicsOpacityEffect(this);
	opacity_effect->setOpacity(1.0);
	setGraphicsEffect(opacity_effect);
}

void ChessPiece::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	if(piece_info.type == DraughtsInfo::black)
	{
		p.setBrush(QBrush(Qt::red, piece_info.is_king ? Qt::Dense3Pattern : Qt::SolidPattern));
		p.drawEllipse(rect().adjusted(5, 5, -5, -5));
	} else if(piece_info.type == DraughtsInfo::white) {
		p.setBrush(QBrush(Qt::yellow, piece_info.is_king ? Qt::Dense3Pattern : Qt::SolidPattern));
		p.drawEllipse(rect().adjusted(5, 5, -5, -5));
	}

	QWidget::paintEvent(ev);
}

void ChessPiece::moveAnimation(vector<QRect> dest_rects, int milliseconds)
{
	QPropertyAnimation *move_ani = new QPropertyAnimation(this, "geometry");
	move_ani->setDuration(milliseconds);
	move_ani->setStartValue(geometry());
	for(int i = 0; i + 1 < (int)dest_rects.size(); ++i)
		move_ani->setKeyValueAt((i + 1.0) / dest_rects.size(), dest_rects[i]);
	move_ani->setEndValue(dest_rects.back());
	move_ani->setEasingCurve(QEasingCurve::BezierSpline);
	move_ani->start(QPropertyAnimation::DeleteWhenStopped);
}

void ChessPiece::fadeOut(qreal start, int milliseconds)
{
	QPropertyAnimation *opacity_ani = new QPropertyAnimation(opacity_effect, "opacity");
	opacity_ani->setDuration(milliseconds);
	opacity_ani->setStartValue(1);
	opacity_ani->setKeyValueAt(start, 1);
	opacity_ani->setEndValue(0);
	opacity_ani->setEasingCurve(QEasingCurve::OutBack);
	opacity_ani->start(QPropertyAnimation::DeleteWhenStopped);
}

void ChessPiece::setPieceInfo(DraughtsInfo piece_info)
{
	this->piece_info = piece_info;
	update();
}
