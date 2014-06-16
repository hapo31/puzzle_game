#include"Title.h"
#include"resource_manager.h"
#include"WindowSize.h"
#include"Controler_Manager.h"
#include"Level_Manager.h"

using namespace level;
using namespace res;

namespace
{
	auto control = input::Controler_Manager::get_Instance();
	auto ctrl1 = control->GetControler(DX_INPUT_KEY_PAD1);
	auto level_mng = level::Level_Manager::get_Instance();
}

Title::Title()
{
	menu[0] = std::make_pair( "GAME START" ,LEVEL_ID::GAME_MAIN);
	menu[1] = std::make_pair( "Config" , LEVEL_ID::CONFIG);
	menu[2] = std::make_pair( "Exit" ,LEVEL_ID::EXIT);
}

bool Title::init()
{
	SetDrawBright(0, 0, 0);
	if (!loading)
	{
		auto gr_loader = get_loader<graphic_controler>();
		auto font_loader = get_loader<font_controler>();
		auto mng = res::Resource_mng::get_Instance();

		font_loader->set_fontinfo("MS Gothic", 40, 3, DX_FONTTYPE_ANTIALIASING_8X8);
		resdata[0] = mng->Regist("data/title.png", gr_loader);
		resdata[1] = mng->Regist("MS Gothic", font_loader);
		loading = true;
		bright = 0;
		resdata[0].Load();
		for (auto& it : resdata)
			it.Load();
		return false;
	}
	else
	{
		if (!resdata[0].is_Loaded() ||
			!resdata[1].is_Loaded())
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

int Title::execute()
{
	if (bright < 255 && !end_flag)
	{
		bright += 5;
	}
	else if (bright > 0 && end_flag)
	{
		bright -= 5;
	}
	SetDrawBright(bright, bright, bright);
	if (!end_flag)
	{
		if (ctrl1->at(input::UP) == 1)
		{
			menu_select = menu_select - 1 < 0 ? menu_select = 2 : menu_select - 1;
		}
		if (ctrl1->at(input::DOWN) == 1)
		{
			menu_select = (menu_select + 1) % 3;
		}
		if (ctrl1->at(input::A) == 1)
		{
			switch (menu_select)
			{
			case START:
				level_mng->set_next_level(level::GAME_MAIN);
				end_flag = true;
				break;
			case EXIT:
				exit(0);
			default:
				break;
			}
		}
	}

	DrawBox(0, 0, WindowWidth, WindowHeight, GetColor(255, 255, 255), true);
	DrawGraph(WindowWidth / 2 - 200, 20, resdata[0], true);
	int menu_color[3] = { GetColor(0, 0, 0), GetColor(0, 0, 0), GetColor(0, 0, 0) };
	menu_color[menu_select] = GetColor(255, 0, 0);
	{
		int i = 0;
		for (auto & it : menu)
		{
			DrawFormatStringToHandle(WindowWidth / 3, WindowHeight - 250 + i * 60, menu_color[i],resdata[1] ,"%s", it.first.c_str());
			++i;
		}
	}
	return bright;
}

bool Title::end()
{
	if (execute() <= 0)
	{
		for (auto& it : resdata)
			it.Delete();
		this->loading = false;
		end_flag = false;
		return true;
	}
	else
	{
		return false;
	}
}