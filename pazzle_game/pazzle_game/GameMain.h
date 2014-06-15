#pragma once
#include"Level_Manager.h"

namespace level
{
	class GameMain : public ILevel
	{
		const LEVEL_ID id;
	public:
		GameMain() = default;
		virtual int execute() override{ return 0; };
		virtual int get_myid() const override{ return id; }

		virtual bool init() override{ return true; };
		virtual bool end() override{ return true; };
	};
}