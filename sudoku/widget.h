#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <vector>
#include "sudoku_grid.h"
#include "timer.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
	Q_OBJECT

public:
	explicit Widget(QWidget *parent = 0);
	~Widget();

public slots:
	void toggle_button();

private:
	Ui::Widget *ui;
	QVBoxLayout *layout;
	QHBoxLayout *top_layout, *bottom_layout;
	QGridLayout *digit_button_layout;
	std::vector<QPushButton*> digit_btns;
	QPushButton *start_btn, *pause_btn, *hint_btn;
	QPushButton *clear_btn, *backward_btn, *forward_btn;
	SudokuGrid *grid;
	Timer *timer;
};

#endif // WIDGET_H
