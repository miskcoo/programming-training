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

	SudokuCell *get_current_selected() const;

signals:
	void set_forward_enable(bool);
	void set_backward_enable(bool);
	void update_digit_signal(IntList);
	void game_over_signal();

public slots:
	void cell_selected(SudokuCell*);
	void add_value(int);
	void set_value(int);
	void remove_value(int);
	void clear_grid();

	void game_start();
	void game_reset();
	void game_hint();
	void game_solve();

	void move_focus(int);
	void backward_step();
	void forward_step();
	void value_changed(int, int, bool, IntList, bool, IntList);
	void level_changed(int);

	void light_value();
	void free_selection();

private:
	int current_level;
	int cell_size, cell_span, fixed_size;
	QGridLayout *top_layer;
	SudokuCell *current_selected;
	std::vector<QGridLayout*> grids;
	std::vector<SudokuCell*> cells;

	Sudoku initial_sudoku;
	std::shared_ptr<Sudoku> sudoku;
	ActionQueue actions;
};

#endif // SUDOKU_GRID_H
