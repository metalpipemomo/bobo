#include "bobopch.h"

#include "SceneManager.h"
#include "../../Time.h"

namespace Bobo
{
	SceneManager::SceneManager()
	{
		p_ActiveScene = nullptr;
		p_Physics = new PhysicsSystem();
		p_Renderer = new RendererSystem();
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

	void SceneManager::LoadScene(const unsigned int& sceneIndex)
	{
		if (sceneIndex > m_RegisteredScenes.size())
		{
			BOBO_ERROR("Attempted to Load Scene at SceneIndex {} Failed, Index Out of Bounds", sceneIndex);
			return;
		}
		BOBO_INFO("Loaded Scene {} in SceneManager", sceneIndex);
		p_ActiveScene = m_RegisteredScenes.at(sceneIndex);
		p_Physics->SetActiveScene(p_ActiveScene);
		p_Renderer->SetActiveScene(p_ActiveScene);

		Time::GetInstance()->ResetTimeSinceLoadScene();
	}

	void SceneManager::UpdateLoadedScene()
	{
		if (p_ActiveScene == nullptr) 
		{
			BOBO_WARN("No Scene Loaded to Update");
			return;
		};
		p_ActiveScene->UpdateSystems();
		p_Physics->Update();
		p_Renderer->Update();
	}

	void SceneManager::FixedUpdateLoadedScene()
	{
		if (p_ActiveScene == nullptr) 
		{
			BOBO_WARN("No Scene Loaded to FixedUpdate");
			return;
		};
		p_ActiveScene->FixedUpdateSystems();
		p_Physics->FixedUpdate();
	}
}