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
	menu[0] = std::make_pair( "Normal Mode" ,LEVEL_ID::GAME_MAIN);
	menu[1] = std::make_pair( "VSMode (score attack)" , LEVEL_ID::VSGAME);
	menu[2] = std::make_pair( "Exit" ,LEVEL_ID::EXIT);
}

bool Title::init(int )
{
	if (!loading)
	{
		auto gr_loader = get_loader<graphic_controler>();
		auto font_loader = get_loader<font_controler>();
		auto mng = res::Resource_mng::get_Instance();

		background_color = util::translater<color>(std::move(color(240, 248, 255)), std::move(color(240, 248, 255)), 1);
		background_color.get_now();
		font_loader->set_fontinfo("MS Gothic", 40, 3, DX_FONTTYPE_ANTIALIASING_8X8);
		resdata[0] = mng->Regist("data/title2.png", gr_loader);
		resdata[1] = mng->Regist("MS Gothic", font_loader);
		loading = true;

		resdata[0]->Load();
		for (auto& it : resdata)
			it->Load();
		return false;
	}
	else
	{
		//全てのリソースが読み込み終わっているかをチェック
		for (auto&& t : resdata)
		{
			if (!t->is_Loaded())
			{
				return false;
			}
		}
		//読み込みが終わっていればフェードインを設定
		level_mng->set_fadein(30);
		loading = false;
		return true;

	}
}

int Title::execute(int )
{
	color colors [] = { color(240, 248, 255), color(255, 255, 224), color(119, 135, 153) };
	const int nextcolor_frames = 120;

	//フェードアウト処理が終わっていれば入力処理をする
	if (!end_flag && level_mng->fadeend())
	{
		if (ctrl1->at(input::UP) == 1 || ctrl1->at(input::DOWN) == 1)
		{
			if (ctrl1->at(input::UP) == 1)
			{
				menu_select = menu_select - 1 < 0 ? menu_select = 2 : menu_select - 1;
			}
			if (ctrl1->at(input::DOWN) == 1)
			{
				menu_select = (menu_select + 1) % 3;
			}
			background_color = util::translater<color>(std::move(*background_color.get_now()), std::move(colors[menu_select]), nextcolor_frames);
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
			case VS_START:
				level_mng->set_next_level(level::VSGAME);
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

	//DrawBox(0, 0, WindowWidth, WindowHeight, background_color.get_now()->get_color(), true);
	DxLib::DrawGraph(0, 0, *resdata[0], true);
	int menu_color[3] = { GetColor(0, 0, 0), GetColor(0, 0, 0), GetColor(0, 0, 0) };
	background_color.next([&]( int frame, int now, color value ) -> color
	{
		int now_r = (int)((now / (double) frame) * value.r);
		int now_g = (int)((now / (double) frame) * value.g);
		int now_b = (int)((now / (double) frame) * value.b);

		if (now_r > colors[menu_select].r)
			now_r = 255 - now_r;
		if (now_g > colors[menu_select].g)
			now_g = 255 - now_g;
		if (now_b > colors[menu_select].b)
			now_b = 255 - now_b;
		return color(now_r, now_g, now_b);
	});
	menu_color[menu_select] = GetColor(255, 0, 0);
	{
		int i = 0;
		for (auto & it : menu)
		{
			DrawFormatStringToHandle(50, WindowHeight - 550 + i * 60, menu_color[i], *resdata[1] ,"%s", it.first.c_str());
			++i;
		}
	}
	return 0;
}

bool Title::end(int msg)
{
	if (level_mng->fadeend())
	{
		for (auto& it : resdata)
			it->Delete();
		this->loading = false;
		end_flag = false;
		return true;
	}
	else
	{
		execute(msg);
		return false;
	}
}