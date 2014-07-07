#pragma once
#include<boost/optional.hpp>

namespace util
{
	template<class Value_Type>
	class translater
	{
		//現在の値
		boost::optional<Value_Type> now;
		//初期値
		boost::optional<Value_Type> start_;
		//終了値
		boost::optional<Value_Type> end_;
		//現在のフレーム数
		int now_time = 0;
		//動作フレーム数
		int frame_ = 0;

	public:
		using optional_value_type = boost::optional<Value_Type>;
		transelater() now(0), start_(0), end_(0), now_time(0), frame_(1){}
		translater(Value_Type&& start, Value_Type&& end, int frame) : start_(std::move(start)), end_(std::move(end)), frame_(frame) {}
		template<class Callable>
		optional_value_type& next(Callable f)
		{
			if (++now_time <= frame_)
				now = f(frame_, now_time, *end_);
			else
				now = boost::none;
			return now;
		}
		optional_value_type& get_now() { return now; }

	};
}