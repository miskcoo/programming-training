#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "sudoku_grid.h"
#include "digit_button.h"
#include "tool_button.h"
#include "timer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QMainWindow *parent = 0);
	~MainWindow();

public slots:
	void toggle_button();
	void set_backward_enable(bool);
	void set_forward_enable(bool);
	void game_start();
	void level_changed(int);

private:
	Ui::MainWindow *ui;
	QVBoxLayout *layout;
	QHBoxLayout *top_layout, *bottom_layout;
	QGridLayout *digit_button_layout;
	std::vector<DigitButton*> digit_btns;
	ToolButton *start_btn, *pause_btn, *hint_btn;
	ToolButton *clear_btn, *backward_btn, *forward_btn;
	QLabel *level_label;
	SudokuGrid *grid;
	Timer *timer;

	bool is_paused;
};

#endif // MAINWINDOW_H
