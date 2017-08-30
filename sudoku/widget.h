#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <vector>
#include "sudoku_grid.h"
#include "digit_button.h"
#include "tool_button.h"
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
	void set_backward_enable(bool);
	void set_forward_enable(bool);
	void game_start();

private:
	Ui::Widget *ui;
	QVBoxLayout *layout;
	QHBoxLayout *top_layout, *bottom_layout;
	QGridLayout *digit_button_layout;
	std::vector<DigitButton*> digit_btns;
	ToolButton *start_btn, *pause_btn, *hint_btn;
	ToolButton *clear_btn, *backward_btn, *forward_btn;
	QComboBox *level_combo;
	SudokuGrid *grid;
	Timer *timer;

	bool is_paused;
};

#endif // WIDGET_H
