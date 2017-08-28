#include <algorithm>
#include "sudoku.h"

Sudoku::Sudoku(int size)
	: size_(size), span_(size * size), grids_(span_ * span_, 0)
{
}

bool Sudoku::_check_coord(int x, int y) const
{
	if(x < 0 || y < 0 || x >= span_ || y >= span_)
		return false;
	return true;
}

bool Sudoku::reset(int x, int y)
{
	return set(x, y, 0);
}

bool Sudoku::set(int x, int y, int v)
{
	if(!_check_coord(x, y))
		return false;

	IntList avail = get_available(x, y);
	if(!v || find(avail.begin(), avail.end(), v) != avail.end())
	{
		grids_[x * span_ + y] = v;
		return true;
	}

	return false;
}

int Sudoku::get(int x, int y) const
{
	if(!_check_coord(x, y))
		return -1;
	return grids_[x * span_ + y];
}

bool Sudoku::is_consistent() const
{
	int now = 1;
	IntList mark(span_ + 1, 0);

	// check rows
	for(int i = 0; i != span_; ++i, ++now)
	{
		for(int j = 0; j != span_; ++j)
		{
			int g = get(i, j);
			if(g && mark[g] == now)
				return false;
			mark[g] = now;
		}
	}

	// check columns
	for(int i = 0; i != span_; ++i, ++now)
	{
		for(int j = 0; j != span_; ++j)
		{
			int g = get(j, i);
			if(g && mark[g] == now)
				return false;
			mark[g] = now;
		}
	}

	// check grids
	for(int i = 0; i != size_; ++i)
	{
		for(int j = 0; j != size_; ++j, ++now)
		{
			for(int k = 0; k != size_; ++k)
			{
				for(int w = 0; w != size_; ++w)
				{
					int x = i * size_ + k, y = j * size_ + w;
					int g = get(x, y);
					if(g && mark[g] == now)
						return false;
					mark[g] = now;
				}
			}
		}
	}

	return true;
}

IntList Sudoku::get_available(int x, int y) const
{
	if(!_check_coord(x, y))
		return {};

	if(get(x, y))
		return { get(x, y) };

	IntList mark(span_ + 1, 0);

	// check row and column
	for(int j = 0; j != span_; ++j)
	{
		mark[get(x, j)] = 1;
		mark[get(j, y)] = 1;
	}

	// check grid
	int grid_x = x / size_, grid_y = y / size_;
	for(int i = 0; i != 3; ++i)
		for(int j = 0; j != 3; ++j)
			mark[get(grid_x * size_ + i, grid_y * size_ + j)] = 1;

	IntList avail;
	for(int i = 1; i <= span_; ++i)
		if(!mark[i]) avail.push_back(i);
	return avail;
}

Sudoku Sudoku::solve() const
{
	// TODO
	return {};
}
