#pragma once
#include <Eigen/Dense>
#include "VectorND.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Geometry.h"
#include "Matrix.h"
#include <complex>

namespace KT
{
	template<typename type, size_t size, template <typename>class VectorType>requires is_Vector<VectorType<type>>
		struct OBB
	{
		using value_type = type;
		using class_type = OBB<type, size, VectorType>;
		using vector_type = VectorType<type>;

		OBB() = default;
		OBB(const vector_type& center,const std::array<vector_type, size>& axes,const vector_type& halfSize);
		OBB(const std::vector<vector_type>& pts);
		OBB(const OBB&) = default;
		OBB(OBB&&) noexcept = default;
		std::vector<vector_type> GetPts() const;

		class_type& operator=(const class_type& other);
		class_type& operator=(class_type&& other) noexcept;

		bool operator==(const class_type& other) const;
		bool operator!=(const class_type& other) const;

		bool operator>(const class_type& other) const;
		bool operator>=(const class_type& other) const;
		bool operator<(const class_type& other) const;
		bool operator<=(const class_type& other) const;

		bool IsDirty() const;
		vector_type Center() const;
		vector_type Size() const;
		vector_type HalfSize() const;
		class_type& Merge(const class_type& other);
		class_type& Expand(const vector_type& vec);
		type BoundingRadius() const;
		type Volume() const;
		
		vector_type center;
		std::array<vector_type, size> axes;
		vector_type halfSize;
	private:
		static std::vector<vector_type> GetQi(const std::vector<vector_type>& pts,const vector_type& center);
		static Matrix <type,size> GetCovMatrix(const std::vector<vector_type>& Qipts);
		static void SetData(const std::vector<vector_type>& pts, vector_type& center, std::array<vector_type, size>& axes, vector_type& halfSize);
		static void SetAxes(const Matrix <type, size>& matrix, std::array<vector_type, size>& axes);
		static void SetHalfSizeAndCenter(const std::vector<vector_type>& pts, std::array<vector_type, size>& axes,vector_type& halfSize,vector_type& center);
		mutable bool m_dirty = true;
		mutable std::vector<VectorType<type>> m_pts;
	};


		template<typename type>
		using OBB3D = OBB<type, 3, Vector3>;
		using OBB3DF = OBB3D<float>;
		using OBB3DD = OBB3D<double>;

		template<typename type>
		using OBB2D = OBB<type, 2, Vector2>;
		using OBB2DF = OBB2D<float>;
		using OBB2DD = OBB2D<double>;

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	std::vector<typename OBB<type, size, VectorType>::vector_type> OBB<type, size, VectorType>::GetQi(
		const std::vector<vector_type>& pts, const vector_type& center)
	{
		std::vector<vector_type> result;
		for (auto& elem : pts)
		{
			result.push_back(elem - center);
		}
		return result;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	Matrix<type, size> OBB<type, size, VectorType>::GetCovMatrix(const std::vector<vector_type>& Qipts)
	{
		type factor = 1 / static_cast<type>(Qipts.size());
		Matrix <type, size> result;
		for (vector_type elem : Qipts)
		{
			Matrix<type, size, 1> Qi;
			for (int i = 0; i < size; ++i)
			{
				Qi.At(i) = elem.At(i);
			}
			result += Qi.MatrixProduct(Qi.Transposition());
		}
		return result * factor;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	void OBB<type, size, VectorType>::SetAxes(const Matrix<type, size>& matrix,
		std::array<vector_type, size>& axes)
	{
		Eigen::Matrix<type,size,size> mat;
		for (size_t i = 0; i < size; ++i)
		{
			for (size_t j = 0; j < size; ++j)
			{
				mat(i, j) = matrix.GetCell(i,j);
			}
		}
		Eigen::EigenSolver<Eigen::Matrix<type, size, size>> solver(mat);
		Eigen::Vector<type,size> eigenvalues = solver.eigenvalues().real();
		Eigen::Matrix<type, size, size> eigenvectors = solver.eigenvectors().real();
		for (int i = 0; i < size; ++i)
		{
			Eigen::Vector<type, size> axis = eigenvectors.col(i);
			for (int j = 0; j < size; ++j)
			{
				axes[i].At(j) = axis[j];
			}
			axes[i].SelfNormalize();
		}
	}

	template <typename type, size_t size, template <typename> class VectorType>requires is_Vector<VectorType<type>>
	void OBB<type, size, VectorType>::SetHalfSizeAndCenter(const std::vector<vector_type>& pts,
		std::array<vector_type, size>& axes, vector_type& halfSize, vector_type& center)
	{
		std::array<type, size> minProj;
		std::array<type, size> maxProj;

		// Initialisation min/max
		for (int i = 0; i < size; ++i)
		{
			minProj[i] = std::numeric_limits<type>::max();
			maxProj[i] = std::numeric_limits<type>::lowest();
		}

		for (const auto& p : pts)
		{
			for (int i = 0; i < size; ++i)
			{
				type proj = p.Dot(axes[i]);
				if (proj < minProj[i]) minProj[i] = proj;
				if (proj > maxProj[i]) maxProj[i] = proj;
			}
		}
		// true center
		center = vector_type{};

		for (int i = 0; i < size; ++i)
		{
			type mid = (minProj[i] + maxProj[i]) / 2;
			type extent = (maxProj[i] - minProj[i]) / 2;

			halfSize.At(i) = extent;
			center += axes[i] * mid;
		}
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	OBB<type, size, VectorType>::OBB(const vector_type& center_, const std::array<vector_type, size>& axes_,
		const vector_type& halfSize_): center(center_),axes(axes_),halfSize(halfSize_)
	{}

	template <typename type, size_t size, template <typename> class VectorType>requires is_Vector<VectorType<type>>
	OBB<type, size, VectorType>::OBB(const std::vector<vector_type>& pts): center(),axes(),halfSize()
	{
		OBB::SetData(pts, center, axes, halfSize);
	}

	template <typename type, size_t size, template <typename> class VectorType>requires is_Vector<VectorType<type>>
	std::vector<typename OBB<type, size, VectorType>::vector_type> OBB<type, size, VectorType>::GetPts() const
	{
		if (!m_dirty)
			return m_pts;

		std::vector<vector_type> result;

		// get the number of point
		size_t numPoints = Math::Pow(static_cast <size_t>(2),size);

		for (size_t i = 0; i < numPoints; ++i)
		{
			VectorType point = center;
			for (size_t dim = 0; dim < size; ++dim)
			{
				// check with bit is 0 or 1 for sign
				int sign = (i & (1 << dim)) ? 1 : -1;
				// move point along axes 
				point += axes[dim] * (halfSize.At(dim) * static_cast<type>(sign));
			}
			result.push_back(point);
		}
		m_dirty = false;
		m_pts = result;

		return m_pts;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	typename OBB<type, size, VectorType>::class_type& OBB<type, size, VectorType>::operator=(const class_type& other)
	{
		m_dirty = other.m_dirty;
		m_pts = other.m_pts;
		axes = other.axes;
		center = other.center;
		halfSize = other.halfSize;
		return *this;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	typename OBB<type, size, VectorType>::class_type& OBB<type, size, VectorType>::operator=(
		class_type&& other) noexcept
	{
		m_dirty = other.m_dirty;
		m_pts = std::move(other.m_pts);
		axes = std::move(other.axes);
		center = std::move(other.center);
		halfSize = std::move( other.halfSize);
		return *this;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	bool OBB<type, size, VectorType>::operator==(const class_type& other) const
	{
		return center == other.center && axes == other.axes && halfSize == other.halfSize;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	bool OBB<type, size, VectorType>::operator!=(const class_type& other) const
	{
		return !(*this == other);
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	bool OBB<type, size, VectorType>::operator>(const class_type& other) const
	{
		return Volume() > other.Volume();
	}

	template <typename type, size_t size, template <typename> class VectorType>requires is_Vector<VectorType<type>>
	bool OBB<type, size, VectorType>::operator>=(const class_type& other) const
	{
		return Volume() >= other.Volume();
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	bool OBB<type, size, VectorType>::operator<(const class_type& other) const
	{
		return Volume() < other.Volume();
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	bool OBB<type, size, VectorType>::operator<=(const class_type& other) const
	{
		return Volume() <= other.Volume();
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	bool OBB<type, size, VectorType>::IsDirty() const
	{
		return m_dirty;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	typename OBB<type, size, VectorType>::vector_type OBB<type, size, VectorType>::Center() const
	{
		return center;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	typename OBB<type, size, VectorType>::vector_type OBB<type, size, VectorType>::Size() const
	{
		return halfSize * static_cast<type>(2);
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	typename OBB<type, size, VectorType>::vector_type OBB<type, size, VectorType>::HalfSize() const
	{
		return halfSize;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	typename OBB<type, size, VectorType>::class_type& OBB<type, size, VectorType>::Merge(const class_type& other)
	{
		m_dirty = true;
		std::vector<vector_type> pts = GetPts();
		std::vector<vector_type> otherPts = other.GetPts();
		pts.insert(pts.end(), otherPts.begin(),otherPts.end());
		OBB::SetData(pts, center, axes, halfSize);
		return *this;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	typename OBB<type, size, VectorType>::class_type& OBB<type, size, VectorType>::Expand(const vector_type& vec)
	{
		m_dirty = true;
		std::vector<vector_type> pts = GetPts();
		pts.push_back(vec);
		OBB::SetData(pts, center, axes, halfSize);
		return *this;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	type OBB<type, size, VectorType>::BoundingRadius() const
	{
		return HalfSize().Length();
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	type OBB<type, size, VectorType>::Volume() const
	{
			vector_type vecSize = Size();
			type sizeResult = 1;
			for (int i = 0; i < vector_type::vector_size_v; ++i)
			{
				sizeResult *= vecSize.At(i);
			}
			return sizeResult;
	}

	template <typename type, size_t size, template <typename> class VectorType>requires is_Vector<VectorType<type>>
	void OBB<type, size, VectorType>::SetData(const std::vector<vector_type>& pts, vector_type& center,
		std::array<vector_type, size>& axes, vector_type& halfSize)
	{
		center = KT::GetCenter(pts);
		auto Qiptr = OBB::GetQi(pts, center);
		auto CovMatrix = OBB::GetCovMatrix(Qiptr);
		OBB::SetAxes(CovMatrix, axes);
		OBB::SetHalfSizeAndCenter(pts, axes, halfSize,center);
	}
}
