#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <QtWidgets>
#include <QGraphicsEffect>
#include "draughts.h"

class ChessPiece : public QWidget
{
	Q_OBJECT
public:
	explicit ChessPiece(QWidget *parent = 0);

	void fadeOut(qreal start, int milliseconds);
	void moveAnimation(vector<QRect> dest_rects, int milliseconds);
	void setPieceInfo(DraughtsInfo piece_info);

signals:

public slots:

protected:
	void paintEvent(QPaintEvent *);

private:
	DraughtsInfo piece_info;
	QGraphicsOpacityEffect *opacity_effect;
};

#endif // CHESSPIECE_H
