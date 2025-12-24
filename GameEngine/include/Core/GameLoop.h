#pragma once
#include <map>
#include "Tools/Chrono.h"
#include "ResourcesManager.h"
#include "Core/Scene.h"
namespace KT
{

	template<typename WindowType, typename Camera>
	class ISceneManager
	{
	public:
		ISceneManager(WindowType& window);
		virtual ~ISceneManager()
		{
			for (auto& pair : m_sceneMap)
			{
				auto& scene = pair.second;
				scene->Destroy();
			}
		}
		void AddScene(const int& index, std::unique_ptr<ISceneBase<WindowType, Camera>> scene, const bool& start = false);
		void Run(const Chrono<float>::Time& fixedTime);
		ISceneBase<WindowType, Camera>& GetScene(const int& index) const;

	protected:
		virtual bool LoopCondition() = 0;
		virtual void BeginRender() = 0;
		virtual void EndRender() = 0;
		WindowType& m_window;
	private:
		void ChangeSceneIfNeeded();
		void VerifyIndex(const int& index);
		std::map<int, std::unique_ptr<ISceneBase<WindowType, Camera>>> m_sceneMap;
		int m_currentIndex;
	};

	template <typename WindowType, typename Camera>
	ISceneManager<WindowType, Camera>::ISceneManager(WindowType& window) :m_currentIndex(0), m_sceneMap(), m_window(window)
	{
	}

	template <typename WindowType, typename Camera>
	void ISceneManager<WindowType, Camera>::AddScene(const int& index,
		std::unique_ptr<ISceneBase<WindowType, Camera>> scene, const bool& start)
	{
		// add the scene 
		if (m_sceneMap.find(index) != m_sceneMap.end())
			throw std::out_of_range("Already registered");
		m_sceneMap[index] = std::move(scene);
		// SetGlobalFIlePath the scene with all needed
		auto& Scene = GetScene(index);
		Scene.SetWindow(&m_window);
		Scene.Init();
		if (start)
			m_currentIndex = index;
	}

	template <typename WindowType, typename Camera>
	void ISceneManager<WindowType, Camera>::Run(const Chrono<float>::Time& fixedTime)
	{
		// simple verification
		if (m_sceneMap.empty())
			throw std::out_of_range("SceneEmpty");
		//initiate the game update clock
		const Chrono clock;
		float previous = clock.GetElapsedTime().AsMilliSeconds();
		//first version of frameRate limitation
		float renderFrameDt = clock.GetElapsedTime().AsMilliSeconds();
		// dt
		const float fixTick = fixedTime.AsMilliSeconds();
		// lag between 2 render 
		auto lag = 0.0f;
		while (LoopCondition())
		{
			// lag incrementation 
			const float startframetime = clock.GetElapsedTime().AsMilliSeconds();
			const auto elapsed = startframetime - previous;
			previous = startframetime;
			lag += elapsed;
			//set scene
			ChangeSceneIfNeeded();
			auto& Scene = GetScene(m_currentIndex);
			//Input 
			Scene.ProcessInput();

			// need improvement if update is too heavy and create lag maybe separate physics and update for animation 
			while (lag >= fixTick)
			{
				//Update 
				Scene.Update(fixedTime.AsSeconds());
				lag -= fixTick;
			}
			//Render if the frame rate is in range need more test 
			if (clock.GetElapsedTime().AsMilliSeconds() - renderFrameDt >= Scene.GetRefreshTime().AsMilliSeconds())
			{
				renderFrameDt = clock.GetElapsedTime().AsMilliSeconds();
				const float alpha = lag / fixTick;
				// if interpolation
				BeginRender();
				Scene.Render(alpha);
				EndRender();
			}
		}
	}

	template <typename WindowType, typename Camera>
	ISceneBase<WindowType, Camera>& ISceneManager<WindowType, Camera>::GetScene(const int& index) const
	{
		// get a scene from the map
		return *m_sceneMap.at(index).get();
	}

	template <typename WindowType, typename Camera>
	void ISceneManager<WindowType, Camera>::ChangeSceneIfNeeded()
	{
		auto& scene = m_sceneMap[m_currentIndex];
		// specific private verification for change scene 
		if (!scene->NeedSceneToChange())
			return;
		
		VerifyIndex(scene->GetNextSceneIndex());
		scene->ChangeSceneDone();
	}

	template <typename WindowType, typename Camera>
	void ISceneManager<WindowType, Camera>::VerifyIndex(const int& index)
	{
		// simple range check
		auto it = m_sceneMap.find(index);
		if (it == m_sceneMap.end())
			throw std::out_of_range("index out of range");
		m_currentIndex = index;
	}

}
