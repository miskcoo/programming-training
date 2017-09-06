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

	void initBoard(Draughts *game);
	QRect getCellRect(int row, int col);
	pair<int, int> mapMouseToCell(QPoint);

	void clearMarks(int cleard_mask = ~0u >> 1);
	void markMoveCandidates(DraughtsInfo::Types player);
	void markAvailTraces();
	void cellClicked(int x, int y);
	bool moveChess(const vector<pair<int, int>>&);
	void applyTrace(const DraughtsTrace& trace);
	void updatePieces();

	void startGame(DraughtsInfo::Types player,
				   DraughtsInfo::Types first_player = DraughtsInfo::black,
				   Draughts *game = nullptr);

	DraughtsInfo::Types getPlayer() { return player; }
	DraughtsInfo::Types getCurrentPlayer() { return cur_player; }
signals:
	void playerMove(vector<pair<int, int>>);
	void noAvailChess();

public slots:

protected:
	void paintEvent(QPaintEvent *);
	void mouseReleaseEvent(QMouseEvent *);
private:
	bool long_term_move;
	int cur_x, cur_y;
	int cell_status[10][10];
	ChessPiece *cells[10][10];
	vector<DraughtsTrace> avail_traces;
	vector<pair<int, int>> cur_move_trace;
	DraughtsInfo::Types player, cur_player;
	std::shared_ptr<Draughts> draughts;
};

#endif // CHESSBOARD_H
