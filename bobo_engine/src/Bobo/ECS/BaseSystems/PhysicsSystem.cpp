#include "bobopch.h"

#include "PhysicsSystem.h"

namespace Bobo
{
	PhysicsSystem::~PhysicsSystem()
	{

	}

	void PhysicsSystem::Update()
	{
		BOBO_INFO("PhysicsSystem ({}) Update: Registered Rigidbodies = {}", m_InScene, m_Rigidbodies.size());
	}

	void PhysicsSystem::FixedUpdate()
	{
		BOBO_INFO("PhysicsSystem ({}) FixedUpdate: Registered Rigidbodies = {}", m_InScene, m_Rigidbodies.size());
	}

	void PhysicsSystem::RegisterRigidbody(Transform* t)
	{
		if (t == nullptr) 
		{
			BOBO_WARN("Failed to Register Rigidbody, Provided Rigidbody is nullptr");
			return;
		}
		BOBO_INFO("Added {} to PhysicsSystem", typeid(*t).name());
		m_Rigidbodies.push_back(t);
	}
}