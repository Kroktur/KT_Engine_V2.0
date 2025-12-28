#pragma once
#include <vector>
#include <stdexcept>
namespace KT
{
	namespace PathFinding
	{

		template<typename Type>
		struct Converter;

		// class Node OK
		template<typename Type>
		struct Node
		{
			Node() = default;
			Node(Type* d) : data(d) {}
			friend Converter<Type>;
		private:
			Type* data;
		};

		// class Track OK
		template<typename Type>
		struct Track
		{
			using node = Node<Type>;
			Track(bool valid = false) : m_data(), m_isValid(valid) {}
			Track(const std::vector<node>& data, bool valid) : m_data(data), m_isValid(valid) {}
			bool IsValid() const
			{
				return m_isValid;
			}
			void SetValidity(bool valid)
			{
				m_isValid = valid;
			}
			void PushFront(const node& n)
			{
				m_data.insert(m_data.begin(), n);
			}
			void PushBack(const node& n)
			{
				m_data.push_back(n);
			}
			bool Empty() const
			{
				return m_data.empty();
			}
			size_t Size() const
			{
				return m_data.size();
			}
			node First() const
			{
				if (Empty() || !m_isValid)
					throw std::out_of_range("out of range");
				return m_data.front();
			}
			node Last() const
			{
				if (Empty() || !m_isValid)
					throw std::out_of_range("out of range");
				return m_data.back();
			}
			std::vector<node> GetNodes() const
			{
				return m_data;
			}
		private:
			bool m_isValid;
			std::vector<node> m_data;
		};

		// ok
		template<typename Type>
		struct Converter
		{
			using PathFindingNode = Node<Type>;
			static PathFindingNode GetNode(Type* t)
			{
				return PathFindingNode(t);
			}
			static std::vector<PathFindingNode> GetNodes(const std::vector<Type*>& vec)
			{
				std::vector<PathFindingNode> result;
				result.reserve(vec.size());
				for (auto& data : vec)
					result.push_back(PathFindingNode(data));
				return result;
			}
			static Type* ExtractType(const PathFindingNode& node)
			{
				return node.data;
			}
			static std::vector<Type*> ExtractTypes(const std::vector<PathFindingNode>& vec)
			{
				std::vector<Type*> result;
				result.reserve(vec.size());
				for (auto& data : vec)
					result.push_back(ExtractType(data));
				return result;
			}
		};

		template<typename T>
		concept PathFindingAlgorythm = requires
		{
			typename T::Algo_Key;
		};

		template<typename Type, bool(*isEqual)(Type*, Type*)>
		struct Execute
		{
			using PathFindingConverter = Converter<Type>;
			using PathFindingTrack = Track<Type>;
			template<PathFindingAlgorythm Algo, typename... Args>
			static PathFindingTrack Compute(Type* start, Type* end, Args&&... args)
			{
				return  Algo::template Compute<isEqual>(PathFindingConverter::GetNode(start), PathFindingConverter::GetNode(end), std::forward<Args>(args)...);
			}
		};


	}
}