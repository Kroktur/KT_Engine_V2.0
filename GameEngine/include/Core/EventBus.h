#pragma once
#include <vector>
#include <stdexcept>
#include <memory>
#include <Tools/Rtti.h>
#include <Tools/Typelist.h>
#include "Tools/PrimaryTypeTrait.h"

//TODO stop require and use concept
namespace KT
{
	struct IEvent
	{
		virtual ~IEvent() = default;
	};

	struct IListener
	{
		virtual ~IListener() = default;
	};

	template<typename crtp,typename list>
	struct AutoManageListener;

	template<typename crtp,typename ...EventType> requires(... && KT::is_base_of_v<IEvent,EventType>)
	struct AutoManageListener<crtp,typelist<EventType...>> : public IListener
	{
		AutoManageListener();
		 ~AutoManageListener() override;
	};

	template<typename EventType> requires KT::is_base_of_v<IEvent, EventType>
	class EventBusManager;

	template<typename EventType> requires KT::is_base_of_v<IEvent,EventType>
	struct IEventBus
	{
		friend EventBusManager<EventType>;
		virtual ~IEventBus();
	protected:
		virtual void Notify(const EventType& event) = 0;
		IEventBus(int id);

	private:
		int GetID() const;
		int m_id;
	};


	template<typename Type, typename EventType> requires KT::is_base_of_v<IEvent, EventType> &&  KT::is_base_of_v<IListener,Type>
	struct ConcreteEventBus : public IEventBus<EventType>
	{
		friend EventBusManager<EventType>;
		ConcreteEventBus();
		using CallBack = void(Type::*)(const EventType& event);
	private:
		void AddCallBack(CallBack cb);

		void AddListener(Type* listener);

		void RemoveListener(Type* listener);

		void Notify(const EventType& event) override final;
		std::vector<Type*> m_listeners;
		std::vector<CallBack> m_callBack;
	};





	template<typename EventType> requires KT::is_base_of_v<IEvent, EventType>
	class EventBusManager
	{
	public:
		template<typename Type> requires  KT::is_base_of_v<IListener, Type>
		using CallBack = void(Type::*)(const EventType& event);

		template<typename Type>  requires  KT::is_base_of_v<IListener, Type>
		static void AddListener(Type* listener);

		template<typename Type>  requires  KT::is_base_of_v<IListener, Type>
		static void RemoveListener(Type* listener);

		template<typename Type>  requires  KT::is_base_of_v<IListener, Type>
		static void AddCallBack(CallBack<Type> cb);

		static void Notify(const EventType& event);

	private:
		static std::vector<std::unique_ptr<IEventBus<EventType>>> m_listeners;
	};

	template<typename EventType> requires KT::is_base_of_v<IEvent, EventType>
	std::vector<std::unique_ptr<IEventBus<EventType>>> EventBusManager<EventType>::m_listeners;



	template <typename crtp, typename ... EventType> requires(... && KT::is_base_of_v<IEvent, EventType>)
	AutoManageListener<crtp, typelist<EventType...>>::AutoManageListener()
	{
		(..., EventBusManager<EventType>::template AddListener<crtp>(static_cast<crtp*>(this)));
	}

	template <typename crtp, typename ... EventType> requires(... && KT::is_base_of_v<IEvent, EventType>)
	AutoManageListener<crtp, typelist<EventType...>>::~AutoManageListener()
	{
		(..., EventBusManager<EventType>::template RemoveListener<crtp>(static_cast<crtp*>(this)));
	}

	template <typename EventType> requires KT::is_base_of_v<IEvent, EventType>
	IEventBus<EventType>::~IEventBus() = default;

	template <typename EventType> requires KT::is_base_of_v<IEvent, EventType>
	IEventBus<EventType>::IEventBus(int id):m_id(id)
	{}

	template <typename EventType> requires KT::is_base_of_v<IEvent, EventType>
	int IEventBus<EventType>::GetID() const
	{ return m_id; }

	template <typename Type, typename EventType> requires KT::is_base_of_v<IEvent, EventType> && KT::is_base_of_v<
		IListener, Type>
	ConcreteEventBus<Type, EventType>::ConcreteEventBus(): IEventBus<EventType>(RTTI::GetTypeId<Type>())
	{}

	template <typename Type, typename EventType> requires KT::is_base_of_v<IEvent, EventType> && KT::is_base_of_v<
		IListener, Type>
	void ConcreteEventBus<Type, EventType>::AddCallBack(CallBack cb)
	{
		auto it = std::find(m_callBack.begin(), m_callBack.end(), cb);
		if (it == m_callBack.end())
			m_callBack.push_back(cb);
	}

	template <typename Type, typename EventType> requires KT::is_base_of_v<IEvent, EventType> && KT::is_base_of_v<
		IListener, Type>
	void ConcreteEventBus<Type, EventType>::AddListener(Type* listener)
	{
		auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
		if (it != m_listeners.end())
			throw std::runtime_error("Listener already added");
		m_listeners.push_back(listener);
	}

	template <typename Type, typename EventType> requires KT::is_base_of_v<IEvent, EventType> && KT::is_base_of_v<
		IListener, Type>
	void ConcreteEventBus<Type, EventType>::RemoveListener(Type* listener)
	{
		auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
		if (it == m_listeners.end())
			throw std::runtime_error("Listener not found");
		m_listeners.erase(it);
	}

	template <typename Type, typename EventType> requires KT::is_base_of_v<IEvent, EventType> && KT::is_base_of_v<
		IListener, Type>
	void ConcreteEventBus<Type, EventType>::Notify(const EventType& event)
	{
		for (auto* listner : m_listeners)
		{
			if (!listner)
				continue;
			for (auto& cb : m_callBack)
			{
				if (cb)
					(listner->*cb)(event);
			}
		}
	}

	template <typename EventType> requires KT::is_base_of_v<IEvent, EventType>
	template <typename Type> requires KT::is_base_of_v<IListener, Type>
	void EventBusManager<EventType>::AddListener(Type* listener)
	{
		auto it = std::find_if(m_listeners.begin(), m_listeners.end(),
		                       [](const std::unique_ptr<IEventBus<EventType>>& l) { return l->GetID() == RTTI::GetTypeId<Type>(); });

		if (it == m_listeners.end())
		{
			auto linker = std::make_unique<ConcreteEventBus<Type, EventType>>();
			linker->AddListener(listener);
			m_listeners.push_back(std::move(linker));
		}
		else
		{
			ConcreteEventBus<Type, EventType>* linker = static_cast<ConcreteEventBus<Type, EventType>*>((*it).get());
			linker->AddListener(listener);
		}
	}

	template <typename EventType> requires KT::is_base_of_v<IEvent, EventType>
	template <typename Type> requires KT::is_base_of_v<IListener, Type>
	void EventBusManager<EventType>::RemoveListener(Type* listener)
	{
		auto it = std::find_if(m_listeners.begin(), m_listeners.end(),
		                       [&](const std::unique_ptr<IEventBus<EventType>>& l) { return l->GetID() == RTTI::GetTypeId<Type>(); });
		if (it != m_listeners.end())
		{
			ConcreteEventBus<Type, EventType>* linker = static_cast<ConcreteEventBus<Type, EventType>*>((*it).get());
			linker->RemoveListener(listener);
		}
		else
		{
			throw std::runtime_error("ConcreteEventBus not found");
		}
	}

	template <typename EventType> requires KT::is_base_of_v<IEvent, EventType>
	template <typename Type> requires KT::is_base_of_v<IListener, Type>
	void EventBusManager<EventType>::AddCallBack(CallBack<Type> cb)
	{
		auto it = std::find_if(m_listeners.begin(), m_listeners.end(),
		                       [&](const std::unique_ptr<IEventBus<EventType>>& l) { return l->GetID() == RTTI::GetTypeId<Type>(); });
		if (it != m_listeners.end())
		{
			ConcreteEventBus<Type, EventType>* linker = static_cast<ConcreteEventBus<Type, EventType>*>((*it).get());
			linker->AddCallBack(cb);
		}
		else
		{
			throw std::runtime_error("ConcreteEventBus not found");
		}
	}

	template <typename EventType> requires KT::is_base_of_v<IEvent, EventType>
	void EventBusManager<EventType>::Notify(const EventType& event)
	{
		for (auto& listener : m_listeners)
			listener->Notify(event);
	}
}
