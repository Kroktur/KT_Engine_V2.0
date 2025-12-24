#pragma once
#include <functional>
#include <vector>
#include <stdexcept>

namespace KT
{
	


template<typename BaseType, typename RootType>
class IComposite;

template<typename BaseType, typename RootType >
class IRoot;

template<typename BaseType, typename RootType>
class IComponent
{
public:
	using composite = IComposite<BaseType, RootType>;
	using root = IRoot<BaseType, RootType>;
	friend composite;
	virtual ~IComponent();

	composite* GetOwner() const;
	void SetOwner(composite* parent);

	virtual void ExecuteAction(const std::function<void(BaseType*)>& fn) = 0;
	virtual void ExecuteAction(const std::function<void(IComponent*)>& fn) = 0;
	virtual BaseType* AsBase() = 0;
	virtual const BaseType* AsBase() const = 0;

	virtual const root* GetRoot() const;
	virtual  root* GetRoot();
	virtual bool IsInSubTree(IComponent* component) const;
protected:
	IComponent(composite* owner);
	virtual void AddFullTree(std::vector<IComponent*>& vec);
private:
	// for root Only
	IComponent();
	composite* m_owner;
};


template<typename BaseType, typename RootType >
class IComposite : public IComponent<BaseType, RootType>
{
public:
	using component = IComponent<BaseType, RootType>;
	friend component;
	using root = IRoot<BaseType, RootType>;
	friend root;
	~IComposite() override;

	std::vector<component*> GetChild() const;
	std::vector<component*> GetAllTree();

	BaseType* AsBase() override = 0;
	const BaseType* AsBase() const override = 0;
	void ExecuteAction(const std::function<void(BaseType*)>& fn) override = 0;
	void ExecuteAction(const std::function<void(component*)>& fn) override = 0;
	bool IsInSubTree(component* component) const override;
	bool HasChild(component* component) const;
	size_t Size() const;

	std::vector<BaseType*> ChildAsBase();
	std::vector<BaseType*> AllTreeAsBase();
protected:
	IComposite(IComposite* owner);
	void AddFullTree(std::vector<component*>& vec) override;
	std::vector<component*> m_child;
private:
	IComposite();
	void Add(component* child);
	void Remove(component* child);
};

template<typename BaseType, typename RootType >
class IRoot : public IComposite<BaseType, RootType>
{
public:
	using component = IComponent<BaseType, RootType>;
	using composite = IComposite<BaseType, RootType>;

	~IRoot() override = default;
	void ExecuteAction(const std::function<void(BaseType*)>& fn) override = 0;
	void ExecuteAction(const std::function<void(component*)>& fn) override = 0;
	BaseType* AsBase() override = 0;
	const BaseType* AsBase() const override = 0;
	virtual RootType* AsRoot() = 0;
	virtual const RootType* AsRoot() const = 0;

	const IRoot<BaseType, RootType>* GetRoot()const  override;
	IRoot<BaseType, RootType>* GetRoot() override;
protected:
	IRoot();
	void AddFullTree(std::vector<component*>& vec) override;
};

template<typename BaseType, typename RootType >
class ILeaf : public IComponent<BaseType, RootType>
{
public:
	using component = IComponent<BaseType, RootType>;
	using composite = IComposite<BaseType, RootType>;
	ILeaf(composite* owner);
	~ILeaf() override = default;
	void ExecuteAction(const std::function<void(BaseType*)>& fn) override = 0;
	void ExecuteAction(const std::function<void(component*)>& fn) override = 0;
	BaseType* AsBase() override = 0;
	const BaseType* AsBase() const override = 0;
};

// IComponent Implementation

template <typename BaseType, typename RootType>
IComponent<BaseType, RootType>::IComponent(composite* owner) : m_owner(nullptr)
{
	if (owner == nullptr)
		throw std::out_of_range("need root");
	SetOwner(owner);
}

template <typename BaseType, typename RootType>
IComponent<BaseType, RootType>::IComponent() : m_owner(nullptr)
{
	SetOwner(nullptr);
}

template <typename BaseType, typename RootType>
IComponent<BaseType, RootType>::~IComponent()
{
	SetOwner(nullptr);
}

template <typename BaseType, typename RootType>
typename IComponent<BaseType, RootType>::composite* IComponent<BaseType, RootType>::GetOwner() const
{
	return m_owner;
}

template <typename BaseType, typename RootType>
const typename IComponent<BaseType, RootType>::root* IComponent<BaseType, RootType>::GetRoot() const
{
	if (!m_owner)
		return nullptr;
	return m_owner->GetRoot();
}

template <typename BaseType, typename RootType>
typename IComponent<BaseType, RootType>::root* IComponent<BaseType, RootType>::GetRoot()
{
	if (!m_owner)
		return nullptr;
	return m_owner->GetRoot();
}

template <typename BaseType, typename RootType>
bool IComponent<BaseType, RootType>::IsInSubTree(IComponent* component) const
{
	return this == component;
}

template <typename BaseType, typename RootType>
void IComponent<BaseType, RootType>::AddFullTree(std::vector<IComponent*>& vec)
{
	vec.push_back(this);
}



template<typename BaseType, typename RootType >
void IComponent<BaseType, RootType>::SetOwner(composite* parent)
{
	if (m_owner == parent)
		return;

	if (m_owner)
		m_owner->Remove(this);
	m_owner = parent;
	if (m_owner)
		m_owner->Add(this);
}

// IComposite Implementation

template <typename BaseType, typename RootType>
IComposite<BaseType, RootType>::IComposite(IComposite* owner) : component(owner)
{

}

template <typename BaseType, typename RootType>
IComposite<BaseType, RootType>::IComposite() : component()
{
}


template <typename BaseType, typename RootType>
IComposite<BaseType, RootType>::~IComposite()
{
	for (int i = static_cast<int>(m_child.size()) - 1; i >= 0; --i)
	{
		delete m_child[i];
	}
	m_child.clear();
}

template <typename BaseType, typename RootType>
std::vector<typename IComposite<BaseType, RootType>::component*> IComposite<BaseType, RootType>::GetChild() const
{
	return m_child;
}

template <typename BaseType, typename RootType>
std::vector<typename IComposite<BaseType, RootType>::component*> IComposite<BaseType, RootType>::GetAllTree()
{
	std::vector<component*> result;
	AddFullTree(result);
	return result;
}


template <typename BaseType, typename RootType>
void IComposite<BaseType, RootType>::Add(component* child)
{
	m_child.push_back(child);
}

template <typename BaseType, typename RootType>
void IComposite<BaseType, RootType>::Remove(component* child)
{
	auto it = std::find(m_child.begin(), m_child.end(), child);
	if (it == m_child.end())
		throw std::out_of_range(" do not exist");
	m_child.erase(it);
}

template <typename BaseType, typename RootType>
void IComposite<BaseType, RootType>::AddFullTree(std::vector<component*>& vec)
{
	component::AddFullTree(vec);
	for (auto& child : m_child)
		child->AddFullTree(vec);
}


template <typename BaseType, typename RootType>
bool IComposite<BaseType, RootType>::IsInSubTree(component* component) const
{
	if (component::IsInSubTree(component))
		return true;
	for (auto& child : m_child)
	{
		if (child->IsInSubTree(component))
			return true;
	}
	return false;
}

template <typename BaseType, typename RootType>
bool IComposite<BaseType, RootType>::HasChild(component* component) const
{
	auto it = std::find(m_child.begin(), m_child.end(), component);
	return it != m_child.end();
}

template <typename BaseType, typename RootType>
size_t IComposite<BaseType, RootType>::Size() const
{
	return m_child.size();
}

template<typename BaseType, typename RootType = BaseType>
std::vector<BaseType*> IComposite<BaseType, RootType>::ChildAsBase()
{
	auto children = GetChild();
	std::vector<BaseType*> result;
	for (auto& child : children)
		result.push_back(child->AsBase());
	return result;
}

template<typename BaseType, typename RootType = BaseType>
std::vector<BaseType*> IComposite<BaseType, RootType>::AllTreeAsBase()
{
	auto children = GetAllTree();
	std::vector<BaseType*> result;
	for (auto& child : children)
		result.push_back(child->AsBase());
	return result;
}

// IRoot Implementation

template <typename BaseType, typename RootType>
IRoot<BaseType, RootType>::IRoot() : composite()
{
}

template <typename BaseType, typename RootType>
const IRoot<BaseType, RootType>* IRoot<BaseType, RootType>::GetRoot() const
{
	return this;
}

template <typename BaseType, typename RootType>
IRoot<BaseType, RootType>* IRoot<BaseType, RootType>::GetRoot()
{
	return this;
}

template <typename BaseType, typename RootType>
void IRoot<BaseType, RootType>::AddFullTree(std::vector<component*>& vec)
{
	IComposite<BaseType, RootType>::AddFullTree(vec);
}

// ILeaf Implementation

template <typename BaseType, typename RootType>
ILeaf<BaseType, RootType>::ILeaf(composite* owner) : component(owner)
{
}

	}