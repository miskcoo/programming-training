#ifndef DRAUGHTS_H
#define DRAUGHTS_H

#include "utils.h"
#include <vector>
using std::vector;

struct DraughtsInfo
{
	enum Types
	{
		empty, black, white
	};

	bool is_king;
	int x, y, type;
	DraughtsInfo(int x = -1, int y = -1, int type = empty, bool is_king = false)
		: is_king(is_king), x(x), y(y), type(type) {}
};

struct TraceInfo
{
	DraughtsInfo from, to;
	vector<DraughtsInfo> kill;
};

class Draughts
{
public:
	Draughts();

	vector<DraughtsInfo> get_avail_chess(DraughtsInfo::Types player);
	vector<DraughtsInfo> get_avail_move(DraughtsInfo::Types player);
	vector<TraceInfo> move(DraughtsInfo src, DraughtsInfo dest);
};

#endif
