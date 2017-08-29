#include <algorithm>
#include <cmath>
#include "sudoku_cell.h"

SudokuCell::SudokuCell(int row, int col, std::shared_ptr<Sudoku> sudoku, QWidget *parent)
	: QLabel(parent),
	  row(row), col(col),
	  sudoku(sudoku),
	  is_vertical_selected(false),
	  is_horizontal_selected(false),
	  value_fixed(false),
	  candidates(sudoku->span(), 0)
{
	setStyleSheet("background-color: #ccc;");
}

void SudokuCell::vertical_selected()
{
	is_vertical_selected = true;
	setStyleSheet("background-color: #999;");
}

void SudokuCell::horizontal_selected()
{
	is_horizontal_selected = true;
	setStyleSheet("background-color: #999;");
}

void SudokuCell::free_selection()
{
	if(is_vertical_selected && is_horizontal_selected)
		emit free_signal();
	is_vertical_selected = false;
	is_horizontal_selected = false;
	setStyleSheet("background-color: #ccc;");
}

void SudokuCell::add_value(int v)
{
	int span = sudoku->span();
	if(1 <= v && v <= span)
	{
		candidates[v] = 1;
		update_text();
	}
}

void SudokuCell::remove_value(int v)
{
	int span = sudoku->span();
	if(1 <= v && v <= span)
	{
		candidates[v] = 0;
		update_text();
	} else if(v == -1) {
		for(int i = span; i; --i)
			if(candidates[i])
			{
				candidates[i] = 0;
				update_text();
				break;
			}
	}
}

void SudokuCell::update_font()
{
	/* This function is modified from
	 * https://stackoverflow.com/questions/42652738/how-to-automatically-increase-decrease-text-size-in-label-in-qt
	 */

	// get initial settings
	QString text = this->text();
	if(text.length() == 0)
		return;

	QRect rect_lbl = geometry();
	QFont font = this->font();
	int size = font.pointSize();
	QFontMetrics fm(font);
	QRect rect = fm.boundingRect(rect_lbl, Qt::TextWordWrap, text);

	// decide whether to increase or decrease
	int step = rect.height() > rect_lbl.height() ? -1 : 1;

	// iterate until text fits best into rectangle of label
	for (;;)
	{
		font.setPointSize(size + step);
		QFontMetrics fm(font);
		rect = fm.boundingRect(rect_lbl, Qt::TextWordWrap, text);
		if (size <= 1)
		{
			// Font cannot be made smaller
			break;
		}
		if (step < 0)
		{
			size += step;
			if (rect.height() < rect_lbl.height()) break;
		} else {
			if (rect.height() > rect_lbl.height()) break;
			size += step;
		}
	}

	// apply result of iteration
	font.setPointSize(size);
	setFont(font);
}

void SudokuCell::update_text()
{
	int last_v = 0;
	int digit_num = count(candidates.begin(), candidates.end(), 1);
	int line_num = std::ceil(std::sqrt(1.0 * digit_num) - 1.0e-9);

	QString str;
	for(int i = 1, cnt = 0; i <= sudoku->span(); ++i)
	{
		if(candidates[i])
		{
			if(last_v) str += cnt % line_num ? ' ' : '\n';
			str += QString::number(i);
			last_v = i;
			++cnt;
		}
	}

	setText(str);
	update_font();

	if(digit_num == 0)
		sudoku->reset(row, col);
	else if(digit_num == 1)
		sudoku->set(row, col, last_v);
}

void SudokuCell::mousePressEvent(QMouseEvent *)
{
	emit selected_signal(this);
}

void SudokuCell::keyPressEvent(QKeyEvent *ev)
{
	int key = ev->key();
	if(Qt::Key_1 <= key && key <= Qt::Key_9)
		add_value(key - Qt::Key_0);
	else if(key == Qt::Key_Backspace)
		remove_value(-1);
}

void SudokuCell::focusInEvent(QFocusEvent *)
{
//	emit selected_signal(this);
}

void SudokuCell::focusOutEvent(QFocusEvent *)
{
}
