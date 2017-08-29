#include "sudoku_cell.h"

SudokuCell::SudokuCell(QWidget *parent)
	: QLabel(parent)
{
	setStyleSheet("background-color: #ccc;");
}
