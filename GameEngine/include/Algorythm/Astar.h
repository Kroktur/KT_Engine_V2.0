#pragma once
#include "Algorythm/PathFinding.h"
#include <functional>
#include <deque>
#include <optional>
namespace KT
{
	namespace PathFinding
	{
		template<typename Type, float (*g)(Type*, Type*), float(*h)(Type*, Type*), std::vector<Type*>(*GetNeighbor)(Type*)>
struct Astar
{
	using Algo_Key = int;
	using PathFindingNode = Node<Type>;
	using PathFindingTrack = Track<Type>;
	using PathFindingConverter = Converter<Type>;

	template<bool(*isEqual)(Type*,Type*)>
	static PathFindingTrack Compute(const PathFindingNode& start, const PathFindingNode& end)
	{
		PathFindingTrack resultTrack;

		std::vector<Node> allNodes;

		Queue OpenQueue;

		float firstWeight = F(start, start, end);
		Node first = Node{};
		first.data = start;
		first.index = 0;
		first.weight = firstWeight;
		OpenQueue.Add(first);
		allNodes.push_back(first);

		Node LastNode;
		bool found = false;

		while (!OpenQueue.Empty() && !found)
		{
			LastNode = OpenQueue.Pop();

			if (isEqual(PathFindingConverter::ExtractType(LastNode.data), PathFindingConverter::ExtractType( end)))
			{
				found = true;
				break;
			}

			auto nodes = Converter<Type>::GetNodes(GetNeighbor(PathFindingConverter::ExtractType(LastNode.data)));
			for (auto& node : nodes)
			{
				auto it = std::find_if(allNodes.begin(), allNodes.end(), [&](const Node& n)
					{
						return 	isEqual(PathFindingConverter::ExtractType(node), PathFindingConverter::ExtractType(n.data));
					});
				if (it != allNodes.end())
					continue;

				float weight = F(start, node, end);
				Node Anode = Node{};
				Anode.data = node;
				Anode.weight = weight;
				Anode.index = allNodes.size();
				Anode.parentIndex = LastNode.index;
				OpenQueue.Add(Anode);
				allNodes.push_back(Anode);
			}
		}

		if (!found)
			return resultTrack;

		resultTrack.SetValidity(true);

		std::optional<Node> CurrentNode = LastNode;

		while (CurrentNode.has_value())
		{
			resultTrack.PushFront(CurrentNode->data);
			if (CurrentNode->parentIndex.has_value())
				CurrentNode = allNodes[CurrentNode->parentIndex.value()];
			else
				CurrentNode.reset();
		}

		return resultTrack;
	}
private:

	// struct for Node
	struct Node
	{
		std::optional<int> parentIndex;
		int index;

		PathFindingNode data;

		float weight;
	};

	// strucct for Queue
	struct Queue
	{
	public:
		void Add(const Node& node)
		{
			for (auto it = queue.begin(); it != queue.end(); ++it)
			{
				if (node.weight < it->weight)
				{
					queue.insert(it, node);
					return;
				}
			}
			queue.push_back(node);
		}
		Node Pop()
		{
			auto front = queue.front();
			queue.pop_front();
			return front;
		}
		bool Empty() const
		{
			return queue.empty();
		}
	private:
		std::deque<Node> queue;
	};
	static float F(const PathFindingNode& start, const PathFindingNode& current, const PathFindingNode& end)
	{
		return g(PathFindingConverter::ExtractType(start), PathFindingConverter::ExtractType(current)) + h(PathFindingConverter::ExtractType(current), PathFindingConverter::ExtractType(end));
	}
};
	}
}