#pragma once
#include "Tools/Chrono.h"

namespace KT
{

	template< typename WindowType, typename Camera>
	class ISceneManager;

	//Base of All Scene
	// Need Improvement and Make it more independence from the API
	template< typename WindowType, typename Camera>
	class ISceneBase
	{
	public:
		// add more safety and need a manager to set member
		friend ISceneManager<WindowType, Camera>;

		template<typename ...Args>
		ISceneBase(const Chrono<float>::Time& refreshTime, Args&&... args) : m_window(nullptr), m_needChange(false), m_nextSceneIndex(int{}), m_refreshTime(refreshTime), m_cam(std::make_unique<Camera>(std::forward<Args>(args)...))
		{
		}
		virtual ~ISceneBase() = default;
		int GetNextSceneIndex()
		{
			if (!NeedSceneToChange())
				throw std::runtime_error("change dont need to be change");
			return m_nextSceneIndex;
		}

		bool NeedSceneToChange() const
		{
			return m_needChange;
		}
		virtual void ProcessInput() = 0;
		virtual void Update(const float& deltatime) = 0;
		virtual void Render(const float& alpha) = 0;
		WindowType& GetWindow() const
		{
			return *m_window;
		}
		Chrono<float>::Time GetRefreshTime() const
		{
			return m_refreshTime;
		}
		void SetRefreshTime(const Chrono<float>::Time& other)
		{
			m_refreshTime = other;
		}
		Camera* GetCamera()
		{
			return m_cam.get();
		}


	protected:
		void ChangeScene(const int& index)
		{
			m_needChange = true;
			m_nextSceneIndex = index;
		}
		void ChangeSceneDone()
		{
			if (!m_needChange)
				throw std::runtime_error("wtf is going on");
			m_needChange = false;
		}
	private:
		// member function that only the SceneManager can use
		void SetWindow(WindowType* win)
		{
			m_window = win;
		}
		virtual void Init() = 0;
		virtual void Destroy() = 0;
		WindowType* m_window;
		std::unique_ptr<Camera> m_cam;
		int m_nextSceneIndex;
		bool m_needChange;
		Chrono<float>::Time m_refreshTime;
	};

}