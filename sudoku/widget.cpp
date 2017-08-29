#include "widget.h"
#include "ui_widget.h"
#include "sudoku_grid.h"


Widget::Widget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Widget)
{
	ui->setupUi(this);

	layout = new QVBoxLayout(this);

	grid = new SudokuGrid;
	layout->addWidget(grid);
}

Widget::~Widget()
{
	delete ui;
}
