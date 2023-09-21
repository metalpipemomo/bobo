#pragma once

#include "bobopch.h"

#include "Scene.h"

namespace Bobo
{
	class BOBO_API SceneManager
	{
	public: 
		static SceneManager& GetInstance()
		{
			static SceneManager* instance = new SceneManager();
			return *instance;
		}
		void RegisterScene(Scene* newScene);
		void LoadScene(size_t sceneIndex);
		void UpdateLoadedScene();
		void FixedUpdateLoadedScene();
	private:
		SceneManager();

		std::vector<Scene*> m_RegisteredScenes;
		Scene* p_CurrentScene;
	};
}