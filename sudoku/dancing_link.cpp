#include "dancing_link.h"

void DancingLink::init(int row_num, int col_num)
{
	this->row_num = row_num;
	this->col_num = col_num;

	int node_num = (row_num + 1) * (col_num + 1) + col_num + 2;
	column_node.assign(col_num + 1, 0);
	belong_row.assign(node_num, 0);
	belong_col.assign(node_num, 0);
	L.assign(node_num, 0); R.assign(node_num, 0);
	U.assign(node_num, 0); D.assign(node_num, 0);

	row_now = 1;
	node_now = col_num + 1;	
	for(int i = 0; i <= col_num; ++i)
	{
		L[i] = i - 1;
		R[i] = i + 1;
		U[i] = D[i] = i;
	}

	L[0] = col_num;
	R[col_num] = 0;
}

int DancingLink::append_row(const IntList& cols)
{
	int first_node_now = node_now;
	for(int col : cols)
	{
		belong_row[node_now] = row_now;
		belong_col[node_now] = col;
		L[node_now] = node_now - 1;
		R[node_now] = node_now + 1;
		D[node_now] = col;
		U[node_now] = U[col];
		D[U[col]] = node_now;
		U[col] = node_now;
		++column_node[col];
		++node_now;
	}

	L[first_node_now] = node_now - 1;
	R[node_now - 1] = first_node_now;
	return row_now++;
}

void DancingLink::remove(int col)
{
	L[R[col]] = L[col];
	R[L[col]] = R[col];
	for(int i = D[col]; i != col; i = D[i])
	{
		for(int j = R[i]; j != i; j = R[j])
		{
			U[D[j]] = U[j];
			D[U[j]] = D[j];
			--column_node[belong_col[j]];
		}
	}
}

void DancingLink::restore(int col)
{
	for(int i = U[col]; i != col; i = U[i])
	{
		for(int j = L[i]; j != i; j = L[j])
		{
			U[D[j]] = j;
			D[U[j]] = j;
			++column_node[belong_col[j]];
		}
	}

	L[R[col]] = col;
	R[L[col]] = col;
}

int DancingLink::dfs(int level, const std::function<bool()>& callback)
{
	if(R[0] == 0)
	{
		answer_num = level;
		return callback();
	}

	int min_node_in_col = R[0];
	for(int i = R[0]; i != 0; i = R[i])
	{
		if(column_node[i] < column_node[min_node_in_col])
			min_node_in_col = i;
	}

	remove(min_node_in_col);

	int m = min_node_in_col;
	for(int i = D[m]; i != m; i = D[i])
	{
		answer[level] = belong_row[i];
		for(int k = R[i]; k != i; k = R[k])
			remove(belong_col[k]);
		if(dfs(level + 1, callback)) return 1;
		for(int k = L[i]; k != i; k = L[k])
			restore(belong_col[k]);
	}

	restore(min_node_in_col);
	return 0;
}

IntList DancingLink::solve()
{
	answer_num = 0;
	answer.assign(row_num + 1, 0);
	if(!dfs(0, []() { return true; })) return {};
	return IntList(answer.begin(), answer.begin() + answer_num);
}

bool DancingLink::solve_unique(IntList& ans)
{
	int count = 0;
	answer_num = 0;
	answer.assign(row_num + 1, 0);
	dfs(0, [&]() { return ++count > 1; });
	if(count == 0)
	{
		ans = {};
		return false;
	} else {
		ans = IntList(answer.begin(), answer.begin() + answer_num);
		return count == 1;
	}
}
