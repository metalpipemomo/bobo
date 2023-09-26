#pragma once

#include "Scene.h"
#include "Bobo/ECS/BaseSystems/PhysicsSystem.h"
#include "Bobo/ECS/BaseSystems/RendererSystem.h"

namespace Bobo
{
	class BOBO_API SceneManager
	{
	public: 
		static SceneManager* GetInstance()
		{
			static SceneManager instance = SceneManager();
			return &instance;
		}
		void RegisterScene(Scene* newScene);
		void LoadScene(const unsigned int& sceneIndex);
		void UpdateLoadedScene();
		void FixedUpdateLoadedScene();
	private:
		SceneManager();
		std::vector<Scene*> m_RegisteredScenes;
		Scene* p_ActiveScene;
		PhysicsSystem* p_Physics;
		RendererSystem* p_Renderer;
	};
}