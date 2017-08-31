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

	connect(m_r, SIGNAL(mapped(int)), grid, SLOT(add_value(int)));
	connect(m_l, SIGNAL(mapped(int)), grid, SLOT(set_value(int)));

	QHBoxLayout *timer_layout = new QHBoxLayout;
	timer_layout->setContentsMargins(15, 5, 15, 5);
	timer = new Timer;
	ToolButton *clock_img = new ToolButton;
	clock_img->set_image(":/icons/icons/alarm-clock.png");
	clock_img->setFixedSize(18, 18);

	start_btn = new ToolButton;
	start_btn->set_image(":/icons/icons/restart.png");
	connect(start_btn, SIGNAL(clicked()), this, SLOT(game_start()));
	connect(start_btn, SIGNAL(clicked()), timer, SLOT(restart_timer()));

	pause_btn = new ToolButton;
	pause_btn->set_image(":/icons/icons/pause.png");
	connect(pause_btn, SIGNAL(clicked()), timer, SLOT(toggle_timer()));
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

	top_layout->addWidget(start_btn);
	top_layout->addWidget(pause_btn);
	top_layout->addWidget(hint_btn);
	top_layout->addWidget(clear_btn);
	top_layout->addWidget(backward_btn);
	top_layout->addWidget(forward_btn);

	timer_layout->addWidget(clock_img);
	timer_layout->addWidget(timer);
	top_layout->addLayout(timer_layout);

	top_layout->addWidget(level_combo);

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

	connect(m_level, SIGNAL(mapped(int)), grid, SLOT(level_changed(int)));
	connect(ui->actionNew_Game, SIGNAL(triggered()), grid, SLOT(game_start()));
	connect(ui->actionHint_one, SIGNAL(triggered()), grid, SLOT(game_hint()));
	connect(ui->actionHint_All, SIGNAL(triggered()), grid, SLOT(game_solve()));

	// run the game
	grid->game_start();
	timer->restart_timer();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::toggle_button()
{
	is_paused = !is_paused;
	if(is_paused)
		pause_btn->set_image(":/icons/icons/play.png");
	else pause_btn->set_image(":/icons/icons/pause.png");
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
	if(is_paused) toggle_button();
	grid->game_start();
}
