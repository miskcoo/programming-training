#include "sudoku_grid.h"

SudokuGrid::SudokuGrid(QWidget *parent)
	: QLabel(parent),
	  cell_size(3),
	  cell_span(cell_size * cell_size)
{
	top_layer = new QGridLayout(this);
	cells.assign(cell_span * cell_span, nullptr);
	for(int r = 0; r != cell_size; ++r)
		for(int c = 0; c != cell_size; ++c)
		{
			QGridLayout *grid = new QGridLayout;
			top_layer->addLayout(grid, r, c);
			grid->setSpacing(1);

			for(int x = 0; x != cell_size; ++x)
				for(int y = 0; y != cell_size; ++y)
				{
					SudokuCell *cell = new SudokuCell;
					grid->addWidget(cell, x, y);

					int id = (r * cell_size + x) * cell_span + (c * cell_size + y);
					cells[id] = cell;
				}

			grids.push_back(grid);
		}

	top_layer->setSpacing(2);
	top_layer->setMargin(2);
	setStyleSheet("background-color: #666;");
}

SudokuGrid::~SudokuGrid()
{
}
