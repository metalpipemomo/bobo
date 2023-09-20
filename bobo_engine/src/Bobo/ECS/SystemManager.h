#pragma once

#include "System.h"
#include "Scene.h"
#include "BaseSystems/RendererSystem.h"

namespace Bobo
{
	class SystemManager
	{
	public: 
		static SystemManager& GetInstance()
		{
			static SystemManager* instance = new SystemManager();
			return *instance;
		}

		void SetCurrentScene(Scene* newScene) {
			p_CurrentScene = newScene;
			SetSceneRendererSystem(newScene);
			SetScenePhysicsSystem(newScene);
		}

		void UpdateAll()
		{
			for (System* sys : p_CurrentScene->GetSystems())
			{
				sys->Update();
			}
		}

		void FixedUpdateAll()
		{
			for (System* sys : p_CurrentScene->GetSystems())
			{
				sys->FixedUpdate();
			}
		}
	private:
		SystemManager() 
		{
			p_CurrentRendererSystem = nullptr;
			p_CurrentPhysicsSystem = nullptr;
		}
		void SetSceneRendererSystem(Scene* ofScene) {
			p_CurrentRendererSystem = ofScene->GetSystem<RendererSystem>();
			if (p_CurrentRendererSystem == nullptr) {
				BOBO_ERROR("Failed to Set Renderer System");
			}
		}
		void SetScenePhysicsSystem(Scene* ofScene) {
			p_CurrentPhysicsSystem = ofScene->GetSystem<PhysicsSystem>();
			if (p_CurrentPhysicsSystem == nullptr) {
				BOBO_ERROR("Failed to Set Physics System");
			}
		}

		Scene* p_CurrentScene;
		RendererSystem* p_CurrentRendererSystem;
		PhysicsSystem* p_CurrentPhysicsSystem;
	};
}