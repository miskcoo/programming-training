#ifndef SUDOKU_GRID_H
#define SUDOKU_GRID_H

#include <QtWidgets>
#include <vector>
#include <memory>
#include "sudoku.h"
#include "sudoku_cell.h"

class SudokuGrid : public QLabel
{
	Q_OBJECT

public:
	explicit SudokuGrid(int cell_size = 3, QWidget *parent = 0);
	~SudokuGrid();

signals:

public slots:
	void cell_selected(SudokuCell*);
	void add_value(int);
	void remove_value(int);

	void game_start();
	void game_hint();

private:
	int cell_size, cell_span, fixed_size;
	QGridLayout *top_layer;
	SudokuCell *current_selected;
	std::vector<QGridLayout*> grids;
	std::vector<SudokuCell*> cells;

	std::shared_ptr<Sudoku> sudoku;
};

#endif // SUDOKU_GRID_H
