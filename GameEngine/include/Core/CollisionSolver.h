#pragma once
#include "Math/AABB.h"
#include "Math/BoundingSphere.h"
#include "Math/VectorND.h"

namespace KT
{
	template<typename type, size_t size, template <typename>class VectorType> requires is_Vector<VectorType<type>>
	struct SolverResult
	{
		using vector_type = VectorType<type>;
		bool isColliding = false;
		vector_type normal = vector_type{};
		type depth = type{};
		vector_type mvt = vector_type{};
	};

	struct CollisionSolver
	{
		template<typename type, size_t size, template <typename>class VectorType> requires is_Vector<VectorType<type>>
		using value_type = SolverResult < type, size, VectorType>;
		template<typename type, size_t size, template <typename>class VectorType> requires is_Vector<VectorType<type>>
		static  value_type<type,size,VectorType> SolveAABB(const AABB<type,size,VectorType>& lhs, const AABB<type, size, VectorType>& rhs);
		template<typename type, size_t size, template <typename>class VectorType> requires is_Vector<VectorType<type>>
		static  value_type<type,size,VectorType> SolveSphere(const BoundingSphere<type,size,VectorType>& lhs, const BoundingSphere<type, size, VectorType>& rhs);
		template<typename type, size_t size, template <typename>class VectorType> requires is_Vector<VectorType<type>>
		static  value_type<type, size, VectorType> SolveAABBSphere(const AABB<type, size, VectorType>& lhs, const BoundingSphere<type, size, VectorType>& rhs);
		template<typename type, size_t size, template <typename>class VectorType> requires is_Vector<VectorType<type>> && (size == 2 || size == 3)
		static  value_type<type, size, VectorType> SolveOBB(const OBB<type, size, VectorType>& lhs, const OBB<type, size, VectorType>& rhs);
		template<typename type, size_t size, template <typename>class VectorType> requires is_Vector<VectorType<type>> && (size == 2 || size == 3)
			static  value_type<type, size, VectorType> SolveOBBAABB(const OBB<type, size, VectorType>& lhs, const OBB<type, size, VectorType>& rhs);

		struct Sat
		{
		public:
			template<typename type, size_t size, template <typename>class VectorType> requires is_Vector<VectorType<type>>
			static CollisionSolver::value_type<type, size, VectorType> GenericSat(const std::vector<VectorType<type>>& lhsPoints, const std::vector<VectorType<type>>& rhsPoints, const std::vector<VectorType<type>>& axes)
			{
				using vector_type = VectorType<type>;
				value_type<type, size, VectorType> result;

				type minOverlap = std::numeric_limits<type>::max();
				vector_type smallestAxis;

				vector_type lhsCenter = KT::GetCenter(lhsPoints);
				vector_type rhsCenter = KT::GetCenter(rhsPoints);
				vector_type Dir = rhsCenter - lhsCenter;
				// Allaxes
				for (auto& axe : axes)
				{
					auto ProjectLeft = GetMinAndMaxFromProjection(lhsPoints, axe);
					auto ProjectRight = GetMinAndMaxFromProjection(rhsPoints, axe);
					auto overlapInfo = OverLapResult(ProjectLeft, ProjectRight);
					if (!overlapInfo.first)
					{
						result.isColliding = false;
						return result;
					}
					if (overlapInfo.second < minOverlap)
					{

						minOverlap = overlapInfo.second;
						if (Dir.Dot(axe) < 0)
							smallestAxis = (axe * - 1).Normalize();
						else
						{
							smallestAxis = axe;
						}
					}
				}
				result.isColliding = true;
				result.depth = minOverlap;
				result.normal = smallestAxis;
				result.mvt = smallestAxis * minOverlap;
				return result;
			}
		private:
			template<typename type, template <typename>class VectorType> requires is_Vector<VectorType<type>>
			static std::pair<type,type> GetMinAndMaxFromProjection(const std::vector<VectorType<type>>& pts, const VectorType<type>& axe)
			{
				type min = std::numeric_limits<type>::max();
				type max = std::numeric_limits<type>::lowest();
				for (auto& p : pts)
				{
					type projection = p.Dot(axe);
					min = Math::Min(min, projection);
					max = Math::Max(max, projection);
				}
				return std::pair<type, type>(min, max);
			}
			template<typename type>
			static std::pair<bool,type> OverLapResult(const std::pair<type, type>& lhs, const std::pair<type, type>& rhs)
			{
				if (lhs.first > rhs.second || lhs.second < rhs.first)
					return std::pair<bool, type>(false, type());
				type overlap = Math::Min(lhs.second , rhs.second) - Math::Max(lhs.first,rhs.first);
				return std::pair<bool, type>(true, overlap);
			}
		};


	};



	











	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	CollisionSolver::value_type<type, size, VectorType> CollisionSolver::SolveAABB(
		const AABB<type, size, VectorType>& lhs, const AABB<type, size, VectorType>& rhs)
	{
		using vector_type = VectorType<type>;
		value_type<type, size, VectorType> result;

		result.isColliding = lhs.Intersects(rhs);
		if (!result.isColliding)
			return result;

		type minOverlap = std::numeric_limits<type>::max();
		vector_type smallestAxis;

		const vector_type distanceBetwenCenter = rhs.Center() - lhs.Center();
		const vector_type totalHalfSize = rhs.HalfSize() + lhs.HalfSize();
		for (size_t i = 0 ; i < size ; ++i)
		{
			type dist = distanceBetwenCenter.At(i);
			type totalHalf = totalHalfSize.At(i);
			type overlap = totalHalf - std::abs(dist);
			if (overlap <= 0)
				throw std::out_of_range("should not pass here");
			if (overlap < minOverlap)
			{
				minOverlap = overlap;
				smallestAxis = vector_type{};
				smallestAxis.At(i) = dist < 0 ? -1 : 1;
			}
		}

		result.depth = minOverlap;
		result.normal = smallestAxis;
		result.mvt = smallestAxis * minOverlap;
		return result;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	CollisionSolver::value_type<type, size, VectorType> CollisionSolver::SolveSphere(
		const BoundingSphere<type, size, VectorType>& lhs, const BoundingSphere<type, size, VectorType>& rhs)
	{
		using vector_type = VectorType<type>;
		value_type<type, size, VectorType> result;

		result.isColliding = lhs.Intersects(rhs);
		if (!result.isColliding)
			return result;
		type totalRadius = lhs.radius  + rhs.radius ;
		vector_type dir = rhs.Center() - lhs.Center();
		type dist = dir.Length();
		result.depth = totalRadius - dist;
		if (dist > 0)
			result.normal = dir.Normalize();
		else
		{
			result.normal = vector_type{};
			result.normal.At(0) = static_cast<type>(1);
		}
		result.mvt =  result.normal * result.depth;
		return result;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>>
	CollisionSolver::value_type<type, size, VectorType> CollisionSolver::SolveAABBSphere(
		const AABB<type, size, VectorType>& lhs, const BoundingSphere<type, size, VectorType>& rhs)
	{
		using vector_type = VectorType<type>;
		value_type<type, size, VectorType> result;

		vector_type closest;
		for (size_t i = 0 ; i < size;++i)
		{
			closest.At(i) = Math::Clamp(rhs.center.At(i), lhs.Amin.At(i), lhs.Amax.At(i));
		}
		vector_type diff = (closest - rhs.center);
		type distSquared = diff.SquareLength();
		result.isColliding = distSquared <= (rhs.radius * rhs.radius);
		if (!result.isColliding)
			return result;

		type dist = std::sqrt(distSquared);
			result.depth = rhs.radius - dist;
			result.normal = (diff / dist) * - 1;      
		result.mvt = result.normal * result.depth;

		return result;
	}

	template <typename type, size_t size, template <typename> class VectorType> requires is_Vector<VectorType<type>> &&
		(size == 2 || size == 3)
	CollisionSolver::value_type<type, size, VectorType> CollisionSolver::SolveOBB(
		const OBB<type, size, VectorType>& lhs, const OBB<type, size, VectorType>& rhs)
	{
		std::vector<VectorType<type>> axes;

		if constexpr (size == 3)
		{
			for (size_t i = 0; i < size; ++i)
			{
				axes.push_back(lhs.axes.at(i).Normalize());
				axes.push_back(rhs.axes.at(i).Normalize());
				for (size_t j = 0; j < size; ++j)
				{
					auto cross = lhs.axes.at(i).Normalize().Cross(rhs.axes.at(j).Normalize());
					if (!Math::IsNull(cross.SquareLength(), Math::EPSILON_V<type> *Math::EPSILON_V<type>))
						axes.push_back(cross.Normalize());
				}
			}
			return Sat::GenericSat < type,size,VectorType>(lhs.GetPts(), rhs.GetPts(), axes);
		}
		else 
		{
			axes.push_back(lhs.axes.at(0).Normalize());
			axes.push_back(lhs.axes.at(1).Normalize());
			axes.push_back(rhs.axes.at(0).Normalize());
			axes.push_back(rhs.axes.at(1).Normalize());
			return Sat::GenericSat < type, size, VectorType>(lhs.GetPts() , rhs.GetPts(), axes);
		}
	}
}
