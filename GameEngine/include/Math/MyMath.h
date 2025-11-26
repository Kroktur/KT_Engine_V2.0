#pragma once
#include "Tools/PrimaryTypeTrait.h"

namespace KT
{

namespace Math
{
	//Epsilon accuracy base on type
	//basic Implementation
	template<typename type>
	static constexpr type EPSILON_V = 1e-15L;
	// call default value of EPSILON_V for long double
	static constexpr long double EPSILON_LONG_DOUBLE = EPSILON_V<long double>;

	// double specialization
	template<>
	constexpr double EPSILON_V<double> = 1e-12;
	// constexpr value for double 
	static constexpr double EPSILON_DOUBLE = EPSILON_V<double>;

	// float specialization
	template<>
	constexpr float EPSILON_V<float> = 1e-6f;
	// constexpr value for float 
	static constexpr float EPSILON_FLOAT = EPSILON_V<float>;

	//PI accuracy base on type
	//basic Implementation
	template<typename type>
	static constexpr type PI_V = 3.141592653589793238462643383279L;
	// call default value of PI_V for long double
	static constexpr long double PI_LD = PI_V<long double>;

	// double specialization
	template<>
	constexpr double PI_V<double> = 3.141592653589793238462643383279;
	// constexpr value for double
	static constexpr double PI_D = PI_V<double>;

	// float specialization
	template<>
	constexpr float PI_V<float> = 3.141593f;
	// constexpr value for float
	static constexpr float PI = PI_V<float>;


	template<typename type> requires KT::is_arithmetic_v<type>
	type Pow(const type & value, size_t iteration = 2)
	{
		type result = 1;
		for (size_t i = 0; i < iteration; ++i)
		{
			result *= value;
		}
		return result;
	}

	template<typename type> requires KT::is_integral_type_v<type>
	type Factorial( type value)
	{
		if (value < 0)
			throw;/*std::invalid_argument("Factorial is not defined for negative numbers");*/
		type result = 1;
		while (value >= 1)
		{
			result *= value;
			--value;
		}
		return  result;
	}

	// same as std::abs
	template<typename type> requires  KT::is_arithmetic_v<type>
	type AbsoluteValue(const type & value)
	{
		return (value < static_cast<type>(0)) ? -value : value;
	}

	// verify if the value is equal base on epsilon
	template<typename type> requires KT::is_floating_type_v<type>
	bool IsSameValue(const type& lhs, const type& rhs, const type& EPSILON = EPSILON_V<type>)
	{
		return AbsoluteValue(lhs - rhs) < EPSILON;
	}
	// same but strict verification for integral types 
	template<typename type> requires KT::is_integral_type_v<type>
	bool IsSameValue(const type& lhs, const type& rhs)
	{
		return lhs == rhs;
	}
	// verify if the value is null base on epsilon
	template<typename type> requires KT::is_floating_type_v<type>
	bool IsNull(const type& lhs, const type& EPSILON = EPSILON_V<type>)
	{
		return IsSameValue<type>(lhs, static_cast<type>(0), EPSILON);
	}
	// same but strict verification for integral types 
	template<typename type> requires KT::is_integral_type_v<type>
	bool IsNull(const type& lhs)
	{
		return IsSameValue(lhs, static_cast<type>(0));
	}
	template<typename type> requires KT::is_floating_type_v<type>
	type Lerp(const type& A, const type& B, const type& t)
	{
		if (t < 0 || t > 1)
			throw; /*std::out_of_range("muse be betwen 0 and 1 ");*/
		return (static_cast<type>(1) - t) * A + t * B;
	}
	template<typename type> requires  KT::is_arithmetic_v<type>
	type Min(const type& A, const type& B)
	{
		return A < B ? A : B;
	}

	template<typename type> requires  KT::is_arithmetic_v<type>
	type Max(const type& A, const type& B)
	{
		return A > B ? A : B;
	}
	template<typename type> requires  KT::is_arithmetic_v<type>
	type Clamp(const type& val, const type& min, const type& max)
	{
		type result = val;
		result = result < min ? min : result;
		result = result > max ? max : result;
		return result;
	}
	template<typename type> requires KT::is_floating_type_v<type>
	type Floor(const type& val)
	{
		int truncated = static_cast<int>(val);
		return (val < 0 && truncated != val) ? static_cast<type>(truncated - 1) : static_cast<type>(truncated);
	}
	template<typename type> requires KT::is_floating_type_v<type>
	type Ceil(const type& val)
	{
		int truncated = static_cast<int>(val);
		return (val > 0 && val != truncated) ? static_cast<type>(truncated + 1) : static_cast<type>(truncated);
	}

}

}
