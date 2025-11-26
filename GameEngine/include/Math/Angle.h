#pragma once
#include "AngleInterval.h"
#include "MyMath.h"
#include<ostream>
namespace KT
{
	//Forward Declaration
	template<typename type> requires is_floating_type_v<type>
	class Angle;

	template<typename type> requires is_floating_type_v<type>
	Angle< type> Degree(type degree, AngleInterval<type> interval);

	template<typename type> requires is_floating_type_v<type>
	Angle < type> Radiant(type radiant, AngleInterval<type> interval);

	//class Angle need Interval
	template<typename type> requires is_floating_type_v<type>
	class Angle
	{
	public:
		friend Angle<type> Degree<type>( type, AngleInterval<type>);
		friend Angle<type> Radiant<type>( type, AngleInterval<type>);
		using value_type = type;
		using class_type = Angle<type>;
		Angle(AngleInterval<type> strategy);
		~Angle() = default;
		Angle(const class_type& other);
		Angle(class_type&& other) noexcept;

		value_type AsRadians();
		value_type AsRadians() const;
		value_type AsDegrees();
		value_type AsDegrees() const;

		class_type& operator=(const class_type& other);
		class_type& operator=(class_type&& other) noexcept;

		void SetInterval(AngleInterval<type> strategy);
		AngleInterval<type> GetInterval() const;

		class_type operator+(const class_type& other) const;
		class_type operator-(const class_type& other) const;
		class_type& operator +=(const class_type& other);
		class_type& operator -=(const class_type& other);

		class_type operator*(value_type factor) const;
		class_type operator/(value_type divider) const;
		class_type& operator *=(value_type factor);
		class_type& operator /=(value_type divider);

		bool operator>(const class_type&) const;
		bool operator<(const class_type&) const;
		bool operator>=(const class_type&) const;
		bool operator<=(const class_type&) const;

		bool operator==(const class_type& other) const;
		bool operator!=(const class_type& other) const;
		bool HasSameInterval(const class_type& other) const;

		void Reset();
	private:
		//Can Only be Create by Degree() or Rad()
		Angle(const type& radiant, const AngleInterval<type>& interval);
		type m_radiant;
		AngleInterval<type> m_strategy;
	};

	using AngleF = Angle<float>;
	using AngleD = Angle<double>;


	// Create an Angle with Degree
	template <typename type> requires is_floating_type_v<type>
	Angle<type> Degree( type degree,AngleInterval<type> interval)
	{
		auto rad = degree * (Math::PI_V<type> / static_cast<type>(180));
		return Angle<type>(rad, interval);
	}


	// Create an Angle with Radiant
	template <typename type> requires is_floating_type_v<type>
	Angle<type> Radiant( type radiant,AngleInterval<type> interval)
	{
		return Angle<type>(radiant, interval);
	}

	template <typename type> requires is_floating_type_v<type>
	Angle<type>::Angle(AngleInterval<type> strategy) :m_radiant(type{}), m_strategy(strategy)
	{
	}

	template <typename type> requires is_floating_type_v<type>
	Angle<type>::Angle(const Angle& other) : m_radiant(other.m_radiant), m_strategy(other.m_strategy)
	{
	}

	template <typename type> requires is_floating_type_v<type>
	Angle<type>::Angle(Angle&& other) noexcept : m_radiant(other.m_radiant), m_strategy(std::move(other.m_strategy))
	{
	}

	template <typename type> requires is_floating_type_v<type>
	type Angle<type>::AsRadians()
	{
		return m_radiant;
	}

	template <typename type> requires is_floating_type_v<type>
	type Angle<type>::AsRadians() const
	{
		return m_radiant;
	}

	template <typename type> requires is_floating_type_v<type>
	type Angle<type>::AsDegrees()
	{
		return m_radiant * static_cast<type>(180) / Math::PI_V<type>;
	}

	template <typename type> requires is_floating_type_v<type>
	type Angle<type>::AsDegrees() const
	{
		return m_radiant * static_cast<type>(180) / Math::PI_V<type>;
	}

	template <typename type> requires is_floating_type_v<type>
	Angle<type>& Angle<type>::operator=(const class_type& other)
	{
		m_radiant = other.m_radiant;
		m_strategy = other.m_strategy;
		return *this;
	}

	template <typename type> requires is_floating_type_v<type>
	Angle<type>& Angle<type>::operator=(class_type&& other) noexcept
	{
		m_radiant = other.m_radiant;
		m_strategy = std::move(other.m_strategy);
		return *this;
	}

	template <typename type> requires is_floating_type_v<type>
	void Angle<type>::SetInterval(AngleInterval<type> strategy)
	{
		m_strategy = strategy;
		m_radiant = m_strategy.Normalize(m_radiant);
	}

	template <typename type> requires is_floating_type_v<type>
	AngleInterval<type> Angle<type>::GetInterval() const
	{
		return m_strategy;
	}

	template <typename type> requires is_floating_type_v<type>
	Angle<type> Angle<type>::operator+(const class_type& other) const
	{
		return Angle(m_radiant + m_strategy.Normalize(other.m_radiant), m_strategy);
	}

	template <typename type> requires is_floating_type_v<type>
	Angle<type> Angle<type>::operator-(const class_type& other) const
	{
		return Angle(m_radiant - m_strategy.Normalize(other.m_radiant), m_strategy);
	}

	template <typename type> requires is_floating_type_v<type>
	Angle<type>& Angle<type>::operator+=(const class_type& other)
	{
		m_radiant = m_strategy.Normalize(m_radiant + other.m_radiant);
		return *this;
	}

	template <typename type> requires is_floating_type_v<type>
	Angle<type>& Angle<type>::operator-=(const class_type& other)
	{
		m_radiant = m_strategy.Normalize(m_radiant - other.m_radiant);
		return *this;
	}

	template <typename type> requires is_floating_type_v<type>
	Angle<type> Angle<type>::operator*(value_type factor) const
	{
		return Angle(m_radiant * factor,m_strategy);
	}

	template <typename type> requires is_floating_type_v<type>
	Angle<type> Angle<type>::operator/(value_type divider) const
	{
		if (divider == 0)
			throw std::out_of_range("Imposible to divide by 0");
		return Angle(m_radiant / divider, m_strategy);
	}

	template <typename type> requires is_floating_type_v<type>
	Angle<type>& Angle<type>::operator*=(value_type factor)
	{
		m_radiant = m_strategy.Normalize(m_radiant * factor);
		return *this;
	}

	template <typename type> requires is_floating_type_v<type>
	Angle<type>& Angle<type>::operator/=(value_type divider)
	{
		if (divider == 0)
			throw std::out_of_range("Imposible to divide by 0");
		m_radiant = m_strategy.Normalize(m_radiant / divider);
		return *this;
	}

	template <typename type> requires is_floating_type_v<type>
	bool Angle<type>::operator>(const class_type& other) const
	{
		if (Math::IsSameValue(m_radiant, m_strategy.Normalize(other.m_radiant), Math::EPSILON_V<value_type>))
			return false;
		return m_radiant > m_strategy.Normalize(other.m_radiant);
	}

	template <typename type> requires is_floating_type_v<type>
	bool Angle<type>::operator<(const class_type& other) const
	{
		if (Math::IsSameValue(m_radiant, m_strategy.Normalize(other.m_radiant), Math::EPSILON_V<value_type>))
			return false;
		return m_radiant < m_strategy.Normalize(other.m_radiant);
	}

	template <typename type> requires is_floating_type_v<type>
	bool Angle<type>::operator>=(const class_type& other) const
	{
		return *this > other || *this == other;
	}

	template <typename type> requires is_floating_type_v<type>
	bool Angle<type>::operator<=(const class_type& other) const
	{
		return *this < other || *this == other;
	}

	template <typename type> requires is_floating_type_v<type>
	bool Angle<type>::operator==(const class_type& other) const
	{
		return Math::IsSameValue(m_radiant, m_strategy.Normalize(other.m_radiant), Math::EPSILON_V<value_type>);
	}

	template <typename type> requires is_floating_type_v<type>
	bool Angle<type>::operator!=(const class_type& other) const
	{
		return !(*this == other);
	}

	template <typename type> requires is_floating_type_v<type>
	bool Angle<type>::HasSameInterval(const class_type& other) const
	{
		return m_strategy == other.m_strategy;
	}

	template <typename type> requires is_floating_type_v<type>
	void Angle<type>::Reset()
	{
		m_radiant = m_strategy.Normalize(type{});
	}

	template <typename type> requires is_floating_type_v<type>
	Angle<type>::Angle(const type& radiant, const AngleInterval<type>& interval) : m_radiant(radiant), m_strategy(interval)
	{
		m_radiant = m_strategy.Normalize(radiant);
	}

	template<typename type, typename AngleInterval > requires is_floating_type_v<type>
	class StaticAngle;

	template<typename type, typename AngleInterval > requires is_floating_type_v<type>
	StaticAngle< type, AngleInterval> Degree(type);

	template<typename type, typename AngleInterval > requires is_floating_type_v<type>
	StaticAngle<type, AngleInterval> Radiant(type);

	// Static Angle Version with Compile Time AngleStrategy
	template<typename type, typename AngleInterval > requires is_floating_type_v<type>
	class StaticAngle
	{
	public:
		friend StaticAngle Degree<type, AngleInterval>(type);
		friend StaticAngle Radiant<type, AngleInterval>(type);
		using value_type = type;
		using class_type = StaticAngle<type, AngleInterval>;
		using Interval = StaticAngleInterval<type, AngleInterval::value_min, AngleInterval::value_max>;

		StaticAngle();
		~StaticAngle() = default;
		StaticAngle(const StaticAngle& other);
		StaticAngle(StaticAngle&& other) noexcept;
		value_type AsRadians();
		value_type AsRadians() const;
		value_type AsDegrees();
		value_type AsDegrees() const;

		StaticAngle& operator=(const StaticAngle& other);
		StaticAngle& operator=(StaticAngle&& other) noexcept;

		StaticAngle operator+(const StaticAngle& other) const;
		StaticAngle operator-(const StaticAngle& other) const;
		StaticAngle& operator +=(const StaticAngle& other);
		StaticAngle& operator -=(const StaticAngle& other);

		StaticAngle operator*(value_type factor) const;
		StaticAngle operator/(value_type divider) const;
		StaticAngle& operator *=(value_type factor);
		StaticAngle& operator /=(value_type divider);

		bool operator==(const StaticAngle& other) const;
		bool operator!=(const StaticAngle& other) const;

		bool operator>(const class_type&) const;
		bool operator<(const class_type&) const;
		bool operator>=(const class_type&) const;
		bool operator<=(const class_type&) const;

		StaticAngleInterval<type, AngleInterval::value_min, AngleInterval::value_max> GetInterval() const;

		void Reset();
	private:
		StaticAngle(value_type radiant);
		type m_radiant;
	};
	template< typename AngleInterval >
	using SAngleF = StaticAngle<float, AngleInterval>;
	template< typename AngleInterval >
	using SAngleD = StaticAngle<double, AngleInterval>;


	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	StaticAngle<type, AngleInterval>::StaticAngle() : m_radiant(type{})
	{
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	StaticAngle<type, AngleInterval>::StaticAngle(const StaticAngle& other) : m_radiant(other.m_radiant)
	{
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	StaticAngle<type, AngleInterval>::StaticAngle(StaticAngle&& other) noexcept : m_radiant(other.m_radiant)
	{
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	type StaticAngle<type, AngleInterval>::AsRadians()
	{
		return m_radiant;
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	type StaticAngle<type, AngleInterval>::AsRadians() const
	{
		return m_radiant;
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	type StaticAngle<type, AngleInterval>::AsDegrees()
	{
		return m_radiant * static_cast<type>(180) / Math::PI_V<type>;
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	type StaticAngle<type, AngleInterval>::AsDegrees() const
	{
		return m_radiant * static_cast<type>(180) / Math::PI_V<type>;
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	StaticAngle<type, AngleInterval>& StaticAngle<type, AngleInterval>::operator=(const StaticAngle& other)
	{
		m_radiant = other.m_radiant;
		return *this;
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	StaticAngle<type, AngleInterval>& StaticAngle<type, AngleInterval>::operator=(StaticAngle&& other) noexcept
	{
		m_radiant = other.m_radiant;
		return *this;
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	StaticAngle<type, AngleInterval> StaticAngle<type, AngleInterval>::operator+(const StaticAngle& other) const
	{
		return class_type(m_radiant + other.m_radiant);
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	StaticAngle<type, AngleInterval> StaticAngle<type, AngleInterval>::operator-(const StaticAngle& other) const
	{
		return class_type(m_radiant - other.m_radiant);
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	StaticAngle<type, AngleInterval>& StaticAngle<type, AngleInterval>::operator+=(const StaticAngle& other)
	{
		m_radiant = AngleInterval::Normalize(m_radiant + other.m_radiant);
		return *this;
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	StaticAngle<type, AngleInterval>& StaticAngle<type, AngleInterval>::operator-=(const StaticAngle& other)
	{
		m_radiant = AngleInterval::Normalize(m_radiant - other.m_radiant);
		return *this;
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	StaticAngle<type, AngleInterval> StaticAngle<type, AngleInterval>::operator*(value_type factor) const
	{
		return class_type(m_radiant * factor);
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	StaticAngle<type, AngleInterval> StaticAngle<type, AngleInterval>::operator/(value_type divider) const
	{
		if (divider == 0)
			throw std::out_of_range("Imposible to divide by 0");
		return class_type(m_radiant / divider);
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	StaticAngle<type, AngleInterval>& StaticAngle<type, AngleInterval>::operator*=(value_type factor)
	{
		m_radiant = AngleInterval::Normalize(m_radiant * factor);
		return *this;
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	StaticAngle<type, AngleInterval>& StaticAngle<type, AngleInterval>::operator/=(value_type divider)
	{
		if (divider == 0)
			throw std::out_of_range("Imposible to divide by 0");
		m_radiant = AngleInterval::Normalize(m_radiant / divider);
		return *this;
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	bool StaticAngle<type, AngleInterval>::operator==(const StaticAngle& other) const
	{
		return Math::IsSameValue(m_radiant, other.m_radiant, Math::EPSILON_V<value_type>);
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	bool StaticAngle<type, AngleInterval>::operator!=(const StaticAngle& other) const
	{
		return !Math::IsSameValue(m_radiant, other.m_radiant, Math::EPSILON_V<value_type>);
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	bool StaticAngle<type, AngleInterval>::operator>(const class_type& other) const
	{
		if (Math::IsSameValue(m_radiant, other.m_radiant, Math::EPSILON_V<value_type>))
			return false;
		return m_radiant > other.m_radiant;
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	bool StaticAngle<type, AngleInterval>::operator<(const class_type& other) const
	{
		if (Math::IsSameValue(m_radiant, other.m_radiant, Math::EPSILON_V<value_type>))
			return false;
		return m_radiant < other.m_radiant;
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	bool StaticAngle<type, AngleInterval>::operator>=(const class_type& other) const
	{
		return *this > other || *this == other;
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	bool StaticAngle<type, AngleInterval>::operator<=(const class_type& other) const
	{
		return *this < other || *this == other;
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	StaticAngleInterval<type, AngleInterval::value_min, AngleInterval::value_max> StaticAngle<type, AngleInterval>::
		GetInterval() const
	{
		return Interval{};
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	void StaticAngle<type, AngleInterval>::Reset()
	{
		m_radiant = AngleInterval::Normalize(type{});
	}

	template <typename type, typename AngleInterval> requires is_floating_type_v<type>
	StaticAngle<type, AngleInterval>::StaticAngle(value_type radiant) : m_radiant(radiant)
	{
		m_radiant = AngleInterval::Normalize(m_radiant);
	}

	template<typename type, typename AngleInterval > requires is_floating_type_v<type>
	StaticAngle< type, AngleInterval> Degree(type degree)
	{
		auto rad = degree * (Math::PI_V<type> / static_cast<type>(180));
		return StaticAngle< type, AngleInterval>(rad);
	}

	template<typename type, typename AngleInterval > requires is_floating_type_v<type>
	StaticAngle< type, AngleInterval> Radiant(type radiant)
	{
		return StaticAngle< type, AngleInterval>(radiant);
	}

	template< typename type>
	std::ostream& operator<<(std::ostream& os, Angle <type>& angle)
	{
		os << "Angle: " << angle.AsRadians() << " Rad" << std::endl;
		return os;
	}

}
template< typename type, typename AngleInterval >
std::ostream& operator<<(std::ostream& os, KT::StaticAngle <type, AngleInterval>& angle)
{
	os << "Angle: " << angle.AsRadians() << " Rad" << std::endl;
	return os;
}

template< typename type, typename AngleInterval >
std::ostream& operator<<(std::ostream& os, KT::Angle <type>& angle)
{
	os << "Angle: " << angle.AsRadians() << " Rad" << std::endl;
	return os;
}