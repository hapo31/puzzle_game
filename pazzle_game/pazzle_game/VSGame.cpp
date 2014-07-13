#include"VSGame.h"
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

VSGame::VSGame()
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
	decltype(control->GetControler(0)) ctrl[2] = 
	{ 
		control->GetControler(DX_INPUT_PAD1), 
		control->GetControler(DX_INPUT_PAD2)
	};
	auto level_mng = level::Level_Manager::get_Instance();
}

bool VSGame::init(int)
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

		draw_[0] = std::make_shared<draw_game>();
		draw_[1] = std::make_shared<draw_game>();
		field_[0] = std::make_shared<field>(7, 7);
		field_[1] = std::make_shared<field>(7, 7);
		cursor_[0] = std::make_shared<cursor>(field_[0].get());
		cursor_[1] = std::make_shared<cursor>(field_[1].get());
		status[0] = std::make_shared<Status>();
		status[1] = std::make_shared<Status>();
		field_[0]->initialize();
		field_[1]->initialize();

		for (auto& it : resdata)
		{
			if (it && it->is_Enable())
				it->Load();
		}
		loading = true;
		return false;
	}
	else
	{
		//読み込みが終わっているかどうか
		for (auto& it : resdata)
		{
			if (it)
			{
				if (!it->is_Loaded())
					return false;
			}
			else
			{
				break;
			}
		}

		loading = false;
		level_mng->set_fadein(30);
		return true;
	}
}

int VSGame::execute(int msg)
{
	DrawBox(0, 0, WindowWidth, WindowHeight, GetColor(255, 255, 255), true);
	if (!status[0]->end_flag && CheckHitKey(KEY_INPUT_ESCAPE))
	{
		level_mng->back_level();
		level_mng->set_fadeout(30);
	}
	//ゲームのメイン処理
	if (!status[0]->end_flag && level_mng->fadeend() )
	{
		++status[0]->frames;
		//スタート時の処理
		if (status[0]->frames == 180)
		{
			PlaySoundMem(*resdata[BGM], DX_PLAYTYPE_LOOP);
		}
		//ゲームの処理
		else if (status[0]->frames > 180)
		{

			if (status[0]->end_frames == 0)
			{
				for (int i = 0; i < 2; ++i)
				{
					if (ctrl[i]->at(input::UP) == 1)
					{
						cursor_[i]->move(cursor::M_DIR::UP);
					}
					if (ctrl[i]->at(input::LEFT) == 1)
					{
						cursor_[i]->move(cursor::M_DIR::LEFT);
					}
					if (ctrl[i]->at(input::DOWN) == 1)
					{
						cursor_[i]->move(cursor::M_DIR::DOWN);
					}
					if (ctrl[i]->at(input::RIGHT) == 1)
					{
						cursor_[i]->move(cursor::M_DIR::RIGHT);
					}

					if (ctrl[i]->at(input::A) == 1)
					{
						PlaySoundMem(*resdata[PANEL_CHANGE], DX_PLAYTYPE_BACK);
						//if (status->cursor_mode != cursor::MODE::SOLO)
						cursor_[i]->swap(cursor::M_DIR::RIGHT);
						//else
						//cursor_->rotation(cursor::M_DIR::RIGHT);
					}
					if (ctrl[i]->at(input::B) == 1)
					{
						PlaySoundMem(*resdata[PANEL_CHANGE], DX_PLAYTYPE_BACK);
						//if (status->cursor_mode != cursor::MODE::SOLO)
						cursor_[i]->swap(cursor::M_DIR::LEFT);
						//else
						//cursor_->rotation(cursor::M_DIR::LEFT);
					}
					if (ctrl[i]->at(input::C) == 1)
					{
						status[i]->cursor_mode = (status[i]->cursor_mode + 1) % (cursor::MODE::SWAP2 + 1);
						cursor_[i]->set_mode((cursor::MODE)status[i]->cursor_mode);
					}

					field_[i]->update();
				}
			}
			//終了処理　VSモードでは結果を表示する
			if (status[0]->time > 0.0)
			{
				status[0]->time -= 1 / (double) 60;
			}
			else if (status[0]->end_frames < 300)
			{
				char buf[128];
				auto font = res::Resource_mng::get_Instance()->Get_RegistedResource("MS Gothic2");
				sprintf_s<sizeof buf>(buf, "%d : %d\n", *resdata[FONT_MSG], *font);
				OutputDebugStringA(buf);
				if (field_[0]->get_score() == field_[1]->get_score())
				{
					//ドローゲーム
					DrawStringToHandle(WindowWidth / 2 - 40, WindowHeight / 3, "DRAW GAME...", *font, GetColor(255, 0, 0));
				}
				else if (field_[0]->get_score() > field_[1]->get_score())
				{
					//1P WIN
					DrawStringToHandle(WindowWidth / 2 - 40, WindowHeight / 3, "1P WINS!", *font, GetColor(255, 0, 0) );
					draw_[1]->move_offset(0, 4);
				}
				else
				{
					//2P WIN
					DrawStringToHandle(WindowWidth / 3 - 40, WindowHeight / 3, "2P WINS!", *font, GetColor(255, 0, 0) );
					draw_[0]->move_offset(0, 4);
				}
				++status[0]->end_frames;
			}
			else
			{
				level_mng->back_level();
				level_mng->set_fadeout(30);
			}
		}
	}

	//描画処理
	for (int i = 0; i < 2; ++i)
	{
		draw_[i]->draw_Field(15 + i * WindowWidth / 2, 50, *field_[i]);
#ifdef _DEBUG
		draw_[i]->draw_Flags(15 + i * WindowWidth / 2, 50, *field_[i]);
#endif
		draw_[i]->draw_coursor(15 + i * WindowWidth / 2, 50, *cursor_[i]);
		draw_[i]->add_frame();
	}
	{
		if (status[0]->time < 0) status[0]->time = 0;

		int min = (int) status[0]->time / 60;
		int sec = int(status[0]->time) % 60;
		int t1 = (int) status[0]->time;			//100.55 -> 100
		int t2 = (int) (status[0]->time * 1000);	//100.55 -> 10055
		int mm = t2 - t1 * 1000;				// 10055 - 100*100 -> 55 ?
		DrawFormatStringToHandle(WindowWidth - 580, 0, GetColor(0, 0, 0), *resdata[FONT_MSG], "Time: %02d:%02d.%03d", min, sec, mm);
	}
	//カウントダウン表示
	if (status[0]->frames < 180)
	{
		DrawFormatStringToHandle(WindowWidth / 2 - 7, WindowHeight / 3, GetColor(255, 0, 0), *resdata[FONT_MSG], "%d", status[0]->count_down);
		if (status[0]->frames != 0 && status[0]->frames % 60 == 0)
		{
			--status[0]->count_down;
		}
	}

	return 0;
}

bool VSGame::end(int msg)
{
	status[0]->end_flag = true;
	if (level_mng->fadeend())
	{
		for (auto& it : resdata)
		{
			if (it && it->is_Enable())
				it->Delete();
		}
		loading = false;
		for (int i = 0; i < 2; ++i)
		{
			status[i] = nullptr;
			field_[i] = nullptr;
			cursor_[i] = nullptr;
			draw_[i] = nullptr;
		}
		
		return true;
	}
	else
	{
		execute(msg);
		SetVolumeMusicMem(level_mng->get_bright(), *resdata[BGM]);
		return false;
	}
}