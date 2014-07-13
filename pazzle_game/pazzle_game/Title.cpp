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
	menu[0] = std::make_pair( "Game Start" ,LEVEL_ID::GAME_MAIN);
	menu[1] = std::make_pair( "Config" , LEVEL_ID::CONFIG);
	menu[2] = std::make_pair( "Exit" ,LEVEL_ID::EXIT);
}

bool Title::init()
{
	if (!loading)
	{
		auto gr_loader = get_loader<graphic_controler>();
		auto font_loader = get_loader<font_controler>();
		auto mng = res::Resource_mng::get_Instance();


		font_loader->set_fontinfo("MS Gothic", 40, 3, DX_FONTTYPE_ANTIALIASING_8X8);
		resdata[0] = mng->Regist("data/title.png", gr_loader);
		resdata[1] = mng->Regist("MS Gothic", font_loader);
		loading = true;

		resdata[0]->Load();
		for (auto& it : resdata)
			it->Load();
		return false;
	}
	else
	{
		if (resdata[0]->is_Loaded() ||
			resdata[1]->is_Loaded())
		{

			//読み込みが終わっていればフェードインを設定
			level_mng->set_fadein(30);
			loading = false;
			return true;
		}
		else
		{
			return false;
		}
	}
}

int Title::execute()
{
	//フェードアウト処理が終わっていれば入力処理をする
	if (level_mng->get_fadeend() && !end_flag)
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
				level_mng->set_fadeout(30);
				end_flag = true;
				break;
			case EXIT:
				level_mng->game_exit();
				break;
			default:
				break;
			}
		}
	}

	DrawBox(0, 0, WindowWidth, WindowHeight, GetColor(255, 255, 255), true);
	DrawGraph(WindowWidth / 2 - 200, 20, *resdata[0], true);
	int menu_color[3] = { GetColor(0, 0, 0), GetColor(0, 0, 0), GetColor(0, 0, 0) };
	menu_color[menu_select] = GetColor(255, 0, 0);
	{
		int i = 0;
		for (auto & it : menu)
		{
			DrawFormatStringToHandle(WindowWidth / 3, WindowHeight - 250 + i * 60, menu_color[i], *resdata[1] ,"%s", it.first.c_str());
			++i;
		}
	}
	return 0;
}

bool Title::end()
{
	if (level_mng->get_fadeend())
	{
		for (auto& it : resdata)
			it->Delete();
		this->loading = false;
		end_flag = false;
		return true;
	}
	else
	{
		execute();
		return false;
	}
}