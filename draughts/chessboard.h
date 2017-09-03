#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QtWidgets>
#include "config.h"
#include "chesspiece.h"

class ChessBoard : public QWidget
{
	Q_OBJECT
public:
	explicit ChessBoard(QWidget *parent = 0);

	QRect getCellRect(int row, int col);
signals:

public slots:

protected:
	void paintEvent(QPaintEvent *);
private:
	int cell_status[10][10];
};

#endif // CHESSBOARD_H
