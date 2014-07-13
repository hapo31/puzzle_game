#include"Level_Manager.h"
#include"WindowSize.h"
#include"resource_manager.h"

using namespace level;

bool Level_Manager::regist_level(int id, level_t level)
{
	return levels.emplace(id, level).second;
}

void Level_Manager::set_fadein(int frame)
{
	fade.set(fade_type::in, frame);
}

void Level_Manager::set_fadeout(int frame)
{
	fade.set(fade_type::out, frame);
}

bool Level_Manager::set_next_level(int id)
{
	//頭にあった場合はinitだけ実行する
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
	//画面のあかるさ管理
	fade.update();
	switch (state)
	{
		//初期化
	case INIT:
		
		if (level_stack.front()->init())
		{
			state = EXE;
#ifdef _DEBUG
			{
				char buf[255];
				sprintf_s<sizeof buf>(buf, "Init Success ID:%d\n", level_stack.front()->get_myid());
				OutputDebugStringA(buf);
			}
#endif
		}
		//初期化が終わっていなければローディング画面表示
		else
		{
			SetDrawBright(255, 255, 255);
			DrawString((frame * 10 ) % WindowWidth, 0, "Now Loading...", GetColor(255, 255, 0));
		}
		break;
		//実行
	case EXE:
		level_stack.front()->execute();
		break;
	
	case END:
		exit(0);
		break;
		
		//Level終了処理
	case NEXT:
	case END_BACK:

		if (level_stack.empty() || level_stack.front()->end())
		{
#ifdef _DEBUG
			{
				char buf[256];
				sprintf_s<sizeof buf>(buf, "End Success ID:%d\n", level_stack.empty() ? -1 : level_stack.front()->get_myid());
				OutputDebugStringA(buf);
			}
#endif
			switch (state)
			{
			case NEXT:
				level_stack.push_front(levels[next_id]);
#ifdef _DEBUG
				{
					char buf[256];
					sprintf_s<sizeof buf>(buf, "Switched Level ID:%d\n", next_id);
					OutputDebugStringA(buf);
				}
#endif
				next_id = 0;
				break;
			case END_BACK:
				level_stack.pop_front();
				break;
			}
			state = INIT;
		}
	}
	++frame;
	return level_stack.front();
}

//Levelを一個戻す
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

bool Level_Manager::game_exit()
{
	state = END;
	return true;
}