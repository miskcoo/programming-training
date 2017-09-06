#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <QtWidgets>
#include <QGraphicsEffect>
#include <QSound>
#include "draughts.h"

class ChessPiece : public QWidget
{
	Q_OBJECT
public:
	explicit ChessPiece(QWidget *parent = 0);

	void setPlayer(DraughtsInfo::Types player);
	void fadeOut(qreal start, qreal end, int milliseconds);
	void moveAnimation(vector<QRect> dest_rects, int milliseconds);
	void setPieceInfo(DraughtsInfo piece_info);

signals:

public slots:
	void moveFinished();

protected:
	void paintEvent(QPaintEvent *);

private:
	QSound *sound;
	DraughtsInfo piece_info;
	DraughtsInfo::Types player;
	QGraphicsOpacityEffect *opacity_effect;
};

#endif // CHESSPIECE_H
