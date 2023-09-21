#include "bobopch.h"

#include "CollisionSolver.h"

namespace Bobo
{
	void CollisionSolver::SolveCollision(Collision col) {
		foo(col.rbA->p_Collider, col.rbB->p_Collider, col);
	}

	void CollisionSolver::foo(Collider* colliderA, Collider* colliderB, Collision col)
	{};

	void CollisionSolver::foo(SphereCollider* sphereA, SphereCollider* sphereB, Collision col)
	{
	
	};

	void CollisionSolver::foo(SphereCollider* sphereA, PlaneCollider* sphereB, Collision col)
	{};

	void CollisionSolver::foo(PlaneCollider* sphereA, SphereCollider* sphereB, Collision col)
	{};


}