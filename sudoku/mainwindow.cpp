#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sudoku_grid.h"
#include "config.h"

MainWindow::MainWindow(QMainWindow *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	is_paused(false)
{
	ui->setupUi(this);

	int fixed_size = FIXED_SIZE;
	int cell_size = CELL_SIZE;
	int cell_span = cell_size * cell_size;

	QWidget *window = new QWidget;
	setCentralWidget(window);

	layout = new QVBoxLayout(window);
	top_layout = new QHBoxLayout;
	bottom_layout = new QHBoxLayout;

	layout->addLayout(top_layout);
	layout->addLayout(bottom_layout);

	grid = new SudokuGrid(cell_size, fixed_size);
	bottom_layout->addWidget(grid);

	// init digit button layout
	QLabel *digit_layout_wrap = new QLabel;
	int size = (fixed_size + 1) * cell_span + 2 * (GRID_SPACING - 1) + 1;
	digit_layout_wrap->setFixedSize(fixed_size + GRID_SPACING * 2, size);
	digit_layout_wrap->setStyleSheet(
		"QLabel { background-color: " GRID_BG_COLOR ";}"
		"QPushButton{ "
				"border:none;"
				"background-color: " DEFAULT_BG_COLOR "; }"
		"QPushButton:pressed {"
				"border:none;"
				"background-color: " PRESSED_BG_COLOR "; }"
		"QPushButton:hover:!pressed {"
				"border:none;"
				"background-color: " HOVER_BG_COLOR "; }"
	);

	digit_button_layout = new QGridLayout(digit_layout_wrap);
	digit_button_layout->setMargin(GRID_SPACING);
	digit_button_layout->setSpacing(CELL_SPACING);

	bottom_layout->setSpacing(10);
	bottom_layout->addWidget(digit_layout_wrap);

	QSignalMapper *m_r = new QSignalMapper(this);
	QSignalMapper *m_l = new QSignalMapper(this);
	for(int i = 1; i <= cell_span; ++i)
	{
		DigitButton *btn = new DigitButton;

		btn->setText(QString::number(i));
		btn->setFixedSize(fixed_size, fixed_size);
		btn->update_font();

		m_l->setMapping(btn, i);
		m_r->setMapping(btn, i);
		connect(btn, SIGNAL(clicked()), m_l, SLOT(map()));
		connect(btn, SIGNAL(right_clicked()), m_r, SLOT(map()));

		digit_button_layout->addWidget(btn, i - 1, 0);
		digit_btns.push_back(btn);
	}

	connect(m_r, SIGNAL(mapped(int)), this, SLOT(digit_add_value(int)));
	connect(m_l, SIGNAL(mapped(int)), this, SLOT(digit_set_value(int)));

	QHBoxLayout *timer_layout = new QHBoxLayout;
	timer_layout->setContentsMargins(15, 5, 15, 5);
	timer = new Timer;
	ToolButton *clock_img = new ToolButton;
	clock_img->set_image(":/icons/icons/alarm-clock.png");
	clock_img->setFixedSize(18, 18);

	start_btn = new ToolButton;
	start_btn->set_image(":/icons/icons/restart.png");
	connect(start_btn, SIGNAL(clicked()), this, SLOT(game_start()));

	pause_btn = new ToolButton;
	pause_btn->set_image(":/icons/icons/pause.png");
	connect(pause_btn, SIGNAL(clicked()), this, SLOT(toggle_button()));

	hint_btn = new ToolButton;
	hint_btn->set_image(":/icons/icons/information.png");
	connect(hint_btn, SIGNAL(clicked()), grid, SLOT(game_hint()));

	clear_btn = new ToolButton;
	clear_btn->set_image(":/icons/icons/eraser.png");
	connect(clear_btn, SIGNAL(clicked()), grid, SLOT(clear_grid()));

	backward_btn = new ToolButton;
	backward_btn->set_image(":/icons/icons/back.png");
	connect(backward_btn, SIGNAL(clicked()), grid, SLOT(backward_step()));
	connect(grid, SIGNAL(set_backward_enable(bool)), this, SLOT(set_backward_enable(bool)));

	forward_btn = new ToolButton;
	forward_btn->set_image(":/icons/icons/next.png");
	connect(forward_btn, SIGNAL(clicked()), grid, SLOT(forward_step()));
	connect(grid, SIGNAL(set_forward_enable(bool)), this, SLOT(set_forward_enable(bool)));

	level_label = new QLabel;
	level_label->setStyleSheet("font-size: 18pt;");
	level_label->setContentsMargins(0, 0, 10, 0);

	// set top layout
	top_layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum));
	top_layout->addWidget(level_label);

	timer_layout->addWidget(clock_img);
	timer_layout->addWidget(timer);
	top_layout->addLayout(timer_layout);

	top_layout->addWidget(start_btn);
	top_layout->addWidget(pause_btn);
	top_layout->addWidget(hint_btn);
	top_layout->addWidget(clear_btn);
	top_layout->addWidget(backward_btn);
	top_layout->addWidget(forward_btn);

	top_layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum));

	// add menu actions
	QSignalMapper *m_level = new QSignalMapper(this);
	QActionGroup *level_group = new QActionGroup(this);
	for(int i = SUDOKU_LEVEL_MIN; i <= SUDOKU_LEVEL_MAX; ++i)
	{
		QAction *level_action = new QAction("Level " + QString::number(i));
		level_action->setCheckable(true);
		if(i == SUDOKU_LEVEL_MIN)
			level_action->setChecked(true);

		m_level->setMapping(level_action, i);
		connect(level_action, SIGNAL(triggered()), m_level, SLOT(map()));

		level_group->addAction(level_action);
		ui->level_menu->addAction(level_action);
	}

	QAction *level_empty = new QAction("Level Empty");
	m_level->setMapping(level_empty, 0);
	connect(level_empty, SIGNAL(triggered()), m_level, SLOT(map()));
	level_group->addAction(level_empty);
	ui->level_menu->addAction(level_empty);

	connect(m_level, SIGNAL(mapped(int)), this, SLOT(level_changed(int)));
	connect(ui->actionNew_Game, SIGNAL(triggered()), this, SLOT(game_start()));
	connect(ui->actionHint_one, SIGNAL(triggered()), grid, SLOT(game_hint()));
	connect(ui->actionHint_All, SIGNAL(triggered()), grid, SLOT(game_solve()));

	connect(grid, SIGNAL(update_digit_signal(IntList)),
			this, SLOT(update_digit(IntList)));
	connect(grid, SIGNAL(game_over_signal()),
			this, SLOT(game_over()));

	// run the game
	level_changed(1);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::toggle_button()
{
	timer->toggle_timer();
	is_paused = !is_paused;
	grid->setEnabled(!is_paused);
	for(DigitButton* btn : digit_btns)
		btn->setEnabled(!is_paused);
	if(is_paused)
	{
		pause_btn->set_image(":/icons/icons/play.png");
	} else {
		pause_btn->set_image(":/icons/icons/pause.png");
	}
}

void MainWindow::set_backward_enable(bool enabled)
{
	backward_btn->setEnabled(enabled);
}

void MainWindow::set_forward_enable(bool enabled)
{
	forward_btn->setEnabled(enabled);
}

void MainWindow::game_start()
{
	timer->restart_timer();
	if(is_paused) toggle_button();
	grid->game_start();
}

void MainWindow::level_changed(int level)
{
	grid->level_changed(level);
	level_label->setText("Level " + QString::number(level));
	game_start();
}

void MainWindow::update_digit(IntList candidates)
{
	for(int i = 1; i != (int)candidates.size(); ++i)
		digit_btns[i - 1]->set_checked(candidates[i]);
}

void MainWindow::digit_add_value(int pos)
{
	SudokuCell *cell = grid->get_current_selected();
	if(digit_btns[pos - 1]->is_checked() && cell)
	{
		if(cell->is_value_settled())
			grid->add_value(pos);
		else grid->remove_value(pos);
	} else grid->add_value(pos);
}

void MainWindow::digit_set_value(int pos)
{
	SudokuCell *cell = grid->get_current_selected();
	if(digit_btns[pos - 1]->is_checked() && cell)
	{
		if(!cell->is_value_settled())
			grid->set_value(pos);
		else grid->remove_value(pos);
	} else grid->set_value(pos);
}

void MainWindow::game_over()
{
	if(!is_paused) toggle_button();
	grid->free_selection();
	for(DigitButton *digit_btn : digit_btns)
		digit_btn->set_checked(false);
}
