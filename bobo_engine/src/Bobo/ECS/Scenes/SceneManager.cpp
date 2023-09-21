#include "bobopch.h"

#include "SceneManager.h"

namespace Bobo
{
	SceneManager::SceneManager()
	{
		p_CurrentScene = nullptr;
	}

	void SceneManager::RegisterScene(Scene* newScene)
	{
		auto it = std::find(m_RegisteredScenes.begin(), m_RegisteredScenes.end(), newScene);

		if (it != m_RegisteredScenes.end()) {
			BOBO_ERROR("Failed to Register Scene {} in SceneManager, Scene already Registered", m_RegisteredScenes.size());
			return;
		}

		BOBO_INFO("Registered Scene {} in SceneManager", m_RegisteredScenes.size());
		m_RegisteredScenes.push_back(newScene);
	}

	void SceneManager::LoadScene(size_t sceneIndex)
	{
		if (sceneIndex > m_RegisteredScenes.size())
		{
			BOBO_ERROR("Attempted to Load Scene at SceneIndex {} Failed, Index Out of Bounds", sceneIndex);
			return;
		}
		BOBO_INFO("Loaded Scene {} in SceneManager", sceneIndex);
		p_CurrentScene = m_RegisteredScenes.at(sceneIndex);
	}

	void SceneManager::UpdateLoadedScene()
	{
		if (p_CurrentScene == nullptr) 
		{
			BOBO_WARN("No Scene Loaded to Update");
			return;
		};
		p_CurrentScene->UpdateSystems();
	}

	void SceneManager::FixedUpdateLoadedScene()
	{
		if (p_CurrentScene == nullptr) 
		{
			BOBO_WARN("No Scene Loaded to FixedUpdate");
			return;
		};
		p_CurrentScene->FixedUpdateSystems();
	}
}