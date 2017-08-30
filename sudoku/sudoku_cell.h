#ifndef SUDOKUCELL_H
#define SUDOKUCELL_H

#include <QtWidgets>
#include <memory>
#include "sudoku.h"
#include "utils.h"

class SudokuCell : public QLabel
{
	Q_OBJECT
public:
	explicit SudokuCell(int row, int col, std::shared_ptr<Sudoku> sudoku, QWidget *parent = 0);

	void set_initial_status(int v);
	int get_initial_status() const;

	void set_hint_value(int v);
	IntList get_candidates() const;

	void add_value(int v, bool emit_signal = true);
	void remove_value(int v, bool emit_signal = true);
	void clear_values(bool emit_signal = true);

signals:
	void selected_signal(SudokuCell*);
	void free_signal();
	void value_changed(int r, int w, int v, IntList candidates = {});

public slots:
	void vertical_selected();
	void horizontal_selected();
	void free_selection();

protected:
	void mousePressEvent(QMouseEvent*);
	void keyPressEvent(QKeyEvent*);
	void focusInEvent(QFocusEvent*);
	void focusOutEvent(QFocusEvent*);

private:
	void update_text();
	void update_font();
	void update_style();

private:
	int row, col, initial_status;
	std::shared_ptr<Sudoku> sudoku;

	bool is_vertical_selected, is_horizontal_selected;

	int value_fixed;
	IntList candidates;
};

#endif // SUDOKUCELL_H
