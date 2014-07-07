#pragma once
#include"translater.h"
#include<DxLib.h>

namespace logic
{
	//フェードイン、フェードアウトの管理
	class fade_updater
	{
	public:
		enum type { in, out };
	private:
		using value_type = double;
		util::translater<value_type> updater;
		type ty;
		int frame_ = 10;
	public:
		fade_updater() : updater(255, 255, 1) {};
		explicit fade_updater(type t) : ty(t), updater(t ? 255 : 0, t ? 0 : 255, frame_){}
		fade_updater(type t, int frames) : frame_(frames), ty(t), updater(t ? 255 : 0, t ? 0 : 255, frame_){}
		void set(type t, int frame)
		{
			value_type start = t == out ? 255 : 0;
			value_type end = t == out ? 0 : 255;
			frame_ = frame;
			ty = t;
			updater = util::translater<value_type>(std::move(start), std::move(end), frame);
		}
		void set(type t)
		{
			set(t, frame_);
		}
		bool update();
		bool is_end() { return !updater.get_now();}
		int get_bright() { return updater.get_now() ? (int)*updater.get_now() : -1; }
	};
	typedef enum fade_updater::type fade_type;
}