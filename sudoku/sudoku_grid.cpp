#include "sudoku_grid.h"

SudokuGrid::SudokuGrid(QWidget *parent)
    : QGridLayout(parent),
      cell_size(3),
      cell_span(cell_size * cell_size)
{
    cells.assign(cell_span * cell_span, nullptr);
    for(int r = 0; r != cell_size; ++r)
        for(int c = 0; c != cell_size; ++c)
        {
            QGridLayout *grid = new QGridLayout;
            addLayout(grid, r, c);

            for(int x = 0; x != cell_size; ++x)
                for(int y = 0; y != cell_size; ++y)
                {
                    SudokuCell *cell = new SudokuCell;
                    grid->addWidget(cell, x, y);

                    int id = (r * cell_size + x) * cell_size + (c * cell_size + y);
                    cells[id] = cell;
                }

            grids.push_back(grid);
        }

    setSpacing(1);
}

SudokuGrid::~SudokuGrid()
{
}
