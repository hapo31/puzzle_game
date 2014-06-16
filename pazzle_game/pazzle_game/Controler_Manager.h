#pragma once
#include"DxLibControl.h"
#include<array>

namespace input
{
	class Controler_Manager
	{
		Controler_Manager() = default;
		Controler_Manager(const Controler_Manager&) = delete;
		Controler_Manager& operator=(const Controler_Manager&) = delete;

		std::unordered_map<int, DxLibControl> ctrls;

	public:
		static Controler_Manager* get_Instance()
		{
			static Controler_Manager obj;
			return &obj;
		}

		DxLibControl* GetControler(int DX_INPUT_DEFINE)
		{
			//emplaceで要素を生成し、その要素へのポインタを返す
			return &ctrls.emplace(DX_INPUT_DEFINE, DxLibControl(DX_INPUT_DEFINE)).first->second;
		}

		void update()
		{
			for (auto &it : ctrls)
				it.second.update();
		}

	};
}