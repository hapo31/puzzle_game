#pragma once
#include<Dx_obj.h>
#include"resource_manager.h"
#include"Controler_Manager.h"

#include"draw_system.h"
#include"coursor.h"
#include"field.h"
#include"random_manager.h"

#include"Level_Manager.h"

#include"Title.h"
#include"GameMain.h"

#include"WindowSize.h"
/*
#include<Effekseer.h>
#include<EffekseerRendererDX9.h>

#ifdef _DEBUG
	#pragma comment(lib, "Effekseer.Debug.lib")
	#pragma comment(lib, "EffekseerRendererDX9.Debug.lib")
#else
	#pragma comment(lib, "Effekseer.Release.lib")
	#pragma comment(lib, "EffekseerRendererDX9.Release.lib")
#endif
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetGraphMode(WindowWidth, WindowHeight, 32);
	SetAlwaysRunFlag(true);
	res::resource::set_async(true);
	Dx_Ready::get("puzzle");
	auto rnd = util::random_manager::getInstance();
	auto cont1 = input::Controler_Manager::get_Instance()->GetControler(DX_INPUT_KEY_PAD1);
	auto levelmng = level::Level_Manager::get_Instance();

	rnd->set_seed(std::random_device()());

	level::Level_Manager::get_Instance()->regist_level(level::TITLE, level::create_level<level::Title>());
	level::Level_Manager::get_Instance()->regist_level(level::GAME_MAIN, level::create_level<level::GameMain>());

	levelmng->set_next_level(level::TITLE);
	
	while (1)
	{
		ClearDrawScreen();

		levelmng->execute();

		input::Controler_Manager::get_Instance()->update();
		ScreenFlip();
		if (ProcessMessage() == -1)
			return 1;
	}
	return 0;
}