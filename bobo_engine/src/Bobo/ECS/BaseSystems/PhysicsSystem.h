#pragma once

#include "bobopch.h"

#include "Bobo/ECS/System.h"
#include "Bobo/ECS/BaseComponents/Transform.h"

namespace Bobo
{
	class BOBO_API PhysicsSystem : public System
	{
		virtual ~PhysicsSystem();

		void Update() override;

		void FixedUpdate() override;

	public:
		PhysicsSystem(std::string inScene) : System(inScene) {};
		void RegisterRigidbody(Transform* t);

	private:
		std::vector<Transform*> m_Rigidbodies;
	};
}