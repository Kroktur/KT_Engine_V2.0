#pragma once
#include "VectorND.h"
namespace KT
{
	template<typename type> requires is_arithmetic_v<type>
	class Vector2 : public VectorND<type, 2>
	{
	public:
	
		using value_type = type;
		using class_type = Vector2<type>;
		using parent_type = VectorND<type, 2>;
		type& x;
		type& y;

		Vector2();
		Vector2(const value_type& x_, const value_type& y_);
		Vector2(const class_type& other);
		Vector2(class_type&& other) noexcept;
		Vector2(const class_type& lhs, const class_type& rhs);
		Vector2(const parent_type& other);
		Vector2(parent_type&& other) noexcept;
		class_type& operator=(const class_type&);
		class_type& operator=(class_type&&) noexcept;
		class_type NormalVector() const;
		type Cross(const class_type& other) const;
	};

	template<typename type>
	using  Point2 = Vector2<type>;
	using  Point2F = Point2<float>;
	using  Point2I = Point2<int>;
	using  Point2UI = Point2<unsigned int>;


	using  Vector2F = Vector2<float>;
	using  Vector2I = Vector2<int>;
	using  Vector2UI = Vector2<unsigned int>;

	template <typename type> requires is_arithmetic_v<type>
	Vector2<type>::Vector2() : parent_type(), x(this->m_data[0]), y(this->m_data[1])
	{
	}

	template <typename type> requires is_arithmetic_v<type>
	Vector2<type>::Vector2(const value_type& x_, const value_type& y_) : parent_type(x_, y_), x(this->m_data[0]), y(this->m_data[1])
	{
	}

	template <typename type> requires is_arithmetic_v<type>
	Vector2<type>::Vector2(const class_type& other) : parent_type(other), x(this->m_data[0]), y(this->m_data[1])
	{
	}

	template <typename type> requires is_arithmetic_v<type>
	Vector2<type>::Vector2(class_type&& other) noexcept :parent_type(std::move(other)), x(this->m_data[0]), y(this->m_data[1])
	{
	}

	template <typename type> requires is_arithmetic_v<type>
	Vector2<type>::Vector2(const class_type& lhs, const class_type& rhs) : parent_type(lhs, rhs), x(this->m_data[0]), y(this->m_data[1])
	{

	}

	template <typename type> requires is_arithmetic_v<type>
	Vector2<type>::Vector2(const parent_type& other) : parent_type(other), x(this->m_data[0]), y(this->m_data[1])
	{

	}

	template <typename type> requires is_arithmetic_v<type>
	Vector2<type>::Vector2(parent_type&& other) noexcept : parent_type(std::move(other)), x(this->m_data[0]), y(this->m_data[1])
	{
	}

	template <typename type> requires is_arithmetic_v<type>
	typename Vector2<type>::class_type& Vector2<type>::operator=(const class_type& other)
	{
		parent_type::operator=(other);
		return *this;
	}

	template <typename type> requires is_arithmetic_v<type>
	typename Vector2<type>::class_type& Vector2<type>::operator=(class_type&& other) noexcept
	{

		parent_type::operator=(std::move(other));
		return *this;
	}

	template <typename type> requires is_arithmetic_v<type>
	typename Vector2<type>::class_type Vector2<type>::NormalVector() const
	{
		return class_type(-y, x);
	}

	template <typename type> requires is_arithmetic_v<type>
	type Vector2<type>::Cross(const class_type& other) const
	{
		return x * other.y - y * other.x;
	}

}