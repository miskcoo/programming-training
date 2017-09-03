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
	p.fillRect(rect(), Qt::black);
	QWidget::paintEvent(ev);
}

void ChessPiece::moveAnimation(int x, int y, int milliseconds)
{
	QPropertyAnimation *move_ani = new QPropertyAnimation(this, "geometry");
	move_ani->setDuration(milliseconds);
	move_ani->setStartValue(rect());
	QRect dest_rect = rect();
	dest_rect.moveBottomRight(QPoint(x, y));
	move_ani->setEndValue(dest_rect);
	move_ani->setEasingCurve(QEasingCurve::OutBack);
	move_ani->start(QPropertyAnimation::DeleteWhenStopped);
}

void ChessPiece::fadeOut(int milliseconds)
{
	QPropertyAnimation *opacity_ani = new QPropertyAnimation(opacity_effect, "opacity");
	opacity_ani->setDuration(milliseconds);
	opacity_ani->setStartValue(1);
	opacity_ani->setEndValue(0);
	opacity_ani->setEasingCurve(QEasingCurve::OutBack);
	opacity_ani->start(QPropertyAnimation::DeleteWhenStopped);
}
