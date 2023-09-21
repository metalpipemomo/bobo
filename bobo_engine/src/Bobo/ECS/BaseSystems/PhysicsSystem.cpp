#include "bobopch.h"

#include "PhysicsSystem.h"

namespace Bobo
{
	PhysicsSystem::PhysicsSystem()
	{
		p_ActiveScene = nullptr;
	}

	PhysicsSystem::~PhysicsSystem()
	{

	}

	void PhysicsSystem::Update()
	{
		BOBO_INFO("Phyzics Updoot");
	}

	void PhysicsSystem::FixedUpdate()
	{
		// You can do something like:
		// This call is kind of expensive, so probably dont call on frame update
		// it returns a map of entities and component type
		// do varname[index]->first for entity, varname[index]->second for rigidbody
		auto rigidbodies = p_ActiveScene->GetComponentsOfType<RigidBody>();
		BOBO_INFO("Fixedzics Updoot");
	}

	void PhysicsSystem::SetActiveScene(Scene* scene)
	{
		p_ActiveScene = scene;
	}
}