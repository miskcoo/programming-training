#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QtWidgets>
#include <memory>
#include "config.h"
#include "draughts.h"
#include "chesspiece.h"

class ChessBoard : public QWidget
{
	Q_OBJECT
public:
	explicit ChessBoard(QWidget *parent = 0);

	void initBoard();
	QRect getCellRect(int row, int col);
	pair<int, int> mapMouseToCell(QPoint);

	void clearMarks(int cleard_mask = ~0u >> 1);
	void markMoveCandidates(DraughtsInfo::Types player);
	void markSelectedCandidates(int x, int y);
	void cellClicked(int x, int y);
	bool moveChess(int src_x, int src_y, int dest_x, int dest_y);
	void updatePieces();

	void startGame(DraughtsInfo::Types player);
	DraughtsInfo::Types getPlayer() { return player; }
	DraughtsInfo::Types getCurrentPlayer() { return cur_player; }
signals:
	void playerMove(int, int, int, int);
	void noAvailChess();

public slots:

protected:
	void paintEvent(QPaintEvent *);
	void mouseReleaseEvent(QMouseEvent *);
private:
	int cur_x, cur_y;
	int cell_status[10][10];
	ChessPiece *cells[10][10];
	DraughtsInfo::Types player, cur_player;
	std::shared_ptr<Draughts> draughts;
};

#endif // CHESSBOARD_H
