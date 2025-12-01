#pragma once
#include "Tools/PrimaryTypeTrait.h"
#include "MyMath.h"
#include <cmath>
#include <stdexcept>
namespace KT
{
	//Class AngleInterval that represent an interval of angle in radiant
	template<typename type> requires is_floating_type_v<type>
	class AngleInterval
	{
	public:
		using class_type = AngleInterval<type>;
		using value_type = type;
		AngleInterval(value_type min, value_type max);
		AngleInterval();
		AngleInterval(const class_type& other);
		AngleInterval(class_type&& other) noexcept;
		~AngleInterval() = default;
		type GetMin() const;
		type GetMax() const;
		type Normalize(value_type radiant) const;

		class_type& operator=(const class_type& other);
		class_type& operator=(class_type&& other) noexcept;
		bool operator==(const class_type& other) const;
		bool operator!=(const class_type& other) const;
		bool IsInInterval(value_type radiant) const;
	private:
		type m_min;
		type m_max;
	};

	using IntervalF = AngleInterval<float>;
	using IntervalD = AngleInterval<double>;

	template <typename type> requires is_floating_type_v<type>
	AngleInterval<type>::AngleInterval(value_type min, value_type max) : m_min(min), m_max(max)
	{
		if (m_min >= m_max)
			throw std::out_of_range("max must be higher than min");
	}

	template <typename type> requires is_floating_type_v<type>
	AngleInterval<type>::AngleInterval() : m_min(type{}), m_max(type{})
	{
	}

	template <typename type> requires is_floating_type_v<type>
	AngleInterval<type>::AngleInterval(const AngleInterval& other) : m_min(other.m_min), m_max(other.m_max)
	{
		if (m_min >= m_max)
			throw std::out_of_range("max must be higher than min");
	}

	template <typename type> requires is_floating_type_v<type>
	AngleInterval<type>::AngleInterval(AngleInterval&& other) noexcept : m_min(other.m_min), m_max(other.m_max)
	{
		if (m_min >= m_max)
			throw std::out_of_range("max must be higher than min");
	}

	template <typename type> requires is_floating_type_v<type>
	type AngleInterval<type>::GetMin() const
	{
		return m_min;
	}

	template <typename type> requires is_floating_type_v<type>
	type AngleInterval<type>::GetMax() const
	{
		return m_max;
	}

	template <typename type> requires is_floating_type_v<type>
	type AngleInterval<type>::Normalize(value_type radiant) const
	{

		type range = m_max - m_min;
		type value = radiant - m_min;

		value = std::fmod(value, range);
		if (value < 0) value += range;

		return value + m_min;

	}

	template <typename type> requires is_floating_type_v<type>
	AngleInterval<type>& AngleInterval<type>::operator=(const class_type& other)
	{
		m_min = other.m_min;
		m_max = other.m_max;
		return *this;
	}

	template <typename type> requires is_floating_type_v<type>
	AngleInterval<type>& AngleInterval<type>::operator=(class_type&& other) noexcept
	{
		m_min = other.m_min;
		m_max = other.m_max;
		return *this;
	}

	template <typename type> requires is_floating_type_v<type>
	bool AngleInterval<type>::operator==(const class_type& other) const
	{

		return Math::IsSameValue(m_min, other.m_min, Math::EPSILON_V<type>) && Math::IsSameValue(m_max, other.m_max, Math::EPSILON_V<type>);

	}

	template <typename type> requires is_floating_type_v<type>
	bool AngleInterval<type>::operator!=(const class_type& other) const
	{

		return !(*this == other);

	}

	template <typename type> requires is_floating_type_v<type>
	bool AngleInterval<type>::IsInInterval(value_type radiant) const
	{
		if (radiant < m_min || radiant > m_max)
			return false;
		return true;
	}

	// Predefined Angle Intervals
	// 0 to 2*PI
	template<typename type> requires is_floating_type_v<type>
	class UnsignedInterval :public AngleInterval<type>
	{
	public:
		UnsignedInterval();
		~UnsignedInterval() = default;
	};

	using UIntervalF = UnsignedInterval<float>;
	using UIntervalD = UnsignedInterval<double>;

	template <typename type> requires is_floating_type_v<type>
	UnsignedInterval<type>::UnsignedInterval() :AngleInterval<type>(0, 2 * Math::PI_V<type>)
	{
	}
	// -PI to PI
	template<typename type> requires is_floating_type_v<type>
	class SignedInterval :public AngleInterval<type>
	{
	public:
		SignedInterval();
		~SignedInterval() = default;
	};
	template <typename type> requires is_floating_type_v<type>
	SignedInterval<type>::SignedInterval() :AngleInterval<type>(-Math::PI_V<type>, Math::PI_V<type>)
	{
	}

	using SiIntervalF = SignedInterval<float>;
	using SiIntervalD = SignedInterval<double>;

	// Static Angle Interval with Compile Time min and max
	template<typename type, type min, type max> requires is_floating_type_v<type> && (min < max) && (max - min <= 2 * Math::PI_V<type>)
		class StaticAngleInterval
	{
	public:
		using value_type = type;
		static constexpr value_type value_min = min;
		static constexpr value_type value_max = max;
		static value_type GetMin();
		static value_type GetMax();
		static value_type Normalize( value_type radiant);
		static bool IsInInterval( value_type radiant);
	};
	template< float min, float max> requires (min < max) && (max - min <= 2 * Math::PI_V<float>)
	using SIntervalF = StaticAngleInterval<float,min,max>;
	template< double min, double max> requires (min < max) && (max - min <= 2 * Math::PI_V<double>)
	using SIntervalD = StaticAngleInterval<double, min, max>;

	template <typename type, type min, type max> requires is_floating_type_v<type> && (min < max) && (max - min <= 2 * Math
		::PI_V<type>)
		type StaticAngleInterval<type, min, max>::GetMin()
	{
		return min;
	}

	template <typename type, type min, type max> requires is_floating_type_v<type> && (min < max) && (max - min <= 2 * Math
		::PI_V<type>)
		type StaticAngleInterval<type, min, max>::GetMax()
	{
		return max;
	}

	template <typename type, type min, type max> requires is_floating_type_v<type> && (min < max) && (max - min <= 2 * Math
		::PI_V<type>)
		type StaticAngleInterval<type, min, max>::Normalize(value_type radiant)
	{
		type range = max - min;
		type value = radiant - min;

		value = std::fmod(value, range);
		if (value < 0) value += range;

		return value + min;

	}

	template <typename type, type min, type max> requires is_floating_type_v<type> && (min < max) && (max - min <= 2 * Math
		::PI_V<type>)
		bool StaticAngleInterval<type, min, max>::IsInInterval(value_type radiant)
	{
		if (radiant < min || radiant > max)
			return false;
		return true;
	}

	// Predefined Static Angle Intervals
	// 0 to 2*PI
	template<typename type> requires is_floating_type_v<type>
	using StaticUnsignedInterval = StaticAngleInterval<type, static_cast<type>(0), 2 * Math::PI_V<type>>;

	using SUIntervalF = StaticUnsignedInterval<float>;
	using SUIntervalD = StaticUnsignedInterval<double>;
	// -PI to PI
	template<typename type> requires is_floating_type_v<type>
	using StaticSignedInterval = StaticAngleInterval<type, -Math::PI_V<type>, Math::PI_V<type>>;
	using SSiIntervalF = StaticSignedInterval<float>;
	using SSiIntervalD = StaticSignedInterval<double>;

}