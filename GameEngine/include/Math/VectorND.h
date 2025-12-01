#pragma once
#include <array>
#include <cmath>
#include <stdexcept>
#include "Tools/PrimaryTypeTrait.h"
#include "Math/MyMath.h"
#include <ostream>

namespace KT
{
	template<typename vecType>
	concept is_Vector = requires(vecType vec)
	{
		{ vec.Length() };
		{ vec.Dot(vec) };
		{ vec.Normalize() };
		vecType::vector_size_v;
	};

	enum class Dir
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		FORWARD,
		BACKWARD
	};

	template<typename type, size_t size> requires is_arithmetic_v<type>
	class VectorND
	{
	public:
		using value_type = type;
		using class_type = VectorND<value_type, size>;
		using iterator_type = typename std::array<type, size>::iterator;
		using const_iterator_type = typename std::array<type, size>::const_iterator;
		static constexpr size_t vector_size_v = size;
		template<typename ...Arg> requires (sizeof...(Arg) <= size) && (... && (KT::is_arithmetic_v<Arg>))
			VectorND(const Arg&... args);
		VectorND();
		VectorND(const class_type& lhs, const class_type& rhs);
		VectorND(const class_type&) = default;
		VectorND(class_type&&)noexcept = default;
		virtual ~VectorND() = default;
		class_type& operator=(const class_type&) = default;
		class_type& operator=(class_type&&) noexcept = default;
		static size_t Size();
		bool IsZero() const;
		bool operator==(const class_type& other) const;
		bool operator!=(const class_type& other) const;
		class_type operator +(const class_type& other) const;
		class_type operator -(const class_type& other) const;
		class_type& operator +=(const class_type& other);
		class_type& operator -=(const class_type& other);
		class_type operator *(const value_type& factor) const;
		class_type operator /(const value_type& divider) const;
		class_type& operator *=(const value_type& factor);
		class_type& operator /=(const value_type& divider);

		// range-based loop
		iterator_type begin();
		iterator_type end();
		const_iterator_type begin() const;
		const_iterator_type end()   const;

		// std::set or std::map
		bool operator<(const class_type& other) const;
		bool operator>(const class_type& other) const;

		value_type& operator[](const size_t& i);
		const value_type& operator[](const size_t& i) const;
		value_type& At(const size_t& index);
		const value_type& At(const size_t& index) const;

		value_type Length() const;
		value_type SquareLength() const;
		class_type Normalize() const;
		class_type& SelfNormalize();
		value_type Dot(const class_type& other) const;
		template<Dir dir> requires (size >= 2)
			static constexpr class_type Dir();
	protected:
		std::array<type, size> m_data;
	};

	template <typename type, size_t size> requires is_arithmetic_v<type>
	template <Dir dir> requires (size >= 2)
		constexpr typename VectorND<type, size>::class_type VectorND<type, size>::Dir()
	{
		class_type result = class_type{};
		if constexpr (dir == Dir::UP)
		{
			result[1] = static_cast<type>(1);
		}
		if constexpr (dir == Dir::DOWN)
		{
			result[1] = static_cast<type>(-1);
		}
		if constexpr (dir == Dir::LEFT)
		{
			result[0] = static_cast<type>(-1);
		}
		if constexpr (dir == Dir::RIGHT)
		{
			result[0] = static_cast<type>(1);
		}
		if constexpr (dir == Dir::FORWARD)
		{
			static_assert(size >= 3, "must be at least 3D vector");
			result[2] = static_cast<type>(1);
		}
		if constexpr (dir == Dir::BACKWARD)
		{
			static_assert(size >= 3, "must be at least 3D vector");
			result[2] = static_cast<type>(-1);
		}
		return result;
	}

	template<size_t size>
	using  VectorF = VectorND<float, size>;
	template< size_t size>
	using  VectorI = VectorND<int, size>;
	template<size_t size>
	using  VectorUI = VectorND<unsigned int, size>;

	template<typename type, size_t size>
	using  Point = VectorND<type, size>;
	template< size_t size>
	using  PointF = Point<float, size>;
	template<size_t size>
	using  PointI = Point<int, size>;
	template< size_t size>
	using  PointUI = Point<unsigned int, size>;



	template <typename type, size_t size> requires is_arithmetic_v<type>
	template <typename ... Arg> requires (sizeof...(Arg) <= size) && (... && (KT::is_arithmetic_v<Arg>))
		VectorND<type, size>::VectorND(const Arg&... args) : m_data(std::array<type, size>{static_cast<type>(args)...})
	{
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	VectorND<type, size>::VectorND()
	{
		std::fill(m_data.begin(), m_data.end(), type{});
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	VectorND<type, size>::VectorND(const class_type& lhs, const class_type& rhs) : m_data(std::array<type, size>{})
	{
		for (size_t i = 0; i < m_data.size(); ++i)
		{
			m_data[i] = rhs[i] - lhs[i];
		}
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	size_t VectorND<type, size>::Size()
	{
		return size;
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	bool VectorND<type, size>::IsZero() const
	{
		for (auto i = 0; i < size; ++i)
		{
			if constexpr (KT::is_floating_type_v<type>)
			{
				if (!Math::IsNull(m_data[i], Math::EPSILON_V<type>))
					return false;
			}
			else {
				if (m_data[i] != static_cast<type>(0))
					return false;
			}
		}
		return true;
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	bool VectorND<type, size>::operator==(const class_type& other) const
	{
		for (auto i = 0; i < size; ++i)
		{
			if constexpr (KT::is_floating_type_v<type>)
			{
				if (!Math::IsSameValue(m_data[i], other[i], Math::EPSILON_V<type>))
					return false;
			}
			else {
				if (m_data[i] != other[i])
					return false;
			}
		}
		return true;
	}
	 
	template <typename type, size_t size> requires is_arithmetic_v<type>
	bool VectorND<type, size>::operator!=(const class_type& other) const
	{
		return !(*this == other);
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	typename VectorND<type, size>::class_type VectorND<type, size>::operator+(const class_type& other) const
	{
		class_type result;
		for (auto i = 0; i < size; ++i)
		{
			result[i] = m_data[i] + other[i];
		}
		return result;
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	typename VectorND<type, size>::class_type VectorND<type, size>::
		operator-(const class_type& other) const
	{
		class_type result;
		for (auto i = 0; i < size; ++i)
		{
			result[i] = m_data[i] - other[i];
		}
		return result;
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	typename VectorND<type, size>::class_type& VectorND<type, size>::operator+=(const class_type& other)
	{
		for (auto i = 0; i < size; ++i)
		{
			m_data[i] += other[i];
		}
		return *this;
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	typename VectorND<type, size>::class_type& VectorND<type, size>::operator-=(const class_type& other)
	{
		for (auto i = 0; i < size; ++i)
		{
			m_data[i] -= other[i];
		}
		return *this;
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	typename VectorND<type, size>::class_type VectorND<type, size>::operator*(const value_type& factor) const
	{
		class_type result;
		for (auto i = 0; i < size; ++i)
		{
			result[i] = m_data[i] * factor;
		}
		return result;
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	typename VectorND<type, size>::class_type VectorND<type, size>::operator/(const value_type& divider) const
	{
		if (divider == 0)
			throw std::out_of_range("impossible to divide by 0");
		class_type result;
		for (auto i = 0; i < size; ++i)
		{
			result[i] = m_data[i] / divider;
		}
		return result;
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	typename VectorND<type, size>::class_type& VectorND<type, size>::operator*=(const value_type& factor)
	{
		for (auto i = 0; i < size; ++i)
		{
			m_data[i] *= factor;
		}
		return *this;
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	typename VectorND<type, size>::class_type& VectorND<type, size>::operator/=(const value_type& divider)
	{
		if (divider == 0)
			throw std::out_of_range("impossible to divide by 0");
		for (auto i = 0; i < size; ++i)
		{
			m_data[i] /= divider;
		}
		return *this;
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	typename VectorND<type, size>::iterator_type VectorND<type, size>::begin()
	{
		return m_data.begin();
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	typename VectorND<type, size>::iterator_type VectorND<type, size>::end()
	{
		return m_data.end();
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	typename VectorND<type, size>::const_iterator_type VectorND<type, size>::begin() const
	{
		return m_data.begin();
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	typename VectorND<type, size>::const_iterator_type VectorND<type, size>::end() const
	{
		return m_data.end();
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	bool VectorND<type, size>::operator<(const class_type& other) const
	{
		for (size_t i = 0; i < size; ++i)
		{
			if (m_data[i] != other.m_data[i]) return m_data[i] < other.m_data[i];
		}
		return false;
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	bool VectorND<type, size>::operator>(const class_type& other) const
	{
		for (size_t i = 0; i < size; ++i)
		{
			if (m_data[i] != other.m_data[i]) return m_data[i] > other.m_data[i];
		}
		return false;
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	typename VectorND<type, size>::value_type VectorND<type, size>::Length() const
	{
		return std::sqrt(SquareLength());
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	typename VectorND<type, size>::value_type VectorND<type, size>::SquareLength() const
	{
		value_type result = value_type{};
		for (auto& it : m_data)
			result += (it * it);
		return  result;
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	typename VectorND<type, size>::class_type VectorND<type, size>::Normalize() const
	{
		if (IsZero())
			throw std::out_of_range("impossible to Normalize null vector");
		auto length = Length();
		return (*this) / length;
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	typename VectorND<type, size>::class_type& VectorND<type, size>::SelfNormalize()
	{
		if (IsZero())
			throw std::out_of_range("impossible to Normalize null vector");
		auto length = Length();
		return (*this) /= length;
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	const typename VectorND<type, size>::value_type& VectorND<type, size>::operator[](const size_t& i) const
	{
		return m_data[i];
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	typename VectorND<type, size>::value_type& VectorND<type, size>::At(const size_t& index)
	{
		if (index >= size)
			throw std::out_of_range("index not in range");
		return m_data.at(index);
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	const typename VectorND<type, size>::value_type& VectorND<type, size>::At(const size_t& index) const
	{
		if (index >= size)
			throw std::out_of_range("index not in range");
		return m_data.at(index);
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	typename VectorND<type, size>::value_type& VectorND<type, size>::operator[](const size_t& i)
	{
		return m_data[i];
	}

	template <typename type, size_t size> requires is_arithmetic_v<type>
	typename VectorND<type, size>::value_type VectorND<type, size>::Dot(const class_type& other) const
	{
		value_type result = type{};
		for (size_t i = 0; i < m_data.size(); ++i)
		{
			result += m_data[i] * other[i];
		}
		return result;
	}
	template<typename type, size_t size>
	std::ostream& operator<<(std::ostream& os, const KT::VectorND<type, size>& vec)
	{
		for (size_t i = 0; i < size; ++i)
		{
			os << vec[i] << " ";
		}
		os << "\n";
		return os;
	}

}
