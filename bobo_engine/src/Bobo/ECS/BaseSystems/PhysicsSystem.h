#pragma once
#include "bobopch.h"
#include "Bobo/ECS/System.h"
#include "Bobo/ECS/BaseComponents/RigidBody.h"
#include "Bobo/ECS/BaseComponents/Collider.h"

namespace Bobo
{
	 struct Collision // this should probably be put into its own header file.
	 {
	 	RigidBody* ObjA;
	 	RigidBody* ObjB;
	 	CollisionPoints points;
	 };

	class PhysicsSystem : public System
	{
		void Update() override;

		void FixedUpdate() override 
		{
			CalculateDeltaTime();
			float dt = CalculateDeltaTime();
			for (RigidBody* rb : this->m_PhysicsObjects)
			{
				//checks if the rigidbody is static, then calculates and applys forces to the object.
				if (!rb->m_IsStatic) 
				{
					rb->m_Force = rb->m_Force + (m_Gravity * rb->m_Mass);
					rb->m_Velocity = rb->m_Velocity + ((rb->m_Force / rb->m_Mass) * dt);
					rb->p_Transform->position = rb->p_Transform->position + (rb->m_Velocity * dt);
					//BOBO_INFO("Velocity Y: " + std::to_string(obj->velocity.GetY()));
					//BOBO_INFO("Position Y: " + std::to_string((obj->transform->position).GetY()));
				}
			}
			ResolveCollisions(dt);
		};

		//this will have to be moved out of this class
		float CalculateDeltaTime()
		{
			// Get the current time
			m_CurrentTime = std::chrono::high_resolution_clock::now();

			// Calculate the time difference between the current and previous frames
			std::chrono::duration<float> deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(m_CurrentTime - m_PreviousTime);

			// Update the previous time for the next frame
			m_PreviousTime = m_CurrentTime;

			// Return the delta time in seconds
			return deltaTime.count();
		}

	void ResolveCollisions(float dt) 
	{
 		std::vector<Collision> collisions;

 		for (RigidBody* a : m_PhysicsObjects)
 		{
 			for (RigidBody* b : m_PhysicsObjects)
 			{
 				if (a == b) break;

 				if (!a->p_Collider || !b->p_Collider) continue;

 				CollisionPoints points = a->p_Collider->TestCollision(a->p_Transform, b->p_Collider, b->p_Transform);

 				if (points.HasCollision)
 				{
 					BOBO_INFO("***********************************COLLISION OCCURED************************************");
 					collisions.emplace_back(Collision{ a, b, points });
 				}
 			}
 		}

		//add code to resolve collisions after theyve been detected.
 	}

	void SolveCollision(std::vector<Collision> collisions) 
	{
		//move the source code of this into its own file. 
		//create a base solver class that has a function called SolveCollision(Collision collision);
		// it will also have specific functions (ie. SolveSphereSphereCollision(), SolvePlaneSphereCollision(), etc..)
		//this function will hand all of the collision data to this solver class
		//
	}


	private:
		std::vector<RigidBody*> m_PhysicsObjects;
		glm::vec3 m_Gravity;


		//This will have to be removed with the CalculateDeltaTime function aswell;
		std::chrono::high_resolution_clock::time_point m_PreviousTime;
		std::chrono::high_resolution_clock::time_point m_CurrentTime;
	};



}