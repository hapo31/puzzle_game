#pragma once
#include"Level_Manager.h"
#include"resource.h"

#include"field.h"
#include"draw_system.h"
#include"coursor.h"

#include<array>
#include<memory>

namespace level
{
	class GameMain : public ILevel
	{
		const LEVEL_ID id = GAME_MAIN;
		bool loading = false;
		struct Status
		{
			bool end_flag = false;
			int bright = 0;
			int frames = 0;
			int count_down = 3;
			int cursor_mode = 0;
			double time = 180.0;
		};
		std::shared_ptr<Status> status;
		std::shared_ptr<draw::draw_game> draw_;
		std::shared_ptr<GameObject::field> field_;
		std::shared_ptr<GameObject::cursor> cursor_;

		std::array<res::resource_sh, 30> resdata;
	public:
		GameMain();
		virtual int execute() override;
		virtual int get_myid() const override{ return id; }

		virtual bool init() override;
		virtual bool end() override ;
	};
}