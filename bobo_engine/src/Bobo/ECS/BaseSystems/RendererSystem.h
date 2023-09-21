#pragma once

#include "bobopch.h"

#include "Bobo/ECS/System.h"
#include "Bobo/ECS/Scenes/Scene.h"

namespace Bobo
{
	class RendererSystem : public System
	{
	public:
		RendererSystem();
		~RendererSystem();
		void Update() override;
		void FixedUpdate() override;
		void SetActiveScene(Scene* scene);
	private:
		Scene* p_ActiveScene;
	};
	
}