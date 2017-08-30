#include "action_queue.h"

ActionQueue::ActionQueue(int max_queue)
	: max_queue(max_queue), cur_pos(0)
{
}

void ActionQueue::add_action(
	int row, int col,
	bool value_settled_old, IntList candidates_old,
	bool value_settled_new, IntList candidates_new)
{
	ActionInfo action;
	action.row = row;
	action.col = col;
	action.candidates_new = candidates_new;
	action.candidates_old = candidates_old;
	action.value_settled_new = value_settled_new;
	action.value_settled_old = value_settled_old;

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
	return ActionInfo();
}

ActionInfo ActionQueue::backward()
{
	if(is_backwardable())
		return actions[--cur_pos];
	return ActionInfo();
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
