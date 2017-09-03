#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <QtWidgets>
#include <QGraphicsEffect>

class ChessPiece : public QWidget
{
	Q_OBJECT
public:
	explicit ChessPiece(QWidget *parent = 0);

	void fadeOut(int milliseconds);
	void moveAnimation(int x, int y, int milliseconds);

signals:

public slots:

protected:
	void paintEvent(QPaintEvent *);

private:
	QGraphicsOpacityEffect *opacity_effect;
};

#endif // CHESSPIECE_H
