#include"fade_manager.h"

using namespace logic;

bool fade_updater::update()
{
	//線形
	if (updater.next([&](int frame, int now, value_type end) -> value_type
	{
		auto a =  ( (ty == in ? now : frame_ - now) / (double) frame) * 255;
		return a;
	}) == boost::none)
	{
		//フェードイン、アウトが終わったらtrue
		return true;
	}
	else
	{
		//終わっていなければ更新
		double n = updater.get_now().get();
		SetDrawBright((int) n, (int) n, (int) n);
		return false;
	}
}