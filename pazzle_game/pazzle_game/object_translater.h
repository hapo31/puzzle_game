#pragma once
#include<vector>
#include"Position.h"

namespace util
{
	class object_translater
	{
		int frame = 0;
		util::pos<int> target;
		util::pos<int> now;
	public:
		object_translater() = default;
		template<class TransLate_Engine>
		bool next(TransLate_Engine& engine, pos<int>& object_pos)
		{
			engine();
		}
	};
}