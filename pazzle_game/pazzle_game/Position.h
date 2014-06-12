#pragma once

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

	/*
	template<>
	struct pos<int>
	{
	private:
		int pos_data = 0;
	public:
		short &x, &y;
		pos() : x(*((short*) (&pos_data) + 0)), y(*((short*) (&pos_data) + 1)) {}
		pos(int x_, int y_) : x(*((short*) (&pos_data) + 0)), y(*((short*) (&pos_data) + 1)) { x = x_; y = y_; }
		bool operator==(const pos& rvl) const { return pos_data == rvl.pos_data; }
	
	};
	*/
}