#pragma once
#include<limits>

namespace util
{
	template<int Base, int Index>
	struct TMP_Pow
	{
		enum { value = Base * TMP_Pow<Base, Index - 1>::value };
	};
	
	template<int Base>
	struct TMP_Pow<Base, 0>
	{
		enum{ value = 1 };
	};

	template<class Dec_Type, int real_width = 16>
	class basic_fixed_point
	{
		Dec_Type num;
	public:
		enum { point_width = std::numeric_limits<Dec_Type>::digits }
		template<class lvalue_Type>
		lvarue_Type operator=() const
		{
			return  num / TMP_Pow<10, point_width>::value;
		}
	};

	using fixed_point32_t = basic_fixed_point<int>;
	using fixed_point64_t = basic_fixed_point<long long>;
}