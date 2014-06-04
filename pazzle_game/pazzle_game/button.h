#pragma once
#include<Windows.h>
#include"Position.h"

namespace util
{
	class push_button
	{
		RECT rect_;
		int push_frame = 0;
	public:
		push_button(const RECT& rect) : rect_(rect){}
		bool pushed(const pos<int>& point, bool push);
	};
}