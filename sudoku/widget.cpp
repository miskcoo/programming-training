#include "widget.h"
#include "ui_widget.h"
#include "config.h"
#include "sudoku_grid.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    grid = new SudokuGrid(this);
}

Widget::~Widget()
{
    delete ui;
}
