#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <vector>
#include "sudoku_grid.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
	Q_OBJECT

public:
	explicit Widget(QWidget *parent = 0);
	~Widget();

private:
	Ui::Widget *ui;
	QVBoxLayout *layout;
	QHBoxLayout *button_layout;
	std::vector<QPushButton*> digit_btns;
	QPushButton *start_btn, *pause_btn;
	SudokuGrid *grid;
};

#endif // WIDGET_H
