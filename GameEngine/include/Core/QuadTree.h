#pragma once
#include "Math/AABB.h"
#include <array>
#include <vector>
#include <deque>
#include <functional>
#include <set>

template<size_t Dim>
using NodeBox = std::conditional_t<
	Dim == 3,
	KT::AABB3DF,
	KT::AABB2DF
>;
using box = KT::AABB2DF;



template<size_t count,size_t Dim> requires( Dim == 2 || Dim == 3)
struct Node
{
	using boxTest = NodeBox<Dim>;
	static constexpr size_t offset = 2 * Dim - 1;
	size_t firstChildIndex;
	bool isSubDivide = false;
	box AABB = box{};
};
template<size_t count,size_t Dim,size_t reservedSize >
struct NodeAllocator
{
	NodeAllocator() {
		nodePull.reserve(reservedSize);
	}
	size_t NewValidIndex()
	{
		
		if (validIndex.empty())
		{
			if (nodePull.capacity() == nodePull.size())
				nodePull.reserve(nodePull.size() + reservedSize);

			nodePull.emplace_back();
			return nodePull.size() - 1;
		}
			const size_t i = validIndex.back();
			validIndex.pop_back();
			return i;
	}
	void FreeIndex(size_t i)
	{
		validIndex.push_back(i);
	}
	Node<count,Dim>& GetNode(size_t index)
	{
		
		return nodePull[index];
	}
private:

	std::vector<Node<count,Dim>> nodePull;
	std::vector<size_t> validIndex;
};


class IGO
{
public:
	virtual ~IGO() = default;
	virtual void Update(float dt) = 0;
	virtual void Render(sf::RenderWindow* win) = 0;
	virtual void Input() = 0;
};



struct EntityInfo
{
	IGO* entity;
	box aabb;

	bool operator<(const EntityInfo& other) const
	{
		return  entity < other.entity;
	}
};

namespace Quad
{
	enum  Cardinal 
	{
		NorthWest = 0,
		NorthEst = 1,
		SouthWest = 2,
		SouthEst = 3
	};

	template<size_t count>
	struct Node
	{
		Node(Node* parent_, const box& aabb_) :
		parent(parent_),
		aabb(aabb_),
		childNodes(std::array<std::unique_ptr<Node>, 4>{nullptr,nullptr,nullptr,nullptr})
		{}

		bool IsSubDivided() const 
		{
			return childNodes[Cardinal::NorthWest].get() ;
		}
		void SubDivide()
		{
			if (IsSubDivided())
				throw std::runtime_error("already subdivided");

			KT::Vector2F center = aabb.Center();
			// North West
			{
				KT::Vector2F amin;
				
				amin.x = aabb.Amin.x;
				amin.y = center.y;

				KT::Vector2F amax;

				amax.x = center.x;
				amax.y = aabb.Amax.y;

				childNodes[Cardinal::NorthWest] = std::make_unique<Node>(this, box(amin, amax));
			}

			// North Est
			{
				KT::Vector2F amin = center;

				KT::Vector2F amax = aabb.Amax;

				childNodes[Cardinal::NorthEst] = std::make_unique<Node>(this, box(amin, amax));
			}

			// South West
			{
				KT::Vector2F amin = aabb.Amin;

				KT::Vector2F amax = center;

				childNodes[Cardinal::SouthWest] = std::make_unique<Node>(this, box(amin, amax));
			}

			// South Est
			{
				KT::Vector2F amin;

				amin.x = center.x;
				amin.y = aabb.Amin.y;

				KT::Vector2F amax;

				amax.x = aabb.Amax.x;
				amax.y = center.y;

				childNodes[Cardinal::SouthEst] = std::make_unique<Node>(this, box(amin, amax));
			}

		}

		bool ContainEntity(const EntityInfo& e)
		{
			auto it = std::find_if(m_entity.begin(), m_entity.end(), [e](const EntityInfo& entity) {return e.entity == entity.entity; });
			return it != m_entity.end();
		}
		void UpdateEntityBox(const EntityInfo& e)
		{
			auto it = std::find_if(m_entity.begin(), m_entity.end(), [e](const EntityInfo& entity) {return e.entity == entity.entity; });
			if (it == m_entity.end())
				throw std::out_of_range("not found");
			it->aabb = e.aabb;
		}
		void EraseEntity(const EntityInfo& e)
		{
			auto it = std::find_if(m_entity.begin(), m_entity.end(), [e](const EntityInfo& entity) {return e.entity == entity.entity; });
			if (it == m_entity.end())
				throw std::out_of_range("not found");
			m_entity.erase(it);
		}

		size_t Size() const
		{
			return m_entity.size();
		}
		bool IsInsertable() const
		{
			return  Size() <= count && !IsSubDivided();
		}

		void Distribute()
		{
			// if notSubdivide it's all good don't need to Distribute
			if (!IsSubDivided())
				return;
			// create list and add child
			std::deque<Node*> openList;
			for (int i = 0; i < 4; ++i)
				openList.push_back(childNodes[i].get());
			while (!openList.empty())
			{
				// get the first in the list
				auto* current = openList.front();
				openList.pop_front();

				// if it is subdivided add the child and continue 
				if (current->IsSubDivided())
				{
					for (int i = 0; i < 4; ++i)
						openList.push_back(current->childNodes[i].get());
					continue;
				}
				// if not that means it's a child so add the one that intersect 
				for (auto& e : m_entity)
				{
					if (!current->aabb.Intersects(e.aabb))
						continue;
					current->m_entity.push_back(e);
				}
			}
			// clear the actual vector
			m_entity.clear();
		}

		std::vector<EntityInfo> GetAllEntity() const
		{
			// if not subDivided return current vector
			if (!IsSubDivided())
				return m_entity;
			// create a list and add child
			std::vector<EntityInfo> result;
			std::deque<Node*> openList;
			for (int i = 0; i < 4; ++i)
				openList.push_back(childNodes[i].get());

			// while is not empty
			while (!openList.empty())
			{
				// take the first element of the list
				auto* current = openList.front();
				openList.pop_front();

				// if it's subdivided that means there is no element in there 
				if (current->IsSubDivided())
				{
					// add the child and continue
					for (int i = 0; i < 4; ++i)
						openList.push_back(current->childNodes[i].get());
					continue;
				}
				//you are in a child so just add the element but don't double
				for (int i = 0 ; i < current->Size(); ++i)
				{
					//TODO optimise with std::set or unordered set
					auto it = std::find_if(result.begin(), result.end(), [&](const EntityInfo& e)
						{
							return	e.entity == current->m_entity[i].entity;
						});
					if (it == result.end())
						result.push_back(current->m_entity[i]);

				}
			}
			// return the result
			return result;
		}
		bool IsMergable() const 
		{
			if (!IsSubDivided())
				return false;

			size_t countAprox = 0;
			std::vector<EntityInfo> result;
			std::deque<Node*> openList;
			for (int i = 0; i < 4; ++i)
				openList.push_back(childNodes[i].get());

			// while is not empty
			while (!openList.empty())
			{
				// take the first element of the list
				auto* current = openList.front();
				openList.pop_front();

				// if it's subdivided that means there is no element in there 
				if (current->IsSubDivided())
				{
					// add the child and continue
					for (int i = 0; i < 4; ++i)
						openList.push_back(current->childNodes[i].get());
					continue;
				}
				//you are in a child so just add the element but don't double
				for (int i = 0; i < current->Size(); ++i)
				{
					countAprox += current->m_entity.size();
					if (countAprox > count)
						return false;
				}
			
			}
			return true;
		}
		void Merge()
		{
			// just delete the regions
			for (int i = 0 ; i < 4 ;++i)
			{
				childNodes[i].reset(nullptr);
			}
		}


		Node* parent;
		std::array<std::unique_ptr<Node>, 4> childNodes;
		box aabb;
		std::vector<EntityInfo> m_entity;
	};

	struct QuadTree
	{
		using Node = Node<10>;
		QuadTree(const box& aabb) : m_anchor(nullptr,aabb){}

		void Insert(const EntityInfo& e)
		{
			 if (!Insert(&m_anchor, e))
				 throw std::out_of_range("impossible to  add this entity");
		}
		void Update(const EntityInfo& e,box newAABB)
		{
			Update(&m_anchor,e, newAABB);
		}
		void Remove(const EntityInfo& e)
		{
			Remove(&m_anchor, e);
		}
		void ExecuteAction(const std::function<void(Node*)>& fn)
		{
			fn(&m_anchor);
		}
		void UpdataMerge()
		{
			std::deque<Node*> openQueue;
			openQueue.push_front(&m_anchor);

			
			while (!openQueue.empty())
			{
				// get the first node on the list and pop it 
				auto* current = openQueue.front();
				openQueue.pop_front();

				

				if (current->IsMergable())
				{
					auto vec =current->GetAllEntity();
					current->Merge();
					current->m_entity = vec;
					continue;
				}

				// if its subdivide go to child
				if (current->IsSubDivided())
				{
					// for each part 
					for (int i = 0; i < 4; ++i)
						// add the child to test 
						openQueue.push_back(current->childNodes[i].get());
					continue;
				}
			}

		}
	private:

		static bool Insert(Node* node, const EntityInfo& e)
		{
			bool insert = false;
			std::deque<Node*> openQueue;
			openQueue.push_front(node);
			while (!openQueue.empty())
			{
				// get the first node on the list and pop it 
				auto* current = openQueue.front();
				openQueue.pop_front();

				// if its not intersecing continue
				if (!current->aabb.Intersects(e.aabb))
					continue;

				// if its subdivide go to child
				if (current->IsSubDivided())
				{
					// for each part 
					for (int i = 0; i < 4; ++i)
						// add the child to test 
						openQueue.push_back(current->childNodes[i].get());
					continue;
				}

				// this fonction check if isSubdivide in our case not because the previous if and if there is space for an other entity 
				if (!current->IsInsertable())
				{

					current->SubDivide();
					current->Distribute();
					// for each part 
					for (int i = 0; i < 4; ++i)
					{
						// add the child to test
						openQueue.push_back(current->childNodes[i].get());
					}
					continue;
				}
				// in this case we are in a node that has no child and is not full so we add the entity to it and continue il the entity is in multiple area
			
				if (!current->ContainEntity(e))
				{
					insert = true;
					current->m_entity.push_back(e);
				}
			}
			// this is just a verification if the entity is at leat insert once 
			return  insert;

		}
		 bool Remove(Node* node, const EntityInfo& e)
		{
			bool removed = false;
			std::deque<Node*> openQueue;
			openQueue.push_front(node);
			while (!openQueue.empty())
			{
				// get the first node on the list and pop it 
				auto* current = openQueue.front();
				openQueue.pop_front();

				// if its not intersecing continue
				if (!current->aabb.Intersects(e.aabb))
					continue;

				// if its subdivide go to child
				if (current->IsSubDivided())
				{
					// for each part 
					for (int i = 0; i < 4; ++i)
						// add the child to test 
						openQueue.push_back(current->childNodes[i].get());
					continue;
				}

				if (current->ContainEntity(e))
				{
					removed = true;
					current->EraseEntity(e);
					if (current->parent)
					toUpdate.insert(current->parent);
				}
				else
				{
					throw std::out_of_range("current is node contained in a node he should");
				}
			}
			// this is just a verification if the entity is at leat insert once 
			return  removed;
		}
		 void Update(Node* node,EntityInfo e,box newAABB)
		{
			 if (!Remove(node, e))
			 {
				 throw std::out_of_range("impossible to remove this entity");
			 }
			e.aabb = newAABB;
			if (!Insert(node, e))
			{
				throw std::out_of_range("impossible to add this entity");
			}

		}
		Node m_anchor;
		std::set<Node*> toUpdate;
	};


}