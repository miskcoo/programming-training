#ifndef SUDOKU_H
#define SUDOKU_H

#include "utils.h"

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
	// set all grids to be unfilled, namely 0
	void clear();
	// set (x, y) to be unfilled, namely 0
	bool reset(int x, int y);
	// set (x, y) to be of value v
	bool set(int x, int y, int v);
	// get the value of (x, y)
	int get(int x, int y) const;
	// check whether the current status is consistent
	bool is_consistent() const;
	// get available candidate numbers of (x, y)
	IntList get_available(int x, int y) const;
	// solve this sudoku puzzle
	Sudoku solve() const;

private:
	bool _check_coord(int x, int y) const;
};

#endif // SUDOKU_H

