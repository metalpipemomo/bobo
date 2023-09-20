#pragma once
#include "bobopch.h"
#include "Bobo/ECS/System.h"
#include "Bobo/ECS/BaseComponents/RigidBody.h"

namespace Bobo
{
	class PhysicsSystem : public System
	{
		void Update() override
		{
		
		};

		void FixedUpdate() override 
		{
			CalculateDeltaTime();
			float dt = CalculateDeltaTime();
			for (RigidBody* rb : this->m_PhysicsObjects)
			{
				//checks if the rigidbody is static, then calculates and applys forces to the object.
				if (!rb->isStatic) 
				{
					rb->force = rb->force + (m_Gravity * rb->mass);
					rb->velocity = rb->velocity + ((rb->force / rb->mass) * dt);
					rb->transform->position = rb->transform->position + (rb->velocity * dt);
					//BOBO_INFO("Velocity Y: " + std::to_string(obj->velocity.GetY()));
					//BOBO_INFO("Position Y: " + std::to_string((obj->transform->position).GetY()));
				}
			}
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

	private:
		std::vector<RigidBody*> m_PhysicsObjects;
		glm::vec3 m_Gravity;


		//This will have to be removed with the CalculateDeltaTime function aswell;
		std::chrono::high_resolution_clock::time_point m_PreviousTime;
		std::chrono::high_resolution_clock::time_point m_CurrentTime;
	};



}