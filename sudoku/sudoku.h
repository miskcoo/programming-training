#ifndef SUDOKU_H
#define SUDOKU_H

#include "utils.h"

#define DIGGING_RANDOM 0
#define DIGGING_S 1
#define DIGGING_Z 2

#define SUDOKU_LEVEL_MIN 1
#define SUDOKU_LEVEL_MAX 10

class Sudoku
{
	int size_, span_;
	// 0 means not being filled yet
	IntList grids_;
public:
	Sudoku(int size = 3);
	~Sudoku() = default;

	// check whether all grids are unfilled
	bool is_empty() const;
	// check whether the Sudoku is solved
	bool is_solved() const;
	// set all grids to be unfilled, namely 0
	void clear();
	// set (x, y) to be unfilled, namely 0
	bool reset(int x, int y);
	// set (x, y) to be of value v
	bool set(int x, int y, int v);
	// try to set (x, y) to be of value v
	bool try_set(int x, int y, int v);
	// get the value of (x, y)
	int get(int x, int y) const;
	// check whether the current status is consistent
	bool is_consistent() const;
	// get available candidate numbers of (x, y)
	IntList get_available(int x, int y) const;
	// solve this sudoku puzzle
	Sudoku solve(bool check_unique = false, bool* is_unique = nullptr) const;
	// get span
	int span() const;

public:
	void exchange_number(int v1, int v2);
	void exchange_row(int r1, int r2);
	void exchange_column(int c1, int c2);
	void random_exchange(int times);
	void random_sudoku(
		int init_cells,
		int empty_cells_lb,
		int line_lb,
		bool is_unique = true,
		int digging_seq_type = DIGGING_Z
	);

	static Sudoku generate(int size, int level);
private:
	bool _check_coord(int x, int y) const;
};

#endif // SUDOKU_H

