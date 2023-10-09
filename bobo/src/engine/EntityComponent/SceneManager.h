#pragma once

#include "Scene.h"

class SceneManager
{
public:

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
	std::map<std::string, Scene*> m_RegisteredScenes;
	Scene* p_ActiveScene;


};