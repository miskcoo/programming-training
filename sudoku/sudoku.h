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

	bool reset(int x, int y);
	bool set(int x, int y, int v);
	int get(int x, int y) const;
	bool is_consistent() const;
	IntList get_available(int x, int y) const;
	Sudoku solve() const;

private:
	bool _check_coord(int x, int y) const;
};

#endif // SUDOKU_H

