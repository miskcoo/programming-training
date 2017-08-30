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

		digit_btns.push_back(btn);
	}

	connect(m, SIGNAL(mapped(int)), grid, SLOT(add_value(int)));

	timer = new Timer;

	start_btn = new QPushButton("Start");
	connect(start_btn, SIGNAL(clicked()), grid, SLOT(game_start()));
	connect(start_btn, SIGNAL(clicked()), timer, SLOT(restart_timer()));

	pause_btn = new QPushButton("Pause");
	connect(pause_btn, SIGNAL(clicked()), timer, SLOT(toggle_timer()));
	connect(pause_btn, SIGNAL(clicked()), this, SLOT(toggle_button()));

	hint_btn = new QPushButton("Hint");
	connect(hint_btn, SIGNAL(clicked()), grid, SLOT(game_hint()));

	clear_btn = new QPushButton("Clear");
	connect(clear_btn, SIGNAL(clicked()), grid, SLOT(clear_grid()));

	backward_btn = new QPushButton("Backward");
	connect(backward_btn, SIGNAL(clicked()), grid, SLOT(backward_step()));

	forward_btn = new QPushButton("Forward");
	connect(forward_btn, SIGNAL(clicked()), grid, SLOT(forward_step()));

	button_layout->addWidget(start_btn);
	button_layout->addWidget(pause_btn);
	button_layout->addWidget(hint_btn);
	button_layout->addWidget(clear_btn);
	button_layout->addWidget(backward_btn);
	button_layout->addWidget(forward_btn);
	button_layout->addWidget(timer);
}

Widget::~Widget()
{
	delete ui;
}

void Widget::toggle_button()
{
}
