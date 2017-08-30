#ifndef ACTION_QUEUE_H
#define ACTION_QUEUE_H

#include <deque>
#include "utils.h"

struct ActionInfo
{
	IntList candidates;
	int row, col, value;
};

class ActionQueue
{
public:
	ActionQueue(int max_queue);
	void add_action(int row, int col, int value, IntList candidates = {});
	ActionInfo forward();
	ActionInfo backward();
	bool is_forwardable() const;
	bool is_backwardable() const;
	void reset();
private:
	int max_queue, cur_pos;
	std::deque<ActionInfo> actions;
};

#endif // ACTION_QUEUE_H
