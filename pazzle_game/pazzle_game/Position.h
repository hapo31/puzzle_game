#pragma once
#include<initializer_list>

namespace util
{
	template <class T>
	struct pos
	{
		T x, y;
		pos() : x(0), y(0){}
		pos(T x_, T y_) : x(x_), y(y_){}
		//pos(std::initializer_list<T> init) : x(*init.begin()), y(*(init.begin() + 1)){}
		bool operator==(const pos& rvl) const { return (this->x == rvl.x && this->y == rvl.y); }
	};
}