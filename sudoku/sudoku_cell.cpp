#include <algorithm>
#include <cmath>
#include "sudoku_cell.h"
#include "utils.h"
#include "config.h"

SudokuCell::SudokuCell(int row, int col, std::shared_ptr<Sudoku> sudoku, QWidget *parent)
	: QLabel(parent),
	  row(row), col(col),
	  initial_status(0),
	  sudoku(sudoku),
	  is_lighted(false),
	  is_marked(false),
	  is_vertical_selected(false),
	  is_horizontal_selected(false),
	  value_fixed(false),
	  value_settled(false),
	  candidates(sudoku->span() + 1, 0)
{
	update_style();
}

void SudokuCell::vertical_selected()
{
	is_vertical_selected = true;
	update_style();
}

void SudokuCell::horizontal_selected()
{
	is_horizontal_selected = true;
	update_style();
}

void SudokuCell::free_selection()
{
	is_vertical_selected = false;
	is_horizontal_selected = false;
	update_style();
}

void SudokuCell::add_value(int v, bool emit_signal)
{
	int span = sudoku->span();
	if(1 <= v && v <= span && !(!value_settled && candidates[v]))
	{
		IntList old_candidates = candidates;
		bool old_value_settled = value_settled;
		value_settled = false;
		candidates[v] = 1;
		is_lighted = false;
		update_style();
		update_text();
		if(emit_signal)
			emit value_changed(row, col,
					old_value_settled, old_candidates,
					value_settled, candidates);
	}
}

void SudokuCell::set_value(int v, bool emit_signal)
{
	int span = sudoku->span();
	if(1 <= v && v <= span && !(value_settled && candidates[v]))
	{
		bool old_value_settled = value_settled;
		value_settled = true;
		IntList old_candidates = candidates;
		std::fill(candidates.begin(), candidates.end(), 0);
		candidates[v] = 1;
		update_text();
		if(emit_signal)
			emit value_changed(row, col,
					old_value_settled, old_candidates,
					value_settled, candidates);
	}
}

void SudokuCell::remove_value(int v, bool emit_signal)
{
	IntList old_candidates = candidates;

	int span = sudoku->span();
	if(1 <= v && v <= span && candidates[v])
	{
		candidates[v] = 0;
		update_text();
		if(emit_signal)
			emit value_changed(row, col,
					value_settled, old_candidates,
					value_settled, candidates);
	} else if(v == -1) {
		for(int i = span; i; --i)
			if(candidates[i])
			{
				candidates[i] = 0;
				update_text();
				if(emit_signal)
					emit value_changed(row, col,
							value_settled, old_candidates,
							value_settled, candidates);
				break;
			}
	}
}

void SudokuCell::update_style()
{
	QString text_color = DEFAULT_TEXT_COLOR,
			bg_color = DEFAULT_BG_COLOR,
			font_style = "normal",
			font_weight = "normal";

	if(!initial_status) text_color = FILLED_TEXT_COLOR;
	if(is_vertical_selected || is_horizontal_selected)
		bg_color = SELECTED_BG_COLOR;
	if(is_lighted)
	{
		font_style = "italic";
		font_weight = "900";
	}

	setStyleSheet("background-color: " + bg_color + ";"
				  "color: " + text_color + ";"
				  "font-style: " + font_style + ";"
				  "font-weight: " + font_weight + ";");
}

void SudokuCell::update_font()
{
	QString text = this->text();
	QRect rect_lbl = geometry().adjusted(5, 5, -5, -5);
	QFont font = this->font();

	font = fit_font_with_text(font, text, rect_lbl);

	setFont(font);
}

void SudokuCell::update_text()
{
	int last_v = 0;
	int digit_num = count(candidates.begin(), candidates.end(), 1);
//	int line_num = std::ceil(std::sqrt(1.0 * digit_num) - 1.0e-9);
	int line_num = std::round(std::sqrt(1.0 * sudoku->span()));

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

	if(!value_settled)
	{
		// using space to fill
		for(int i = digit_num; i < sudoku->span(); ++i)
			if(i % line_num == 0)
				str += "\n ";
			else str += "  ";
	}

	setText(str);
	update_font();

	if(digit_num == 0)
		sudoku->reset(row, col);
	else if(digit_num == 1 && value_settled)
		sudoku->set(row, col, last_v);
}

void SudokuCell::mousePressEvent(QMouseEvent *ev)
{
//	if(initial_status == 0)
//	{
	emit_selected_signal();

	if(ev->button() == Qt::RightButton)
		set_mark(!is_marked);
//	} else emit free_signal();
}

void SudokuCell::keyPressEvent(QKeyEvent *ev)
{
	int key = ev->key();
	if(!is_initial_status())
	{
		if(Qt::Key_1 <= key && key <= Qt::Key_9)
		{
			int val = key - Qt::Key_0;
			if(QApplication::keyboardModifiers() & Qt::ControlModifier)
				add_value(val);
			else set_value(val);
		} else if(key == Qt::Key_Backspace)
			remove_value(-1);
	}

	if(key == Qt::Key_Left || key == Qt::Key_Right || key == Qt::Key_Down
			|| key == Qt::Key_Up || key == Qt::Key_Tab)
		emit move_focus(key);
}

void SudokuCell::paintEvent(QPaintEvent *ev)
{
	QLabel::paintEvent(ev);
	QPainter p(this);

	if(is_marked && !initial_status)
	{
		QPainterPath path;
		path.moveTo(0, 0);
		path.lineTo(0, 5);
		path.lineTo(5, 0);
		path.closeSubpath();
		p.fillPath(path, QBrush(qRgb(44, 44, 44)));
	}
}

void SudokuCell::set_initial_status(int v)
{
	initial_status = v;
	fill(candidates.begin(), candidates.end(), 0);
	value_settled = false;

	if(v) set_value(v);
	else update_text();

	update_style();
	set_mark(false);
}

int SudokuCell::get_initial_status() const
{
	return initial_status;
}

void SudokuCell::clear_values(bool emit_signal)
{
	IntList old_candidates = candidates;
	fill(candidates.begin(), candidates.end(), 0);
	update_text();
	update_style();

	if(emit_signal)
		emit value_changed(row, col,
				value_settled, old_candidates,
				value_settled, candidates);
}

IntList SudokuCell::get_candidates() const
{
	return candidates;
}

void SudokuCell::emit_selected_signal()
{
	emit selected_signal(this);
}

void SudokuCell::light_value(int v)
{
	if(v && get_value() == v)
	{
		is_lighted = true;
	} else is_lighted = false;
	update_style();
	update_text();
}

void SudokuCell::set_mark(bool mark)
{
	is_marked = mark;
	this->update();
}

int SudokuCell::get_value() const
{
	if(value_settled && count(candidates.begin(), candidates.end(), 1) == 1)
		return std::find(candidates.begin(), candidates.end(), 1) - candidates.begin();
	return 0;
}

void SudokuCell::recover_status(bool value_settled, IntList candidates)
{
	this->value_settled = value_settled;
	this->candidates = candidates;
	update_text();
	update_style();
}

bool SudokuCell::is_initial_status() const
{
	return initial_status != 0;
}

bool SudokuCell::is_value_settled() const
{
	return value_settled;
}
