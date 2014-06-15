#include"Level_Manager.h"

using namespace level;

bool Level_Manager::regist_level(int id, level_t level)
{
	return levels.emplace(id, level).second;
}


bool Level_Manager::set_next_level(int id)
{
	//“ª‚É‚ ‚Á‚½ê‡‚Íinit‚¾‚¯ŽÀs‚·‚é
	if (!level_stack.empty() && level_stack.front()->get_myid() == id)
	{
		state = INIT;
		return false;
	}
	else
	{
		next_id = id;
		state = NEXT;
		return true;
	}
}

level_t Level_Manager::execute()
{
	switch (state)
	{
	case INIT:
		if (level_stack.front()->init())
			state = EXE;
		break;
	case EXE:
		level_stack.front()->execute();
		break;
	case NEXT:
	case END:
	case END_BACK:

		if (level_stack.empty() || level_stack.front()->end())
		{
			switch (state)
			{
			case NEXT:
				level_stack.push_front(levels[next_id]);

				next_id = 0;
				break;
			case END_BACK:
				level_stack.pop_back();
			case END:

				break;
			}
			state = INIT;
		}
	}
	++frame;
	return level_stack.front();
}

//Level‚ðˆêŒÂ–ß‚·
bool Level_Manager::back_level()
{
	if (!level_stack.empty())
	{
		state = END_BACK;
		return true;
	}
	else
	{
		return false;
	}
}