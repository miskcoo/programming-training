#include "action_queue.h"

ActionQueue::ActionQueue(int max_queue)
	: max_queue(max_queue), cur_pos(0)
{
}

void ActionQueue::add_action(int r, int c, int v, IntList candidates)
{
	ActionInfo action;
	action.row = r;
	action.col = c;
	action.value = v;
	action.candidates = candidates;

	actions.erase(actions.begin() + cur_pos, actions.end());
	actions.push_back(action);
	if(++cur_pos > max_queue)
	{
		cur_pos = max_queue;
		actions.pop_front();
	}
}

ActionInfo ActionQueue::forward()
{
	if(is_forwardable())
		return actions[cur_pos++];
	return { {}, -1, -1, -1 };
}

ActionInfo ActionQueue::backward()
{
	if(is_backwardable())
		return actions[--cur_pos];
	return { {}, -1, -1, -1 };
}

bool ActionQueue::is_forwardable() const
{
	return (unsigned)cur_pos < actions.size();
}

bool ActionQueue::is_backwardable() const
{
	return cur_pos > 0;
}

void ActionQueue::reset()
{
	cur_pos = 0;
	actions.clear();
}
