#pragma once
#include "VectorND.h"

namespace KT
{

template<typename type> requires is_arithmetic_v<type>
class Vector3 : public VectorND<type, 3>
{
public:
	using value_type = type;
	using class_type = Vector3<type>;
	using parent_type = VectorND<type, 3>;
	type& x;
	type& y;
	type& z;
	Vector3();
	Vector3(const value_type& x_, const value_type& y_, const value_type& z_);
	Vector3(const class_type& other) ;
	Vector3(const class_type& lhs, const class_type& rhs);
	Vector3(class_type&& other) noexcept;
	Vector3(const parent_type& other);
	Vector3(parent_type&& other) noexcept;
	class_type& operator=(const class_type&);
	class_type& operator=(class_type&&) noexcept;
	class_type Cross(const class_type&) const;
};

template<typename type>
using  Point3 = Vector3<type>;
using  Point3F = Point3<float>;
using  Point3I = Point3<int>;
using  Point3UI = Point3<unsigned int>;


using  Vector3F = Vector3<float>;
using  Vector3I = Vector3<int>;
using  Vector3UI = Vector3<unsigned int>;

template <typename type> requires is_arithmetic_v<type>
Vector3<type>::Vector3() : parent_type(), x(this->m_data[0]), y(this->m_data[1]), z(this->m_data[2])
{

}

template <typename type> requires is_arithmetic_v<type>
Vector3<type>::Vector3(const value_type& x_, const value_type& y_, const value_type& z_) : parent_type(x_, y_, z_), x(this->m_data[0]), y(this->m_data[1]), z(this->m_data[2])
{
}

template <typename type> requires is_arithmetic_v<type>
Vector3<type>::Vector3(const class_type& other) : parent_type(other), x(this->m_data[0]), y(this->m_data[1]), z(this->m_data[2])
{
	
}

template <typename type> requires is_arithmetic_v<type>
Vector3<type>::Vector3(const class_type& lhs, const class_type& rhs) : parent_type(lhs,rhs), x(this->m_data[0]), y(this->m_data[1]), z(this->m_data[2])
{

}

template <typename type> requires is_arithmetic_v<type>
Vector3<type>::Vector3(class_type&& other) noexcept : parent_type(std::move(other)), x(this->m_data[0]), y(this->m_data[1]), z(this->m_data[2])
{

}

template <typename type> requires is_arithmetic_v<type>
Vector3<type>::Vector3(const parent_type& other) : parent_type(other), x(this->m_data[0]), y(this->m_data[1]), z(this->m_data[2])
{

}

template <typename type> requires is_arithmetic_v<type>
Vector3<type>::Vector3(parent_type&& other) noexcept  : parent_type(std::move(other)), x(this->m_data[0]), y(this->m_data[1]), z(this->m_data[2])
{
}

template <typename type> requires is_arithmetic_v<type>
typename Vector3<type>::class_type& Vector3<type>::operator=(const class_type& other)
{
	parent_type::operator=(other);
	return *this;
}

template <typename type> requires is_arithmetic_v<type>
typename Vector3<type>::class_type& Vector3<type>::operator=(class_type&& other) noexcept
{
	parent_type::operator=(std::move(other));
	return *this;
}


template <typename type> requires is_arithmetic_v<type>
typename Vector3<type>::class_type Vector3<type>::Cross(const class_type& other) const
{
	type resultX = y * other.z - z * other.y;
	type resultY = z * other.x - x * other.z;
	type resultZ = x * other.y - y * other.x;
	return class_type{ resultX, resultY, resultZ };
}

}