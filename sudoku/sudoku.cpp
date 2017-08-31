#include <algorithm>
#include <cstdlib>
#include <map>
#include "sudoku.h"
#include "dancing_link.h"

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

bool Sudoku::is_empty() const
{
	return count(grids_.begin(), grids_.end(), 0) == span_ * span_;
}

bool Sudoku::is_solved() const
{
	return std::count(grids_.begin(), grids_.end(), 0) == 0
		&& is_consistent();
}

void Sudoku::clear()
{
	fill(grids_.begin(), grids_.end(), 0);
}

int Sudoku::span() const
{
	return span_;
}

bool Sudoku::reset(int x, int y)
{
	return set(x, y, 0);
}

bool Sudoku::set(int x, int y, int v)
{
	if(!_check_coord(x, y))
		return false;

	grids_[x * span_ + y] = v;
	return true;
}

bool Sudoku::try_set(int x, int y, int v)
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

Sudoku Sudoku::solve(bool check_unique, bool *is_unique) const
{
	int span2 = span_ * span_;
	int row_num = span2 * span_;
	int col_num = span2 * 4;

	std::map<int, std::pair<int, int>> row_map;
	DancingLink dlx(row_num, col_num);
	for(int r = 0; r != span_; ++r)
	{
		for(int c = 0; c != span_; ++c)
		{
			int now_v = get(r, c);
			for(int v = 0; v != span_; ++v)
			{
				if(now_v != 0 && now_v != v + 1)
					continue;
				int g = r / size_ * size_ + c / size_;
				int row_id = dlx.append_row( {
					r * span_ + c + 1,			 // existance
					r * span_ + v + span2 + 1,	 // rows consistency
					c * span_ + v + span2 * 2 + 1, // columns consistency
					g * span_ + v + span2 * 3 + 1  // grids consistency
				} );

				row_map[row_id] = std::make_pair(r * span_ + c, v + 1);
			}
		}
	}

	IntList dlx_rows;
	if(!check_unique)
		dlx_rows = dlx.solve();
	else *is_unique = dlx.solve_unique(dlx_rows);
	Sudoku ans = *this;

	for(int row : dlx_rows)
	{
		auto info = row_map[row];
		ans.grids_[info.first] = info.second;
	}

	if(dlx_rows.size() != (unsigned)span2)
		ans.clear();

	return ans;
}

void Sudoku::exchange_column(int c1, int c2)
{
	if(0 <= c1 && c1 < span_ && 0 <= c2 && c2 < span_)
	{
		for(int r = 0; r != span_; ++r)
			std::swap(grids_[r * span_ + c1], grids_[r * span_ + c2]);
	}
}

void Sudoku::exchange_row(int r1, int r2)
{
	if(0 <= r1 && r1 < span_ && 0 <= r2 && r2 < span_)
	{
		for(int c = 0; c != span_; ++c)
			std::swap(grids_[r1 * span_ + c], grids_[r2 * span_ + c]);
	}
}

void Sudoku::exchange_number(int v1, int v2)
{
	if(1 <= v1 && v1 <= span_ && 1 <= v2 && v2 <= span_)
	{
		for(int& cell : grids_)
		{
			if(cell == v1) cell = v2;
			else if(cell == v2) cell = v1;
		}
	}
}

void Sudoku::random_exchange(int times)
{
	for(int i = 0; i != times; ++i)
	{
		int type = std::rand() % 5;
		if(type == 0)
		{
			int v1 = std::rand() % span_ + 1;
			int v2 = std::rand() % span_ + 1;
			exchange_number(v1, v2);
		} else if(type == 1 || type == 2) {
			int l = std::rand() % size_ * size_;
			int l1 = l + std::rand() % size_;
			int l2 = l + std::rand() % size_;
			if(type == 1) exchange_row(l1, l2);
			else exchange_column(l1, l2);
		} else {
			int l1 = std::rand() % size_ * size_;
			int l2 = std::rand() % size_ * size_;
			for(int i = 0; i != size_; ++i)
				if(type == 3) exchange_row(l1 + i, l2 + i);
				else exchange_column(l1 + i, l2 + i);
		}
	}
}

void Sudoku::random_sudoku(
	int init_cells,
	int empty_cells_lb,
	int line_lb,
	bool is_unique,
	int digging_seq_type)
{
	int span2 = span_ * span_;
	auto try_random = [=] () -> int {
		clear();

		// randomly fill `init_cells` cells
		auto try_random_one = [this] () -> bool {
			int r = std::rand() % span_, c = std::rand() % span_;
			if(get(r, c) != 0) return false;
			IntList avail = get_available(r, c);
			if(avail.empty()) return false;
			set(r, c, avail[std::rand() % avail.size()]);
			return true;
		};

		for(int count = 0; count < init_cells; count += try_random_one());

		// generate a valid solution
		*this = solve();
		if(is_empty()) return 0;

		// random exchange row/column/value
		random_exchange(30);

		// generate digging sequence
		IntList digging_seq;

		switch(digging_seq_type)
		{
		case DIGGING_RANDOM:
			for(int i = 0; i != span2; ++i)
				digging_seq.push_back(i);
			std::random_shuffle(digging_seq.begin(), digging_seq.end());
			break;
		case DIGGING_S:
			for(int r = 0; r != span_; ++r)
				for(int c = 0; c != span_; ++c)
				{
					int nc = (r & 1) ? span_ - c - 1 : c;
					digging_seq.push_back(r * span_ + nc);
				}
			break;
		case DIGGING_Z:
			for(int i = 0; i != span2; ++i)
				digging_seq.push_back(i);
			break;
		}

		// random peturb digging sequence
		for(int i = 0; i != span_; ++i)
			std::swap(digging_seq[std::rand() % span2], digging_seq[std::rand() % span2]);

		// digging empty cells
		auto try_dig_one = [=] (int pos) -> bool {
			int r = pos / span_, c = pos % span_;
			int cnt_r = 0, cnt_c = 0;
			for(int i = 0; i != span_; ++i)
			{
				if(get(r, i)) ++cnt_c;
				if(get(i, c)) ++cnt_r;
			}

			if(cnt_r <= line_lb || cnt_c <= line_lb)
				return false;

			if(is_unique)
			{
				Sudoku new_sudoku = *this;
				new_sudoku.reset(r, c);

				bool uniqueness;
				new_sudoku = new_sudoku.solve(true, &uniqueness);

				if(uniqueness)
					this->reset(r, c);
				return uniqueness;
			} else {
				this->reset(r, c);
				return true;
			}
		};

		int dig_num = 0;
		for(int pos : digging_seq)
		{
			dig_num += try_dig_one(pos);
			if(dig_num >= empty_cells_lb)
				break;
		}

		return dig_num;
	};

	while(try_random() < empty_cells_lb);
	random_exchange(50);
}

Sudoku Sudoku::generate(int size, int level)
{
	bool is_unique = true;
	int given = 81, lb = 0, random_type = DIGGING_RANDOM;

	switch(level)
	{
	case 1:
		lb = 5;
		given = 50 + std::rand() % 8;
		break;
	case 2:
		lb = 4;
		given = 50 + std::rand() % 5;
		break;
	case 3:
		lb = 4;
		given = 40 + std::rand() % 8;
		break;
	case 4:
		lb = 4;
		given = 36 + std::rand() % 6;
		break;
	case 5:
		lb = 3;
		given = 36 + std::rand() % 6;
		break;
	case 6:
		lb = 3;
		given = 28 + std::rand() % 3;
		break;
	case 7:
		lb = 2;
		given = 28 + std::rand() % 3;
		break;
	case 8:
		lb = 2;
		given = 22 + std::rand() % 5;
		break;
	case 9:
		lb = 1;
		random_type = DIGGING_S;
		given = 22 + std::rand() % 3;
		break;
	case 10:
		lb = 0;
		random_type = DIGGING_Z;
		given = 22 + std::rand() % 2;
		break;
	}

	Sudoku ret(size);

	if(size > 3)
	{
		double p = ret.size_ / 3.0 + 1.0e-10;
		lb *= p;
		given *= p * p * p * p;
		is_unique = false;
		random_type = DIGGING_RANDOM;
	}

	ret.random_sudoku(11, ret.span() * ret.span() - given, lb, is_unique, random_type);
	return ret;
}
