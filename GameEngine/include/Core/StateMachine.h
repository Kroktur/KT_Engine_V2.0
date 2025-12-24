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

		virtual void OnEnter() {}
		virtual void OnExit() {}
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
		StateMachine(std::unique_ptr<IState<Entity>> state, size_t maxcount = 1)
			: m_currentState(), m_maxcount(maxcount)
		{
			m_currentState.push_back(std::move(state));
			m_currentState.back()->OnEnter();
		}

		void ProcessInput()
		{
			m_currentState.back()->ProcessInput();
		}

		void Update(const float& dt)
		{
			m_currentState.back()->Update(dt);
		}

		void Render(const float& alpha)
		{
			m_currentState.back()->Render(alpha);
		}

		void ChangeState()
		{
			auto nextState = m_currentState.back()->GetNextState();
			if (!nextState)
				return;

			m_currentState.back()->OnExit();

			m_currentState.push_back(std::move(nextState));

			m_currentState.back()->OnEnter();

			if (m_currentState.size() > m_maxcount)
				m_currentState.erase(m_currentState.begin());
		}

	private:
		std::vector<std::unique_ptr<IState<Entity>>> m_currentState;
		size_t m_maxcount;
	};
}