#include"GameMain.h"
#include"resource_manager.h"
#include"resource_loader.h"
#include"WindowSize.h"
#include"Controler_Manager.h"
#include"Level_Manager.h"

#include<DxLib.h>

using namespace level;
using namespace res;

using namespace GameObject;
using namespace draw;

GameMain::GameMain()
{}

enum RESID
{
	BGM,
	PANEL_CHANGE,
	PANEL_DESTROY,
	GR_BLOCK,
	GR_WALL,
	GR_FIELD,
	FONT_MSG,

};

namespace
{
	auto control = input::Controler_Manager::get_Instance();
	auto ctrl1 = control->GetControler(DX_INPUT_KEY_PAD1);
	auto level_mng = level::Level_Manager::get_Instance();
}

bool GameMain::init(int ) 
{
	SetDrawBright(0, 0, 0);
	if (!loading)
	{
		auto gr_loader = get_loader<graphic_controler>();
		auto snd_loader = get_loader<sound_controler>();
		auto font_loader = get_loader<font_controler>();
		auto mng = res::Resource_mng::get_Instance();

		font_loader->set_fontinfo("MS Gothic", 40, 3, DX_FONTTYPE_ANTIALIASING_8X8);

		resdata[BGM] = mng->Regist("data/Play01.ogg", snd_loader);
		resdata[PANEL_CHANGE] = mng->Regist("data/panel_change01.ogg", snd_loader);
		resdata[PANEL_DESTROY] = mng->Regist("data/block_destroy.ogg", snd_loader);
		resdata[FONT_MSG] = mng->Regist("MS Gothic2", font_loader);
		resdata[GR_WALL] = mng->Regist("data/wall.png", gr_loader);
		resdata[GR_BLOCK] = mng->Regist("data/wall.png", gr_loader);
		resdata[GR_FIELD] = mng->Regist("data/field.png", gr_loader);

		draw_ = std::make_shared<draw_game>();
		field_ = std::make_shared<field>(7, 7);
		cursor_ = std::make_shared<cursor>(field_.get());
		status = std::make_shared<Status>();
		field_->initialize();

		for (auto& it : resdata)
		{
			if (it && it->is_Enable())
				it->Load();
		}

		loading = true;
		return false;
	}
	else if (resdata[BGM]->is_Loaded() ||
			resdata[PANEL_CHANGE]->is_Loaded() ||
			resdata[PANEL_DESTROY]->is_Loaded() ||
			resdata[FONT_MSG]->is_Loaded())
	{
		loading = false;
		level_mng->set_fadein(30);
		return true;
	}
	else
	{
		return false;
	}
}

int GameMain::execute(int msg)
{
	if (!status->end_flag && CheckHitKey(KEY_INPUT_ESCAPE))
	{
		level_mng->back_level();
		level_mng->set_fadeout(30);
	}
	//ゲームのメイン処理
	if (!status->end_flag && level_mng->fadeend())
	{
		++status->frames;
		//カウントダウン表示
		if (status->frames < 150)
		{

			DrawFormatStringToHandle(WindowWidth / 3, WindowHeight / 3, GetColor(0, 255, 0), *resdata[FONT_MSG], "%d", status->count_down);
			DrawFormatString(WindowWidth / 3, WindowHeight / 3, GetColor(0, 255, 0), "%d", status->count_down);
			if (status->frames % 60 == 0)
			{
				--status->count_down;
			}
		}
		//スタート時の処理
		else if (status->frames == 150)
		{
			PlaySoundMem(*resdata[BGM], DX_PLAYTYPE_LOOP);
		}
		else
		{
			//ゲームの処理
			if (ctrl1->at(input::UP) == 1)
			{
				cursor_->move(cursor::M_DIR::UP);
			}
			if (ctrl1->at(input::LEFT) == 1)
			{
				cursor_->move(cursor::M_DIR::LEFT);
			}
			if (ctrl1->at(input::DOWN) == 1)
			{
				cursor_->move(cursor::M_DIR::DOWN);
			}
			if (ctrl1->at(input::RIGHT) == 1)
			{
				cursor_->move(cursor::M_DIR::RIGHT);
			}

			if (ctrl1->at(input::A) == 1)
			{
				PlaySoundMem(*resdata[PANEL_CHANGE], DX_PLAYTYPE_BACK);
				//if (status->cursor_mode != cursor::MODE::SOLO)
					cursor_->swap(cursor::M_DIR::RIGHT);
				//else
					//cursor_->rotation(cursor::M_DIR::RIGHT);
			}
			if (ctrl1->at(input::B) == 1)
			{
				PlaySoundMem(*resdata[PANEL_CHANGE], DX_PLAYTYPE_BACK);
				//if (status->cursor_mode != cursor::MODE::SOLO)
					cursor_->swap(cursor::M_DIR::LEFT);
				//else
					//cursor_->rotation(cursor::M_DIR::LEFT);
			}
			if (ctrl1->at(input::C) == 1)
			{
				status->cursor_mode = (status->cursor_mode + 1) % (cursor::MODE::SWAP2 + 1);
				cursor_->set_mode((cursor::MODE)status->cursor_mode);
			}

			field_->update();
			if (status->time > 0.0)
			{
				status->time -= 1 / (double)60;
			}
			else
			{
				level_mng->back_level();
				level_mng->set_fadeout(30);
			}
		}
	}
	DrawBox(0, 0, WindowWidth, WindowHeight, GetColor(255, 255, 255), true);
	//描画処理
	draw_->draw_Field(50, 50, *field_);
	draw_->draw_Flags(50, 50, *field_);
	draw_->draw_coursor(50, 50, *cursor_);
	{
		int min = (int) status->time / 60;
		int sec = int(status->time) % 60;
		int t1 = (int)status->time;			//100.55 -> 100
		int t2 = (int)(status->time * 1000);	//100.55 -> 10055
		int mm = t2 - t1 * 1000;				// 10055 - 100*100 -> 55 ?
		DrawFormatStringToHandle(WindowWidth  - 580, 0, GetColor(0, 0, 0), *resdata[FONT_MSG], "Time: %02d:%02d.%03d", min, sec, mm);
	}
	draw_->add_frame();
	return 0;
}

bool GameMain::end(int msg)
{
	status->end_flag = true;
	if (level_mng->fadeend())
	{
		for (auto& it : resdata)
		{
			if (it && it->is_Enable())
				it->Delete();
		}
		loading = false;
		status = nullptr;
		draw_ = nullptr;
		field_ = nullptr;
		cursor_ = nullptr;
		return true;
	}
	else
	{
		execute(msg);
		SetVolumeMusicMem(level_mng->get_bright(), *resdata[BGM]);
		return false;
	}
}