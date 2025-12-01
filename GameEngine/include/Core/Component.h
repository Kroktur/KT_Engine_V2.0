#pragma once
#include <stdexcept>
#include <memory>
#include <map>
#include <Tools/Rtti.h>
namespace KT
{
// ComponentContainer to store components of a specific type using a map with unique IDs
// Need improvement with a component pool to avoid fragmentation
template<typename type>
class ComponentContainer
{
public:
	template<typename... Args>
	static type* AddComponent(int id, const Args&... args);
	static type* GetComponent(int id);
	static void RemoveComponent(int id);
private:
	// map to store components with unique Instance IDs
	static std::map<int, std::unique_ptr<type>> m_components;
};

template <typename type>
template <typename ... Args>
type* ComponentContainer<type>::AddComponent(int id, const Args&... args)
{
	// add a component only if it doesn't already exist and return a pointer to it
	auto it = m_components.find(id);
	if (it == m_components.end())
		m_components[id] = std::make_unique<type>(args...);
	else
		throw std::runtime_error("component Already Exist");
	return m_components[id].get();
}

template <typename type>
type* ComponentContainer<type>::GetComponent(int id)
{
	// get a component by its unique Instance ID
	auto it = m_components.find(id);
	return (it != m_components.end()) ? it->second.get() : nullptr;
}

template <typename type>
void ComponentContainer<type>::RemoveComponent(int id)
{
	// remove a component by its unique Instance ID
	auto it = m_components.find(id);
	if (it == m_components.end())
		throw std::runtime_error("Doesn't exists");
	m_components.erase(it);
}

template<typename type>
inline std::map<int, std::unique_ptr<type>> ComponentContainer<type>::m_components = std::map<int, std::unique_ptr<type>>{};

// Base class to manage components entity need to inherit from this class to have components
//need improvement with an id pool to avoid fragmentation
template<typename Entity>
struct ComponentManager
{
public:
	virtual ~ComponentManager();
	template<typename type, typename... Args>
	type* AddComponent(const Args&... args);
	template<typename type>
	type* GetComponent();
	template<typename type>
	void RemoveComponent();
	template<typename type, typename... Args>
	type* ReplaceComponent(const Args&... args);
	template<typename type>
	bool HasComponent() const;
protected:
	// only Entity can create a ComponentManager
	ComponentManager(Entity* owner);
private:
	std::map<int, int> m_idsComponent;
	Entity* m_ownen;
};

template <typename Entity>
ComponentManager<Entity>::~ComponentManager() = default;

template <typename Entity>
template <typename type, typename ... Args>
type* ComponentManager<Entity>::AddComponent(const Args&... args)
{
	// add a component only if it doesn't already exist and return a pointer to it
	auto it = m_idsComponent.find(RTTI::GetTypeId<type>());
	if (it != m_idsComponent.end())
		throw std::runtime_error("Component Already Exists");
	auto id = RTTI::GetInstanceId<type>();

	m_idsComponent[RTTI::GetTypeId<type>()] = id;
	// set the owner of the component to the entity that owns this component manager
	ComponentContainer<type>::AddComponent(id, args...)->SetOwner(m_ownen);

	return GetComponent<type>();
}

template <typename Entity>
template <typename type>
type* ComponentManager<Entity>::GetComponent()
{
	// get a component by its type
	auto it = m_idsComponent.find(RTTI::GetTypeId<type>());
	return (it != m_idsComponent.end()) ? ComponentContainer<type>::GetComponent(it->second) : nullptr;
}

template <typename Entity>
template <typename type>
void ComponentManager<Entity>::RemoveComponent()
{
	// remove a component by its type
	auto it = m_idsComponent.find(RTTI::GetTypeId<type>());
	if (it == m_idsComponent.end())
		throw std::runtime_error("Doesn't exists");
	ComponentContainer<type>::RemoveComponent(it->second);
	m_idsComponent.erase(it);
}

template <typename Entity>
template <typename type, typename ... Args>
type* ComponentManager<Entity>::ReplaceComponent(const Args&... args)
{
	// remove a component if it exists and add a new one
	RemoveComponent<type>();
	return AddComponent<type>(args...);
}

template <typename Entity>
template <typename type>
bool ComponentManager<Entity>::HasComponent() const
{
	auto it = m_idsComponent.find(RTTI::GetTypeId<type>());
	return it != m_idsComponent.end();
}

template <typename Entity>
ComponentManager<Entity>::ComponentManager(Entity* owner): m_ownen(owner)
{}

// Base Component class that other components will inherit from
template<typename Entity>
class Component
{
public:
	// only ComponentManager can create a Component
	friend ComponentManager<Entity>;
	Component() : m_owner(nullptr) {}
protected:
	Entity* GetOwner() { return m_owner; }
private:
	// only ComponentManager can set the owner of the component
	void SetOwner(Entity* owner) { m_owner = owner; }
	
	Entity* m_owner;
};
}