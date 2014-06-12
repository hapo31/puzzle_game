#pragma once
#include<Dx_obj.h>
#include"resource_manager.h"

#include"draw_system.h"
#include"coursor.h"
#include"field.h"
#include"random_manager.h"

const int WindowWidth = 1280;
const int WindowHeight = 720;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetGraphMode(WindowWidth, WindowHeight, 32);
	SetAlwaysRunFlag(true);
	SetUseASyncLoadFlag(TRUE);
	res::resource::set_async(true);
	Dx_Ready::get("puzzle");
	auto rnd = util::random_manager::getInstance();
	rnd->set_seed(std::random_device()());

	////////
	GameObject::field field(7, 7);
	GameObject::cursor cursor(&field);
	draw::draw_game g;

	field.initialize();

	char key[256];
	char kfr[256] = {0};
	int mode = 0;
	while (!CheckHitKey(KEY_INPUT_ESCAPE))
	{
		ClearDrawScreen();
		GetHitKeyStateAll(key);
		/* key update */
		{
			int i = 0;
			for (auto& k : key)
			{
				if (k == 1)
					++kfr[i];
				else
					kfr[i] = 0;
				++i;
			}
		}

		if (kfr[KEY_INPUT_SPACE] == 1)
			field.initialize();
		if (kfr[KEY_INPUT_UP] == 1)
		{
			cursor.move(GameObject::cursor::UP);
		}
		if (kfr[KEY_INPUT_DOWN] == 1)
		{
			cursor.move(GameObject::cursor::DOWN);
		}
		if (kfr[KEY_INPUT_LEFT] == 1)
		{
			cursor.move(GameObject::cursor::LEFT);
		}
		if (kfr[KEY_INPUT_RIGHT] == 1)
		{
			cursor.move(GameObject::cursor::RIGHT);
		}
		if (kfr[KEY_INPUT_Z] == 1)
		{
			cursor.rotation(GameObject::cursor::M_DIR::RIGHT);
		}
		if (kfr[KEY_INPUT_X] == 1)
		{
			cursor.swap(GameObject::cursor::M_DIR::RIGHT);
		}
		if (kfr[KEY_INPUT_LSHIFT] == 1)
		{
			mode = (mode + 1) % (GameObject::cursor::MODE::ROTA + 1);
			cursor.set_mode(GameObject::cursor::MODE(mode));
		}


		g.draw_Field(5, 5, field);
		g.draw_coursor(5, 5, cursor);
		g.draw_Flags(5, 5, field);
		g.add_frame();
		field.update();
		ScreenFlip();
		if (ProcessMessage() == -1)
			return 1;
	}

	return 0;
}