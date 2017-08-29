#include "widget.h"
#include "ui_widget.h"
#include "sudoku_grid.h"


Widget::Widget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Widget)
{
	ui->setupUi(this);

	layout = new QVBoxLayout(this);

	int cell_size = 3;
	int cell_span = cell_size * cell_size;

	grid = new SudokuGrid(cell_size);
	layout->addWidget(grid);

	button_layout = new QHBoxLayout;
	layout->addLayout(button_layout);

	QSignalMapper *m = new QSignalMapper(this);
	for(int i = 1; i <= cell_span; ++i)
	{
		QPushButton *btn = new QPushButton(QString::number(i));
		button_layout->addWidget(btn);

		m->setMapping(btn, i);
		connect(btn, SIGNAL(clicked()), m, SLOT(map()));
		connect(m, SIGNAL(mapped(int)), grid, SLOT(add_value(int)));

		digit_btns.push_back(btn);
	}

	start_btn = new QPushButton("Start");
	connect(start_btn, SIGNAL(clicked()), grid, SLOT(game_start()));

	pause_btn = new QPushButton("Pause");

	button_layout->addWidget(start_btn);
	button_layout->addWidget(pause_btn);
}

Widget::~Widget()
{
	delete ui;
}
