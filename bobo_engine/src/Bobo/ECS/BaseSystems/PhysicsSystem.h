#pragma once

#include "bobopch.h"

#include "Bobo/ECS/System.h"
#include "Bobo/ECS/Scenes/Scene.h"
#include "Bobo/ECS/BaseComponents/RigidBody.h"

namespace Bobo
{
	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem();
		virtual ~PhysicsSystem();
		void Update() override;
		void FixedUpdate() override;
		void SetActiveScene(Scene* scene);
	private:
		Scene* p_ActiveScene;
	};
}