#ifndef SUDOKU_GRID_H
#define SUDOKU_GRID_H

#include <QtWidgets>
#include <vector>
#include <memory>
#include "sudoku.h"
#include "sudoku_cell.h"
#include "action_queue.h"

class SudokuGrid : public QLabel
{
	Q_OBJECT

public:
	explicit SudokuGrid(int cell_size, int fixed_size, QWidget *parent = 0);
	~SudokuGrid();

signals:

public slots:
	void cell_selected(SudokuCell*);
	void add_value(int);
	void remove_value(int);
	void clear_grid();

	void game_start();
	void game_hint();

	void backward_step();
	void forward_step();
	void value_changed(int r, int c, int v, IntList candidates = {});

	void light_value();
	void free_selection();

private:
	int cell_size, cell_span, fixed_size;
	QGridLayout *top_layer;
	SudokuCell *current_selected;
	std::vector<QGridLayout*> grids;
	std::vector<SudokuCell*> cells;

	std::shared_ptr<Sudoku> sudoku;
	ActionQueue actions;
};

#endif // SUDOKU_GRID_H
