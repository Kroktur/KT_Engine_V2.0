#pragma once
#include <memory>
#include <vector>

namespace KT
{


	template<typename Entity>
	class IState
	{
	public:
		IState(Entity* entity) :m_entity(entity), m_nextState(nullptr) {}
		virtual ~IState() = default;
		virtual void ProcessInput() = 0;
		virtual void Update(const float& dt) = 0;
		virtual void Render(const float& alpha) = 0;
		template<typename state, typename... Args> requires std::is_base_of_v<IState<Entity>, state>
		void SetNextState(const Args&... args)
		{
			m_nextState = std::make_unique<state>(m_entity, args...);
		}
		std::unique_ptr<IState> GetNextState()
		{
			return std::move(m_nextState);
		}
	protected:
		Entity* m_entity;
	private:
		std::unique_ptr<IState<Entity>> m_nextState;
	};

	template<typename Entity>
	class StateMachine
	{
	public:
		StateMachine(std::unique_ptr<IState<Entity>> state, size_t maxcount = 1) : m_curentState(), m_maxcount(maxcount)
		{
			m_curentState.push_back(std::move(state));
		}
		void ProcessInput()
		{
			m_curentState.back()->ProcessInput();
		}
		void Update(const float& dt)
		{
			m_curentState.back()->Update(dt);
		}
		void Render(const float& alpha)
		{
			m_curentState.back()->Render(alpha);
		}
		void ChangeState()
		{
			auto nextState = m_curentState.back()->GetNextState();
			if (!nextState)
				return;
			m_curentState.push_back(std::move(nextState));
			if (m_curentState.size() > m_maxcount)
				m_curentState.erase(m_curentState.begin());

		}

	private:
		std::vector<std::unique_ptr<IState<Entity>>> m_curentState;
		size_t m_maxcount;
	};
}