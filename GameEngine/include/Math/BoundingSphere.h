#pragma once
#include "VectorND.h"
#include "Vector2.h"
#include "Vector3.h"
namespace KT
{
	template<typename type, size_t size, template <typename>class VectorType> requires is_Vector<VectorType<type>>
	struct BoundingSphere
	{
		using value_type = type;
		using class_type = BoundingSphere<type, size, VectorType>;
		using vector_type = VectorType<type>;

		BoundingSphere() = default;
		BoundingSphere(const vector_type& center_, size_t radius_,size_t outlinePoints = 32);
		BoundingSphere(const class_type& other) = default;
		BoundingSphere(class_type&& other) noexcept = default;
		BoundingSphere(const std::vector<VectorType<type>>& vec, size_t outlinePoints = 32);

		class_type& operator=(const class_type& other) = default;
		class_type& operator=(class_type&& other) noexcept = default;

		bool operator==(const class_type& other) const;
		bool operator!=(const class_type& other) const;

		bool operator>(const class_type& other) const;
		bool operator>=(const class_type& other) const;
		bool operator<(const class_type& other) const;
		bool operator<=(const class_type& other) const;

		vector_type Center() const;
		type Radius() const;
		type Diameter() const;
		class_type& Merge(const class_type& other);
		class_type& Expand(const vector_type& vec);
		type Volume() const;


		bool Contains(const vector_type& vec) const;
		bool Intersects(const vector_type& vec) const;
		bool Contains(const class_type& other) const;
		bool Intersects(const class_type& other) const;

		void SetOutlinePoints(const size_t& count);
		size_t GetOutlinePoints() const;

		vector_type center;
		value_type radius;
	private:
		size_t m_outlinePoints;
	};


	template<typename type>
	using BoundingSphere3D = BoundingSphere<type, 3, Vector3>;
	using BoundingSphere3DF = BoundingSphere3D<float>;
	using BoundingSphere3DD = BoundingSphere3D<double>;

	template<typename type>
	using BoundingSphere2D = BoundingSphere<type, 2, Vector2>;
	using BoundingSphere2DF = BoundingSphere2D<float>;
	using BoundingSphere2DD = BoundingSphere2D<double>;

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	BoundingSphere<type, size, VectorType>::BoundingSphere(const vector_type& center_, size_t radius_,
		size_t outlinePoints) : center(center_),radius(radius_),m_outlinePoints(outlinePoints)
	{

	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	BoundingSphere<type, size, VectorType>::BoundingSphere(const std::vector<VectorType<type>>& vec, size_t outlinePoints) : m_outlinePoints(outlinePoints),center(),radius(type(1))
	{
		if (vec.empty())
			return;
		center = vec.front();
		for (auto& elem : vec)
			Expand(elem);
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	bool BoundingSphere<type, size, VectorType>::operator==(const class_type& other) const
	{
		return center == other.center && radius == other.radius;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	bool BoundingSphere<type, size, VectorType>::operator!=(const class_type& other) const
	{
		return !(*this == other);
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	bool BoundingSphere<type, size, VectorType>::operator>(const class_type& other) const
	{
		return Volume() > other.Volume();
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	bool BoundingSphere<type, size, VectorType>::operator>=(const class_type& other) const
	{
		return  Volume() >= other.Volume();
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	bool BoundingSphere<type, size, VectorType>::operator<(const class_type& other) const
	{
		return Volume() < other.Volume();
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	bool BoundingSphere<type, size, VectorType>::operator<=(const class_type& other) const
	{
		return Volume() <= other.Volume();
	}


	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	typename BoundingSphere<type, size, VectorType>::vector_type BoundingSphere<type, size, VectorType>::Center() const
	{
		return center;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	type BoundingSphere<type, size, VectorType>::Radius() const
	{
		return radius;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	type BoundingSphere<type, size, VectorType>::Diameter() const
	{
		return radius * 2;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	typename BoundingSphere<type, size, VectorType>::class_type& BoundingSphere<type, size, VectorType>::Merge(
		const class_type& other)
	{
		vector_type dir = other.center - center;
		type dist = dir.Length();
		if (radius >= other.radius + dist)
			return *this;
		if (other.radius >= radius + dist)
		{
			*this = other;
			return *this;
		}
		type newRadius = (radius + dist + other.radius) / 2;
		if (dist >0)
		{
			center += dir * ((newRadius - radius) / dist);
		}
		radius = newRadius;

		return *this;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	typename BoundingSphere<type, size, VectorType>::class_type& BoundingSphere<type, size, VectorType>::Expand(
		const vector_type& vec)
	{
		type dist = (vec - center).Length();
		if (dist > radius)
		{
			type shift = (dist - radius) / 2;
			center += (vec - center).Normalize() * shift;
			radius = (radius + dist) / 2;
		}
		return *this;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	type BoundingSphere<type, size, VectorType>::Volume() const
	{
		if constexpr (size== 3)
			return static_cast<type>(4) / static_cast<type>(3) * Math::PI_V<type> *Math::Pow(radius, 3);
		if constexpr (size == 2)
			return  Math::PI_V<type> * Math::Pow(radius, 2);
		throw std::out_of_range("need 2 or 3D");
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	bool BoundingSphere<type, size, VectorType>::Contains(const vector_type& vec) const
	{
		type radiusSquared = radius * radius;
		type distSquared = (vec - center).SquareLength();
		return distSquared < radiusSquared;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	bool BoundingSphere<type, size, VectorType>::Intersects(const vector_type& vec) const
	{
		type radiusSquared = radius * radius;
		type distSquared = (vec - center).SquareLength();
		return distSquared <= radiusSquared;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	bool BoundingSphere<type, size, VectorType>::Contains(const class_type& other) const
	{
		type totalRadius = radius + other.radius;
		type totalRadiusSquared = totalRadius * totalRadius;
		type distSquared = (other.center - center).SquareLength();
		return distSquared < totalRadiusSquared;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	bool BoundingSphere<type, size, VectorType>::Intersects(const class_type& other) const
	{
		type totalRadius = radius + other.radius;
		type totalRadiusSquared = totalRadius * totalRadius;
		type distSquared = (other.center - center).SquareLength();
		return distSquared <= totalRadiusSquared;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	void BoundingSphere<type, size, VectorType>::SetOutlinePoints(const size_t& count)
	{
		if constexpr (size == 2 )
		{
			if (count < 4)
				throw std::out_of_range(" need At Least 4 point");
			m_outlinePoints = count;
		}
		if (count < 16)
			throw std::out_of_range(" need At Least 16 point");
		m_outlinePoints = count;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	size_t BoundingSphere<type, size, VectorType>::GetOutlinePoints() const
	{
		return m_outlinePoints;
	}
}
