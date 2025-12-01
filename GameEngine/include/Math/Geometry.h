#pragma once
#include "VectorND.h"
#include <vector>
namespace KT
{
	
	template<typename vecType> requires is_Vector<vecType>
	static vecType GetCenter(const std::vector<vecType>& vec)
	{
		if (vec.empty())
			throw std::runtime_error("vector is empty");
		vecType center;
		for (auto& elmem : vec)
		{
			center += elmem;
		}
		return center / vec.size();
	}
}
