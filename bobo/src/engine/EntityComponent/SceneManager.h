#pragma once

#include "Scene.h"

class SceneManager
{
public:
	static SceneManager* GetInstance()
	{
		static SceneManager* instance = new SceneManager();
		return instance;
	}

	void CreateScene(const std::string& name)
	{
		Scene* scene = new Scene(name);
		m_RegisteredScenes.insert({ name, scene });
		if (p_ActiveScene == nullptr)
		{
			p_ActiveScene = scene;
		}
	}

	void LoadScene(const std::string& name)
	{
		auto scene = m_RegisteredScenes.find(name);
		if (scene == m_RegisteredScenes.end())
		{
			BOBO_WARN("Failed to load Scene: {}", name);
			return;
		}
		p_ActiveScene = scene->second;
	}

	Scene* GetActiveScene()
	{
		return p_ActiveScene;
	}

	Scene* GetScene(const std::string& name)
	{
		auto scene = m_RegisteredScenes.find(name);
		if (scene == m_RegisteredScenes.end())
		{
			BOBO_WARN("Scene: {} does not exist...", name);
			return nullptr;
		}

		return scene->second;
	}
	
private:
	SceneManager()
	{
		p_ActiveScene = nullptr;
	}
	std::map<std::string, Scene*> m_RegisteredScenes;
	Scene* p_ActiveScene;


};