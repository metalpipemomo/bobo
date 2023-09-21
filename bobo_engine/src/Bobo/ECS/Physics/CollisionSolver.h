#pragma once
#include "Bobo/ECS/System.h"
#include "Bobo/ECS/BaseComponents/RigidBody.h"
#include "Bobo/ECS/BaseComponents/Collider.h"
#include "Bobo/ECS/Scenes/Scene.h"
#include "Bobo/ECS/Physics/Collision.h"
namespace Bobo
{
	class CollisionSolver
	{
		static void SolveCollision(Collision col);

		static void foo(Collider* colliderA, Collider* colliderB, Collision col);

		static void foo(SphereCollider* sphereA, SphereCollider* sphereB, Collision col);

		static void foo(SphereCollider* sphereA, PlaneCollider* sphereB, Collision col);

		static void foo(PlaneCollider* sphereA, SphereCollider* sphereB, Collision col);

		
	private:
	};



}