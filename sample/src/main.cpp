#include <iostream>


#include "Core/EventBus.h"


struct LoopedEvent : public KT::IEvent
{
public:
	LoopedEvent() = default;
	void Yes() const
	{
		std::cout << "Looped Event Triggered" << std::endl;
	}
};
struct LoopedEvent2 : public KT::IEvent
{
public:
	LoopedEvent2() = default;
	void Yes() const
	{
		std::cout << "Looped Event 2 Triggered" << std::endl;
	}
};
//
//struct AudioManager : public KT::AutoManageListener<AudioManager,KT::typelist<LoopedEvent,LoopedEvent2>>
//{
//	AudioManager(int totocc) : toto(totocc)
//	{
//		KT::EventBusManager<LoopedEvent>::AddCallBack(&AudioManager::TOTo);
//		KT::EventBusManager<LoopedEvent2>::AddCallBack(&AudioManager::TOTo);
//	}
//	~AudioManager()
//	{
//	}
//	void TOTo(const LoopedEvent& event)
//	{
//		event.Yes();
//	}
//	void TOTo(const LoopedEvent2& event)
//	{
//		event.Yes();
//	}
//	void TOTO2(const LoopedEvent& event)
//	{
//		std::cout << "TOTO2 called: " << toto << std::endl;
//	}
//private:
//	int toto = 4;
//};
//
//
//struct AudioManager2  : public KT::AutoManageListener<AudioManager2, KT::typelist<LoopedEvent>>
//{
//	AudioManager2(int totocc) : toto(totocc)
//	{
//		KT::EventBusManager<LoopedEvent>::AddCallBack(&AudioManager2::TOTo);
//	}
//	~AudioManager2()
//	{
//
//	}
//	void TOTo(const LoopedEvent& event)
//	{
//		event.Yes();
//	}
//	void TOTO2ALLLLA(const LoopedEvent& event)
//	{
//		std::cout << "TOTO2 on other audio called: " << toto << std::endl;
//	}
//private:
//	int toto = 4;
//};
//
//
//
//int main()
//{
//	
//	AudioManager audioMgr(3);
//
//	AudioManager2 audioMgr3(400);
//
//	while (true)
//	{
//		KT::EventBusManager<LoopedEvent2>::Notify(LoopedEvent2{});
//		KT::EventBusManager<LoopedEvent>::Notify(LoopedEvent{});
//
//	}
//
//
//}

int main()
{
	
}