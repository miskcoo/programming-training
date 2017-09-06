#ifndef DRAUGHTS_H
#define DRAUGHTS_H

#include "utils.h"
#include <vector>
#include <utility>
#include <functional>
using std::vector;
using std::pair;

struct DraughtsInfo
{
	enum Types
	{
		empty, black, white
	};

	bool is_king;
	int x, y;
	Types type;
	DraughtsInfo(int x = -1, int y = -1, Types type = empty, bool is_king = false)
		: is_king(is_king), x(x), y(y), type(type) {}

	void set_empty() { type = empty, is_king = false; }
};

typedef vector<DraughtsInfo> DraughtsTrace;

class Draughts
{
public:
	Draughts();

	vector<DraughtsInfo> get_avail_chess(DraughtsInfo::Types player);
	pair<int, vector<DraughtsTrace>> get_avail_move(int x, int y);
	DraughtsTrace move(const vector<pair<int, int>>&);
	DraughtsInfo get_info(int x, int y) const;
	bool is_empty(int x, int y) const;

private:
	bool dfs_jump(
		int step, int x, int y, bool is_king,
		DraughtsInfo::Types type,
		std::function<bool(int, DraughtsInfo*, DraughtsInfo*)>);
	bool check_coord_avail(int x, int y) const;

private:
	DraughtsInfo status[10][10];
	DraughtsInfo dfs_info[100], eat_info[100];
	int mark[10][10];
};

#endif
