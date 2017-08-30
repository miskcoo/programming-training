#ifndef ACTION_QUEUE_H
#define ACTION_QUEUE_H

#include <deque>
#include "utils.h"

struct ActionInfo
{
	int row, col;
	bool value_settled_old, value_settled_new;
	IntList candidates_old, candidates_new;

	ActionInfo() : row(-1), col(-1) {}
};

class ActionQueue
{
public:
	ActionQueue(int max_queue);
	void add_action(
		int row, int col,
		bool value_settled_old, IntList candidates_old,
		bool value_settled_new, IntList candidates_new
	);
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
