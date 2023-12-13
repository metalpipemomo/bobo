#pragma once

#include "Scene.h"

class SceneManager
{
public:
	static void Init()
	{
		GetInstance();
	}

	static void CreateScene(const std::string& name)
	{
		auto sm = GetInstance();
		Scene* scene = new Scene(name);
		sm->m_RegisteredScenes.insert({ name, scene });
		if (sm->p_ActiveScene == nullptr)
		{
			sm->p_ActiveScene = scene;
		}
	}

	static void LoadScene(const std::string& name)
	{
		auto sm = GetInstance();
		auto scene = sm->m_RegisteredScenes.find(name);
		if (scene == sm->m_RegisteredScenes.end())
		{
			BOBO_WARN("Failed to load Scene: {}", name);
			return;
		}
		sm->p_ActiveScene = scene->second;

		// Call callbacks associated with the scene changing
		sm->OnSceneChanged();
	}

	static Scene* GetActiveScene()
	{
		auto sm = GetInstance();
		return sm->p_ActiveScene;
	}

	static Scene* GetScene(const std::string& name)
	{
		auto sm = GetInstance();
		auto scene = sm->m_RegisteredScenes.find(name);
		if (scene == sm->m_RegisteredScenes.end())
		{
			BOBO_WARN("Scene: {} does not exist...", name);
			return nullptr;
		}

		return scene->second;
	}

	// calls the active scene's awake function when it first starts
	static void AwakeActiveScene()
	{
		auto sm = GetInstance();
		if (sm->p_ActiveScene == nullptr) return;
		sm->p_ActiveScene->Awake();
	}

	// calls the active scene's update every update
	static void UpdateActiveScene()
	{
		auto sm = GetInstance();
		if (sm->p_ActiveScene == nullptr) return;
		sm->p_ActiveScene->Update();
	}

	// calls the active scene's fixed update every fixed update
	static void FixedUpdateActiveScene()
	{
		auto sm = GetInstance();
		if (sm->p_ActiveScene == nullptr) return;
		sm->p_ActiveScene->FixedUpdate();
	}

	// adds a function to be called when the scene is changed
	static void AddOnSceneChanged(std::string id, std::function<void()> callback)
	{
		auto sm = GetInstance();
		sm->onSceneChangedActions[id] = callback;
	}

	// removes a function which would be called when the scene is changed
	static void RemoveOnSceneChanged(std::string id)
	{
		auto sm = GetInstance();
		sm->onSceneChangedActions.erase(id);
	}

private:
	static SceneManager* GetInstance()
	{
		static SceneManager* instance = new SceneManager();
		return instance;
	}

	SceneManager()
	{
		p_ActiveScene = nullptr;
	}

	void OnSceneChanged()
	{
		for (auto& callback : onSceneChangedActions)
			callback.second();
	}

	std::map<std::string, Scene*> m_RegisteredScenes;
	Scene* p_ActiveScene;
	std::map<std::string, std::function<void()>> onSceneChangedActions;
};