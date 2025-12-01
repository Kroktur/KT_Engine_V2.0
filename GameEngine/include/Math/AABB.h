#pragma once
#include "VectorND.h"
#include "Vector2.h"
#include "Vector3.h"

namespace KT
{

	template<typename type, size_t size,template <typename>class VectorType> requires is_Vector<VectorType<type>>
	struct AABB
	{
		using value_type = type;
		using class_type = AABB<type,size,VectorType>;
		using vector_type = VectorType<type>;
		AABB();
		AABB(const vector_type& Amin_, const vector_type& Amax_);
		AABB(const class_type& other);
		AABB(class_type&& other) noexcept;
		AABB(const std::vector<VectorType<type>>& vector);

		class_type& operator=(const class_type& other);
		class_type& operator=(class_type&& other) noexcept;

		bool operator==(const class_type& other) const;
		bool operator!=(const class_type& other) const;

		bool operator>(const class_type& other) const;
		bool operator>=(const class_type& other) const;
		bool operator<(const class_type& other) const;
		bool operator<=(const class_type& other) const;

		vector_type Center() const;
		vector_type Size() const;
		vector_type HalfSize() const;
		class_type& Merge(const class_type& other);
		class_type& Expand(const vector_type& vec);
		type BoundingRadius() const;
		type Volume() const;

		bool Contains(const vector_type& vec) const;
		bool Intersects(const vector_type& vec) const;
		bool Contains(const class_type& other) const;
		bool Intersects(const class_type& other) const;

		vector_type Amin, Amax;
	};

	template<typename type>
	using AABB3D = AABB<type, 3, Vector3>;
	using AABB3DF = AABB3D<float>;
	using AABB3DI = AABB3D<int>;

	template<typename type>
	using AABB2D = AABB<type, 2, Vector2>;
	using AABB2DF = AABB2D<float>;
	using AABB2DI = AABB2D<int>;

	template<typename type, size_t size, template <typename>class VectorType> requires is_Vector<VectorType<type>>
	AABB<type, size,VectorType>::AABB() : Amin(), Amax()
	{

	}

	template<typename type, size_t size, template <typename>class VectorType> requires is_Vector<VectorType<type>>
	AABB<type, size,VectorType>::AABB(const vector_type& Amin_, const vector_type& Amax_) :Amin(Amin_), Amax(Amax_)
	{

	}

	template<typename type, size_t size, template <typename>class VectorType>requires is_Vector<VectorType<type>>
	AABB<type, size,VectorType>::AABB(const class_type& other) : Amin(other.Amin), Amax(other.Amax )
	{

	}

	template<typename type, size_t size, template <typename>class VectorType>requires is_Vector<VectorType<type>>
	AABB<type, size,VectorType>::AABB(class_type&& other) noexcept : Amin(std::move(other.Amin)), Amax(std::move(other.Amax))
	{

	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	AABB<type, size, VectorType>::AABB(const std::vector<VectorType<type>>& vector) : Amin(),Amax()
	{
		if (vector.empty())
			return;

		Amin = vector.front();
		for (auto& elem : vector)
			Expand(elem);
	}

	template<typename type, size_t size, template <typename>class VectorType>requires is_Vector<VectorType<type>>
	bool AABB<type, size,VectorType>::Contains(const vector_type& vec) const
	{
		for (int i = 0; i < vector_type::vector_size_v; ++i)
		{
			if (Amax.At(i) < vec.At(i) || Amin.At(i) > vec.At(i))
				return false;
		}
		return true;
	}

	template<typename type, size_t size, template <typename>class VectorType>requires is_Vector<VectorType<type>>
	bool AABB<type, size,VectorType>::Intersects(const vector_type& vec) const
	{
		for (int i = 0; i < vector_type::vector_size_v; ++i)
		{
			if (Amax.At(i) <= vec.At(i) || Amin.At(i) >= vec.At(i))
				return false;
		}
		return true;
	}

	template<typename type, size_t size, template <typename>class VectorType>requires is_Vector<VectorType<type>>
	bool AABB<type, size,VectorType>::Contains(const class_type& other) const
	{
		for (int i = 0; i < vector_type::vector_size_v; ++i)
		{
			if (Amax.At(i) < other.Amin.At(i) || Amin.At(i) > other.Amax.At(i))
				return false;
		}
		return true;
	}

	template<typename type, size_t size, template <typename>class VectorType>requires is_Vector<VectorType<type>>
	bool AABB<type, size,VectorType>::Intersects(const class_type& other) const
	{
		for (int i = 0; i < vector_type::vector_size_v; ++i)
		{
			if (Amax.At(i) <= other.Amin.At(i) || Amin.At(i) >= other.Amax.At(i))
				return false;
		}
		return true;
	}

	template<typename type, size_t size, template <typename>class VectorType>requires is_Vector<VectorType<type>>
	typename AABB<type, size,VectorType>::class_type& AABB<type, size,VectorType>::operator=(const class_type& other)
	{
		Amin = other.Amin;
		Amax = other.Amax;
		return *this;
	}

	template<typename type, size_t size, template <typename>class VectorType>requires is_Vector<VectorType<type>>
	typename AABB<type, size,VectorType>::class_type& AABB<type, size,VectorType>::operator=(class_type&& other) noexcept
	{
		Amin = std::move(other.Amin);
		Amax = std::move(other.Amax);
		return *this;
	}

	template<typename type, size_t size, template <typename>class VectorType>requires is_Vector<VectorType<type>>
	bool AABB<type, size,VectorType>::operator==(const class_type& other) const
	{
		return  Amin == other.Amin && Amax == other.Amax;
	}

	template<typename type, size_t size, template <typename>class VectorType>requires is_Vector<VectorType<type>>
	bool AABB<type, size,VectorType>::operator!=(const class_type& other) const
	{
		return Amin != other.Amin || Amax != other.Amax;
	}

	template<typename type, size_t size, template <typename>class VectorType>requires is_Vector<VectorType<type>>
	typename AABB<type, size,VectorType>::vector_type AABB<type, size,VectorType>::Center() const
	{
		return  Amin + (Amax - Amin) / 2;
	}

	template<typename type, size_t size, template <typename>class VectorType>requires is_Vector<VectorType<type>>
	typename AABB<type, size,VectorType>::class_type& AABB<type, size,VectorType>::Merge(const class_type& other)
	{
		if (*this == other)
			return *this;
		for (int i = 0 ; i < vector_type::vector_size_v ; ++i)
		{
			Amin.At(i) = Math::Min(Amin.At(i), other.Amin.At(i));
			Amax.At(i) = Math::Max(Amax.At(i), other.Amax.At(i));
		}
		return *this;
	}

	template<typename type, size_t size, template <typename>class VectorType>requires is_Vector<VectorType<type>>
	typename AABB<type, size, VectorType>::class_type& AABB<type, size,VectorType>::Expand(const vector_type& vec)
	{

		for (int i = 0; i < vector_type::vector_size_v; ++i)
		{
			Amin.At(i) = Math::Min(Amin.At(i), vec.At(i));
			Amax.At(i) = Math::Max(Amax.At(i), vec.At(i));
		}
		return *this;
	}

	template<typename type, size_t size, template <typename>class VectorType>requires is_Vector<VectorType<type>>
	typename AABB<type,size,VectorType>::vector_type AABB<type, size,VectorType>::Size() const
	{
		return Amax - Amin;
	}

	template<typename type, size_t size, template <typename>class VectorType>requires is_Vector<VectorType<type>>
	typename AABB<type, size,VectorType>::vector_type AABB<type, size,VectorType>::HalfSize() const
	{
		return Size() / 2;
	}

	template<typename type, size_t size, template <typename>class VectorType>requires is_Vector<VectorType<type>>
	type AABB<type, size,VectorType>::BoundingRadius() const
	{
		return HalfSize().Length();
	}

	template<typename type, size_t size, template <typename>class VectorType>requires is_Vector<VectorType<type>>
	type AABB<type, size,VectorType>::Volume() const
	{
		vector_type vecSize = Size();
		type sizeResult = 1;
		for (int i = 0; i < vector_type::vector_size_v; ++i)
		{
			sizeResult *= vecSize.At(i);
		}
		return sizeResult;
	}

	template<typename type, size_t size, template <typename>class VectorType>requires is_Vector<VectorType<type>>
	bool AABB<type, size,VectorType>::operator>(const class_type& other) const
	{
		return Volume() > other.Volume();
	}

	template<typename type, size_t size, template <typename>class VectorType>requires is_Vector<VectorType<type>>
	bool AABB<type, size,VectorType>::operator>=(const class_type& other) const
	{
		return Volume() >= other.Volume();
	}

	template<typename type, size_t size, template <typename>class VectorType>requires is_Vector<VectorType<type>>
	bool AABB<type, size,VectorType>::operator<(const class_type& other) const
	{
		return Volume() < other.Volume();
	}

	template<typename type, size_t size, template <typename>class VectorType>requires is_Vector<VectorType<type>>
	bool AABB<type, size,VectorType>::operator<=(const class_type& other) const
	{
		return Volume() <= other.Volume();
	}

	



}