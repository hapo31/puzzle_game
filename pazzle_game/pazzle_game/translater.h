#pragma once

namespace util
{
	template<class Value_Type>
	class translater
	{
		Value_Type now;
		Value_Type start_;
		Value_Type end_;
		int now_time = 0;
		int frame_ = 0;

	public:
		enum { IS_END = INT_MAX };
		using value_type = Value_Type;

		translater(Value_Type&& start, Value_Type&& end, int frame) : start_(std::move(start)), end_(std::move(end)), frame_(frame) {}
		template<class Callable >
		double next(Callable f)
		{
			return  ++now_time <= frame_ ? (now = start_ + f(frame_, now_time, end_)) : IS_END;
		}
		double get_now() const { return now; }

	};
}