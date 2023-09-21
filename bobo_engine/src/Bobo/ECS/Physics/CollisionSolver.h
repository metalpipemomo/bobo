#pragma once
#include "Bobo/ECS/System.h"
#include "Bobo/ECS/BaseComponents/RigidBody.h"
#include "Bobo/ECS/BaseComponents/SphereCollider.h"
#include "Bobo/ECS/BaseComponents/PlaneCollider.h"
#include "Bobo/ECS/Scenes/Scene.h"
#include "Bobo/ECS/Physics/Collision.h"
namespace Bobo
{
	class CollisionSolver
	{
	public:
		static void SolveCollision(Collision col);


		
	private:
		static void Solve(Collider* colliderA, Collider* colliderB, Collision col);

		static void Solve(SphereCollider* sphereA, SphereCollider* sphereB, Collision col);

		static void Solve(SphereCollider* sphereA, PlaneCollider* sphereB, Collision col);

		static void Solve(PlaneCollider* sphereA, SphereCollider* sphereB, Collision col);
	};



}