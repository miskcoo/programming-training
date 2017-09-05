#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chessboard.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void initWidgets();

private:
	Ui::MainWindow *ui;
	QWidget *window;
	ChessBoard *chess_board;
};

#endif // MAINWINDOW_H
