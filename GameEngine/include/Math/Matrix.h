#pragma once
#include <array>
#include <stdexcept>
#include <iostream>
#include "MyMath.h"

namespace KT
{

	template<typename type, size_t height, size_t width = height> requires is_arithmetic_v<type> && (height > 0 && width > 0)
	class Matrix
	{
	public:
		using value_type = type;
		using class_type = Matrix<type, height, width>;
		static constexpr size_t size = height * width;
		using iterator_type = typename std::array<type, size>::iterator;
		using const_iterator_type = typename std::array<type, size>::const_iterator;

		Matrix();
		Matrix(const class_type& other);
		template<typename... Args> requires (sizeof...(Args) <= height * width) && (... && (is_arithmetic_v<Args>))
		Matrix(const Args&... types);
		Matrix(class_type&& other) noexcept;
		~Matrix() = default;

		void Clear();
		static size_t Size();
		static size_t Height();
		static size_t Width();
		static value_type CofactorSign(const size_t& row, const size_t& col);
		static size_t GetCellIndex(const size_t& row, const size_t& col);

		void SetCell(const size_t& row, const size_t& col, const value_type& data);
		value_type& GetCell(const size_t& row, const size_t& col);
		const value_type& GetCell(const size_t& row, const size_t& col) const;

		value_type& operator[](const size_t& index);
		value_type& At(const size_t& index);
		const value_type& operator[](const size_t& index) const;
		const value_type& At(const size_t& index)const;

		class_type& operator=(const class_type& other);
		class_type& operator=(class_type&& other) noexcept;
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

		bool IsNull() const;

		// range-based loop
		iterator_type begin();
		iterator_type end();
		const_iterator_type begin() const;
		const_iterator_type end()   const;

		// std::set or std::map
		bool operator<(const class_type& other) const;
		bool operator>(const class_type& other) const;


		template<size_t otherWidth>requires (otherWidth >= 1)
		Matrix<value_type, height, otherWidth> MatrixProduct(const Matrix<value_type, width, otherWidth>& other);
		KT::VectorND<type,height> MatrixProduct(const KT::VectorND<type, width>& vec);
		Matrix<value_type, width, height> Transposition() const;
		static class_type Identity() requires(height == width);
		value_type Det()const  requires(height == width);
		class_type Inverse()const  requires(height == width);
		Matrix<value_type, width, height> Inverse() const requires(height != width);
		class_type CofactorMatrix() const  requires(height == width);
	protected:
		std::array< value_type, size> m_data;
	};

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	Matrix<type, height, width>::Matrix()
	{
		std::fill(m_data.begin(), m_data.end(), type{});
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	Matrix<type, height, width>::Matrix(const class_type& other) : m_data(other.m_data) {}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	template <typename ... Args> requires (sizeof...(Args) <= height * width) && (... && (is_arithmetic_v<Args>))
		Matrix<type, height, width>::Matrix(const Args&... types) : m_data(std::array<value_type, size>{static_cast<type>(types)...}) {}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	Matrix<type, height, width>::Matrix(class_type&& other) noexcept : m_data(std::move(other.m_data))
	{

	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	void Matrix<type, height, width>::Clear()
	{
		std::fill(m_data.begin(), m_data.end(), type{});
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	size_t Matrix<type, height, width>::Size()
	{
		return size;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	size_t Matrix<type, height, width>::Height()
	{
		return height;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	size_t Matrix<type, height, width>::Width()
	{
		return  width;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	typename Matrix<type, height, width>::value_type Matrix<type, height, width>::CofactorSign(const size_t& row, const size_t& col)
	{
		return static_cast<type>(((row + col) % 2 == 0) ? 1 : -1);
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	size_t Matrix<type, height, width>::GetCellIndex(const size_t& row, const size_t& col)
	{
		auto index = row * width + col;
		if (index >= size)
			throw std::out_of_range("out of range");
		return index;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	void Matrix<type, height, width>::SetCell(const size_t& row, const size_t& col, const value_type& data)
	{
		GetCell(row, col) = data;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	typename Matrix<type, height, width>::value_type& Matrix<type, height, width>::GetCell(const size_t& row, const size_t& col)
	{
		return m_data[GetCellIndex(row, col)];
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	typename Matrix<type, height, width>::value_type const& Matrix<type, height, width>::GetCell(const size_t& row, const size_t& col) const
	{
		return m_data[GetCellIndex(row, col)];
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	typename Matrix<type, height, width>::value_type& Matrix<type, height, width>::operator[](const size_t& index)
	{
		return m_data[index];
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	typename Matrix<type, height, width>::value_type& Matrix<type, height, width>::At(const size_t& index)
	{
		if (index >= size)
			throw std::runtime_error("index is too big ");
		return m_data.at(index);
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	typename Matrix<type, height, width>::value_type const& Matrix<type, height, width>::operator[](const size_t& index) const
	{
		return m_data[index];
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	typename Matrix<type, height, width>::value_type const& Matrix<type, height, width>::At(const size_t& index) const
	{
		if (index >= size)
			throw std::runtime_error("index is too big ");
		return m_data.at(index);
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	Matrix<type, height, width>& Matrix<type, height, width>::operator=(const class_type& other)
	{
		if (this != &other)
			m_data = other.m_data;
		return *this;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	Matrix<type, height, width>& Matrix<type, height, width>::operator=(class_type&& other) noexcept
	{
		m_data = std::move(other.m_data);
		return *this;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	bool Matrix<type, height, width>::operator==(const class_type& other) const
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

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	bool Matrix<type, height, width>::operator!=(const class_type& other) const
	{
		return !(*this == other);
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	Matrix<type, height, width> Matrix<type, height, width>::operator+(const class_type& other) const
	{
		Matrix result;
		for (size_t i = 0; i < size; ++i)
		{
			result[i] = m_data[i] + other[i];
		}
		return  result;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	Matrix<type, height, width> Matrix<type, height, width>::operator-(const class_type& other) const
	{
		Matrix result;
		for (size_t i = 0; i < size; ++i)
		{
			result[i] = m_data[i] - other[i];
		}
		return  result;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	Matrix<type, height, width>& Matrix<type, height, width>::operator+=(const class_type& other)
	{
		for (size_t i = 0; i < size; ++i)
		{
			m_data[i] += other[i];
		}
		return  *this;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	Matrix<type, height, width>& Matrix<type, height, width>::operator-=(const class_type& other)
	{
		for (size_t i = 0; i < size; ++i)
		{
			m_data[i] -= other[i];
		}
		return  *this;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	Matrix<type, height, width> Matrix<type, height, width>::operator*(const value_type& factor) const
	{
		Matrix result;
		for (size_t i = 0; i < size; ++i)
		{
			result[i] = m_data[i] * factor;
		}
		return result;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	Matrix<type, height, width> Matrix<type, height, width>::operator/(const value_type& divider) const
	{
		if (divider == 0)
			throw std::out_of_range("can't divide by 0");
		Matrix result;
		for (size_t i = 0; i < size; ++i)
		{
			result[i] = m_data[i] / divider;
		}
		return  result;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	Matrix<type, height, width>& Matrix<type, height, width>::operator*=(const value_type& factor)
	{
		for (size_t i = 0; i < size; ++i)
		{
			m_data[i] *= factor;
		}
		return *this;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	Matrix<type, height, width>& Matrix<type, height, width>::operator/=(const value_type& divider)
	{
		if (divider == 0)
			throw std::out_of_range("can't divide by 0");

		for (size_t i = 0; i < size; ++i)
		{
			m_data[i] /= divider;
		}
		return  *this;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	bool Matrix<type, height, width>::IsNull() const
	{
		if constexpr (is_floating_type_v<value_type>)
		{
			value_type Epsilon = Math::EPSILON_V<value_type>;
			for (auto it : m_data)
			{
				if (!Math::IsNull<type>(it, Epsilon))
					return false;
			}
		}
		else
		{
			for (auto it : m_data)
			{
				if (it != 0)
					return false;
			}
		}
		return true;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	typename Matrix<type, height, width>::iterator_type Matrix<type, height, width>::begin()
	{
		return m_data.begin();
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	typename Matrix<type, height, width>::iterator_type Matrix<type, height, width>::end()
	{
		return m_data.end();
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	typename Matrix<type, height, width>::const_iterator_type Matrix<type, height, width>::begin() const
	{
		return m_data.begin();
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	typename Matrix<type, height, width>::const_iterator_type Matrix<type, height, width>::end() const
	{
		return m_data.end();
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	bool Matrix<type, height, width>::operator<(const class_type& other) const
	{
		for (size_t i = 0; i < size; ++i)
		{
			if (m_data[i] != other.m_data[i]) return m_data[i] < other.m_data[i];
		}
		return false;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	bool Matrix<type, height, width>::operator>(const class_type& other) const
	{
		for (size_t i = 0; i < size; ++i)
		{
			if (m_data[i] != other.m_data[i]) return m_data[i] > other.m_data[i];
		}
		return false;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	template <size_t otherWidth> requires (otherWidth >= 1)
		Matrix<type, height, otherWidth> Matrix<type, height, width>::MatrixProduct(const Matrix<value_type, width, otherWidth>& other)
	{
		Matrix<type, height, otherWidth> result;
		for (size_t row = 0; row < height; ++row)
		{
			for (size_t col = 0; col < otherWidth; ++col)
			{
				auto data = type{};
				for (size_t idx = 0; idx < width; ++idx)
					data += GetCell(row, idx) * other.GetCell(idx, col);
				result.GetCell(row, col) = Math::IsNull(data, Math::EPSILON_V<type>) ? static_cast<type>(0) : data;
			}
		}
		return result;
	}

	template <typename type, size_t height, size_t width> requires is_arithmetic_v<type> && (height > 0 && width > 0)
	VectorND<type, height> Matrix<type, height, width>::MatrixProduct(const VectorND<type, width>& vec)
	{
		VectorND<type, height> result;
		for (size_t row = 0; row < height; ++row)
		{
			type sum = 0;
			for (size_t col = 0; col < width; ++col)
			{
				sum += GetCell(row, col) * vec.At(col);
			}
			result.At(row) = Math::IsNull(sum, Math::EPSILON_V<type>) ? type(0) : sum;
		}
		return result;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	Matrix<type, width, height> Matrix<type, height, width>::Transposition() const
	{
		Matrix<type, width, height> result;
		for (size_t row = 0; row < height; ++row)
		{
			for (size_t col = 0; col < width; ++col)
			{
				result.GetCell(col, row) = GetCell(row, col);
			}
		}
		return result;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	typename Matrix<type, height, width>::class_type Matrix<type, height, width>::Identity()  requires(height == width)
	{

		auto matrixResult = Matrix<type, height, width>{};
		matrixResult.Clear();

		for (size_t i = 0; i < height; ++i)
		{
			matrixResult.SetCell(i, i, 1);
		}
		return matrixResult;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	type Matrix<type, height, width>::Det() const   requires(height == width)
	{
		if constexpr (height == 1)
		{
			return GetCell(0, 0);
		}
		else if constexpr (height == 2)
		{
			return GetCell(0, 0) * GetCell(1, 1) - GetCell(0, 1) * GetCell(1, 0);
		}
		else {
			type det = type{};
			for (size_t col = 0; col < width; ++col)
			{
				type factor = GetCell(0, col);
				Matrix<type, height - 1, width - 1> sub;
				for (size_t raw = 1; raw < height; ++raw)
				{
					int realcol = 0;
					for (size_t col2 = 0; col2 < width; ++col2)
					{
						if (col2 == col)
							continue;

						sub[sub.GetCellIndex(raw - 1, realcol)] = GetCell(raw, col2);
						realcol++;
					}
				}
				type sign = CofactorSign(0, col);

				det += sign * factor * sub.Det();
			}
			return det;
		}
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	typename Matrix<type, height, width>::class_type Matrix<type, height, width>::Inverse() const  requires(height == width)
	{
		auto det = Det();
		if (det == static_cast<type>(0))
			throw std::out_of_range("det can't be 0");
		auto result = CofactorMatrix().Transposition() * (1 / Det());
		return  result;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	Matrix<type, width, height> Matrix<type, height, width>::Inverse() const requires (height != width)
	{
		auto matrix = *this;
		Matrix<type, width, height> At = matrix.Transposition();
		Matrix<type, height> AAt = matrix.MatrixProduct(At);
		Matrix<type, height> AAt_inv = AAt.Inverse();
		Matrix<type, width, height> A_pseudoInv = At.MatrixProduct(AAt_inv);
		return A_pseudoInv;
	}

	template <typename type, size_t height, size_t width>requires is_arithmetic_v<type> && (height > 0 && width > 0)
	typename Matrix<type, height, width>::class_type Matrix<type, height, width>::CofactorMatrix() const  requires(height == width)
	{
		Matrix<type, height, width > result;
		for (size_t row = 0; row < height; ++row)
		{
			for (size_t col = 0; col < width; ++col)
			{
				Matrix<type, height - 1, width - 1 > sub;
				size_t realRow = 0;

				for (size_t row2 = 0; row2 < height; ++row2)
				{
					if (row2 == row)
						continue;
					size_t realCol = 0;
					for (size_t col2 = 0; col2 < width; ++col2)
					{
						if (col == col2)
							continue;
						sub.GetCell(realRow, realCol) = GetCell(row2, col2);
						realCol++;
					}
					realRow++;
				}
				result.GetCell(row, col) = CofactorSign(row, col) * sub.Det();
			}
		}
		return result;
	}


}


template<typename type, size_t height, size_t width>
std::ostream& operator<<(std::ostream& os, const KT::Matrix<type, height, width >& tab)
{
	if (tab.Size() == 0)
		return os;
	for (auto i = 0; i < height; ++i)
	{
		os << "(";
		for (auto j = 0; j < width; ++j)
		{
			os << tab.GetCell(i, j);
			if (j != width - 1)
				os << ",";
		}
		os << ")" << "\n";
	}

	return os;
}