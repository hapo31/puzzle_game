#pragma once
#include<limits>

namespace util
{


	void f()
	{
		int i = pow<5, 4>::value;
	}

	template<class Dec_Type, int real_width = 20>
	class basic_fixed_point
	{
		Dec_Type data;
	public:
		enum { point_width = std::numeric_limits<Dec_Type>::digits };
		template<class lvalue_Type, class rvalue_Type>
		decltype(*this) operator=()
		{
			return 
		}
		template<class lvalue_Type>
		lvalue_Type operator=() const
		{
			return data /  
		}
	};

	using fixed_point32_t = basic_fixed_point<int>;
	using fixed_point64_t = basic_fixed_point<long long>;
}