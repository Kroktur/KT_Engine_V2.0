#pragma once
#include "Algorythm/PathFinding.h"
#include <functional>
#include <deque>

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

			template<bool(*isEqual)(Type*, Type*)>
			static PathFindingTrack Compute(const PathFindingNode& start, const PathFindingNode& end)
			{
				PathFindingTrack resultTrack;

				Queue OpenQueue;
				Queue ClosedQueue;

				float firstWeight = F(start, start, end);
				auto first = CreateNode(nullptr, start, firstWeight);
				OpenQueue.Add(first);


				Node* LastNode = nullptr;
				bool found = false;

				while (!OpenQueue.Empty() && !found)
				{
					LastNode = OpenQueue.Pop();
					ClosedQueue.Add(LastNode);

					if (isEqual(PathFindingConverter::ExtractType(LastNode->data), PathFindingConverter::ExtractType(end)))
					{
						found = true;
						break;
					}

					auto nodes = Converter<Type>::GetNodes(GetNeighbor(PathFindingConverter::ExtractType(LastNode->data)));
					for (auto& node : nodes)
					{
						float weight = F(start, node, end);
						Node* Anode = CreateNode(LastNode, node, weight);
						if (!OpenQueue.contains(Anode, isEqual) && !ClosedQueue.contains(Anode, isEqual))
							OpenQueue.Add(Anode);
						else
							delete Anode;
					}
				}

				if (!found)
					return resultTrack;

				resultTrack.SetValidity(true);

				auto* CurrentNode = LastNode;

				while (CurrentNode != nullptr)
				{
					resultTrack.PushFront(CurrentNode->data);
					CurrentNode = CurrentNode->parent;
				}

				return resultTrack;
			}
		private:

			// struct for Node
			struct Node
			{
				Node* parent = nullptr;
				PathFindingNode data;
				float weight;
			};

			// strucct for Queue
			struct Queue
			{
			public:
				void Add(Node* node)
				{
					for (auto it = queue.begin(); it != queue.end(); ++it)
					{
						if (node->weight < (*it)->weight)
						{
							queue.insert(it, node);
							return;
						}
					}
					queue.push_back(node);
				}
				Node* Pop()
				{
					auto front = queue.front();
					queue.pop_front();
					return front;
				}
				bool Empty() const
				{
					return queue.empty();
				}
				bool contains(Node* node, std::function<bool(Type*, Type*)> isEqual) const
				{
					for (auto it = queue.begin(); it != queue.end(); ++it)
					{
						if (isEqual(PathFindingConverter::ExtractType((*it)->data), PathFindingConverter::ExtractType(node->data)))
							return true;
					}
					return false;
				}
				~Queue()
				{
					while (!Empty())
					{
						auto* data = Pop();
						delete data;
					}
				}
			private:
				std::deque<Node*> queue;
			};
			static float F(const PathFindingNode& start, const PathFindingNode& current, const PathFindingNode& end)
			{
				return g(PathFindingConverter::ExtractType(start), PathFindingConverter::ExtractType(current)) + h(PathFindingConverter::ExtractType(current), PathFindingConverter::ExtractType(end));
			}
			static Node* CreateNode(Node* parent, const PathFindingNode& node, float weight)
			{
				auto MyNode = new Node{};
				MyNode->parent = parent;
				MyNode->data = node;
				MyNode->weight = weight;
				return MyNode;
			}

		};
	}
}