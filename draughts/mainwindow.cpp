#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	initWidgets();
}

void MainWindow::initWidgets()
{
	window = new QWidget;
	setCentralWidget(window);

	QVBoxLayout *layout = new QVBoxLayout(window);

	chess_board = new ChessBoard;

	layout->addWidget(chess_board);
}

MainWindow::~MainWindow()
{
	delete ui;
}
