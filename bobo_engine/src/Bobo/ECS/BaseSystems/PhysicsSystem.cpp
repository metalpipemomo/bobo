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
		// BOBO_INFO("Phyzics Updoot");
	}

	void PhysicsSystem::FixedUpdate()
	{
		// BOBO_INFO("Phyzics fixed Updoot");

		float dt = 0.16;
		for (auto entity : m_PhysicsObjects)
		{
			//checks if the rigidbody is static, then calculates and applys forces to the object.
			if (!entity.second->m_IsStatic)
			{
				auto transform = p_ActiveScene->GetComponent<Transform>(entity.first);

				entity.second->m_Force += (m_Gravity * entity.second->m_Mass);
				entity.second->m_Velocity += ((entity.second->m_Force / entity.second->m_Mass) * dt);
				transform->position += transform->position + (entity.second->m_Velocity * dt);


				//BOBO_INFO("Velocity Y: " + std::to_string(obj->velocity.GetY()));
				//BOBO_INFO("Position Y: " + std::to_string((obj->transform->position).GetY()));
			}
		}
		ResolveCollisions(dt);
		// You can do something like:
		// This call is kind of expensive, so probably dont call on frame update
		// it returns a map of entities and component type
		// do varname[index]->first for entity, varname[index]->second for rigidbody
	}

	void PhysicsSystem::ResolveCollisions(float dt)
	{
		std::vector<Collision> collisions;

		for (auto a : m_PhysicsObjects)
		{
			for (auto b : m_PhysicsObjects)
			{
				if (a.first == b.first) break;

				if (!a.second->p_Collider || !b.second->p_Collider) continue;

				auto transformA = p_ActiveScene->GetComponent<Transform>(a.first);
				auto transformB = p_ActiveScene->GetComponent<Transform>(b.first);

				CollisionPoints points = a.second->p_Collider->TestCollision(transformA, b.second->p_Collider, transformB);

				if (points.HasCollision)
				{
					BOBO_INFO("***********************************COLLISION OCCURED************************************");
					RigidBody* x = a.second;
					Collision col;
					col.transformA = transformA;
					col.transformB = transformB;
					collisions.emplace_back(col);
				}
			}
		}

		//add code to resolve collisions after theyve been detected.
		SolveCollision(collisions);
	}


	void PhysicsSystem::SolveCollision(std::vector<Collision> collisions)
	{
		for (Collision col : collisions) 
		{
			CollisionSolver::SolveCollision(col);
		}
		//move the source code of this into its own file. 
		//create a base solver class that has a function called SolveCollision(Collision collision);
		// it will also have specific functions (ie. SolveSphereSphereCollision(), SolvePlaneSphereCollision(), etc..)
		//this function will hand all of the collision data to this solver class
		//
	}


	void PhysicsSystem::SetActiveScene(Scene* scene)
	{
		p_ActiveScene = scene;
		m_PhysicsObjects = p_ActiveScene->GetComponentsOfType<RigidBody>();
		BOBO_INFO("Fixedzics Updoot");
	}
}