#include <algorithm>
#include "sudoku_grid.h"
#include "config.h"

SudokuGrid::SudokuGrid(int cell_size, int fixed_size, QWidget *parent)
	: QLabel(parent),
	  current_level(1),
	  cell_size(cell_size),
	  cell_span(cell_size * cell_size),
	  fixed_size(fixed_size),
	  current_selected(nullptr),
	  sudoku(std::make_shared<Sudoku>(cell_size)),
	  actions(10)
{
	// init cells
	top_layer = new QGridLayout(this);

	cells.assign(cell_span * cell_span, nullptr);
	for(int r = 0; r != cell_size; ++r)
		for(int c = 0; c != cell_size; ++c)
		{
			QGridLayout *grid = new QGridLayout;
			top_layer->addLayout(grid, r, c);
			grid->setSpacing(CELL_SPACING);

			for(int x = 0; x != cell_size; ++x)
				for(int y = 0; y != cell_size; ++y)
				{
					int row = r * cell_size + x;
					int col = c * cell_size + y;
					SudokuCell *cell = new SudokuCell(row, col, sudoku);
					cell->setFocusPolicy(Qt::StrongFocus);
					cell->setFixedSize(fixed_size, fixed_size);
					// cell->setWordWrap(true);
					cell->setTextFormat(Qt::PlainText);
					cell->setAlignment(Qt::AlignCenter);

					grid->addWidget(cell, x, y);

					cells[row * cell_span + col] = cell;
				}

			grids.push_back(grid);
		}

	// connect signals
	for(int r = 0; r != cell_span; ++r)
		for(int c = 0; c != cell_span; ++c)
		{
			SudokuCell *cell = cells[r * cell_span + c];

			connect(cell, SIGNAL(selected_signal(SudokuCell*)),
					this, SLOT(cell_selected(SudokuCell*)));

			connect(cell, SIGNAL(selected_signal(SudokuCell*)),
					this, SLOT(light_value()));

			connect(cell, SIGNAL(value_changed(int,int,bool,IntList,bool,IntList)),
					this, SLOT(value_changed(int,int,bool,IntList,bool,IntList)));

			connect(cell, SIGNAL(value_changed(int,int,bool,IntList,bool,IntList)),
					this, SLOT(light_value()));

			connect(cell, SIGNAL(free_signal()),
					this, SLOT(free_selection()));

			for(int i = 0; i != cell_span; ++i)
			{
				connect(cell, SIGNAL(selected_signal(SudokuCell*)),
						cells[i * cell_span + c], SLOT(vertical_selected()));

				connect(cell, SIGNAL(selected_signal(SudokuCell*)),
						cells[r * cell_span + i], SLOT(horizontal_selected()));
			}
		}

	// set layer style
	top_layer->setSpacing(GRID_SPACING);
	top_layer->setMargin(GRID_SPACING);
	setStyleSheet(QString("background-color: ") + GRID_BG_COLOR + ";");

	int size = (fixed_size + 1) * cell_span + cell_size * GRID_SPACING;
	setFixedSize(size, size);
}

SudokuGrid::~SudokuGrid()
{
}

void SudokuGrid::cell_selected(SudokuCell *cell)
{
	if(cell != current_selected)
	{
		free_selection();
		current_selected = cell;
	}
}

void SudokuGrid::add_value(int v)
{
	if(current_selected)
		current_selected->add_value(v);
}

void SudokuGrid::set_value(int v)
{
	if(current_selected)
		current_selected->set_value(v);
}

void SudokuGrid::remove_value(int v)
{
	if(current_selected)
		current_selected->remove_value(v);
}

void SudokuGrid::game_start()
{
	if(current_selected)
		free_selection();
	current_selected = nullptr;

	*sudoku = Sudoku::generate(cell_size, current_level);

	for(int r = 0; r != cell_span; ++r)
		for(int c = 0; c != cell_span; ++c)
		{
			int id = r * cell_span + c;
			cells[id]->set_initial_status(sudoku->get(r, c));
		}

	actions.reset();
	emit set_forward_enable(false);
	emit set_backward_enable(false);
}

void SudokuGrid::game_solve()
{
	Sudoku hint_sudoku = sudoku->solve();
	if(hint_sudoku.is_empty())
	{
		QMessageBox::warning(this, "No Solution!", "Your current status leads to no solution.");
	} else {
		for(int r = 0; r != cell_span; ++r)
			for(int c = 0; c != cell_span; ++c)
				if(sudoku->get(r, c) == 0)
					cells[r * cell_span + c]->set_value(hint_sudoku.get(r, c));
}

void SudokuGrid::game_hint()
{
	Sudoku hint_sudoku = sudoku->solve();
	if(hint_sudoku.is_empty())
	{
		QMessageBox::warning(this, "No Solution!", "Your current status leads to no solution.");
	} else {
		IntList empty_cells;
		for(int r = 0; r != cell_span; ++r)
			for(int c = 0; c != cell_span; ++c)
				if(sudoku->get(r, c) == 0)
					empty_cells.push_back(r * cell_span + c);

		if(empty_cells.empty())
		{
			// already solved
			return;
		}

		int id = empty_cells[std::rand() % empty_cells.size()];
		int r = id / cell_span, c = id % cell_span;
		cells[id]->emit_selected_signal();
		cells[id]->set_value(hint_sudoku.get(r, c));
	}
}

void SudokuGrid::clear_grid()
{
	if(current_selected)
		current_selected->clear_values();
}

void SudokuGrid::value_changed(
	int r, int c,
	bool value_settled_old, IntList candidates_old,
	bool value_settled_new, IntList candidates_new)
{
	actions.add_action(r, c,
			value_settled_old, candidates_old,
			value_settled_new, candidates_new);

	emit set_backward_enable(actions.is_backwardable());
	emit set_forward_enable(actions.is_forwardable());
}

void SudokuGrid::backward_step()
{
	ActionInfo action = actions.backward();

	if(action.row < 0)
		return;

	int id = action.row * cell_span + action.col;
	cells[id]->recover_status(action.value_settled_old, action.candidates_old);
	cells[id]->emit_selected_signal();

	emit set_backward_enable(actions.is_backwardable());
	emit set_forward_enable(actions.is_forwardable());
}

void SudokuGrid::forward_step()
{
	ActionInfo action = actions.forward();

	if(action.row < 0)
		return;

	int id = action.row * cell_span + action.col;
	cells[id]->recover_status(action.value_settled_new, action.candidates_new);
	cells[id]->emit_selected_signal();

	emit set_backward_enable(actions.is_backwardable());
	emit set_forward_enable(actions.is_forwardable());
}

void SudokuGrid::light_value()
{
	if(current_selected)
	{
		int value = current_selected->get_value();
		for(SudokuCell* cell : cells)
			cell->light_value(value);
	}
}

void SudokuGrid::free_selection()
{
	if(current_selected)
	{
		int row = current_selected->get_row(),
			col = current_selected->get_col();
		for(int i = 0; i != cell_span; ++i)
		{
			cells[row * cell_span + i]->free_selection();
			cells[i * cell_span + col]->free_selection();
		}

		for(SudokuCell* cell : cells)
			cell->light_value(0);
	}

	current_selected = nullptr;
}

void SudokuGrid::level_changed(int index)
{
	current_level = index + SUDOKU_LEVEL_MIN;
	game_start();
}
