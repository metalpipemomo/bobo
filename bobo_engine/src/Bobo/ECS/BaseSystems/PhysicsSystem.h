#pragma once

#include "Bobo/ECS/System.h"
#include "Bobo/ECS/BaseComponents/RigidBody.h"
#include "Bobo/ECS/BaseComponents/Collider.h"
#include "Bobo/ECS/Scenes/Scene.h"
#include "Bobo/ECS/Physics/Collision.h"
#include "Bobo/ECS/Physics/CollisionSolver.h"

namespace Bobo
{
	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem();
		~PhysicsSystem();

		void Update() override;
		void FixedUpdate() override;
		void ResolveCollisions(float dt);
		void SolveCollision(std::vector<Collision> collisions);
		void SetActiveScene(Scene* scene);
		float CalculateDeltaTime();//this will have to be moved out of this class


	private:
		std::map<Entity, RigidBody*> m_PhysicsObjects;
		glm::vec3 m_Gravity;
		Scene* p_ActiveScene;

		//This will have to be removed with the CalculateDeltaTime function aswell;
		std::chrono::high_resolution_clock::time_point m_PreviousTime;
		std::chrono::high_resolution_clock::time_point m_CurrentTime;
	};



}