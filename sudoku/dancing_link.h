#ifndef DANCING_LINK_H
#define DANCING_LINK_H
#include "utils.h"

class DancingLink
{
	int col_num, row_num;
	int node_now, row_now;
	IntList column_node;

	IntList L, R, U, D;
	IntList belong_row, belong_col;

	IntList answer;
	int answer_num;
public:
	DancingLink(int row_num, int col_num) { init(row_num, col_num); }

	void init(int row_num, int col_num);
	void append_row(const IntList& cols);
	IntList solve();

private:
	void remove(int col);
	void restore(int col);
	int dfs(int level);
};

#endif // DANCING_LINK_H
