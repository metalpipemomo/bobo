#include "bobopch.h"

#include "CollisionSolver.h"

namespace Bobo
{
	void CollisionSolver::SolveCollision(Collision col) {
		Solve(col.rbA->p_Collider, col.rbB->p_Collider, col);
	}

	void CollisionSolver::Solve(Collider* colliderA, Collider* colliderB, Collision col)
	{};

	void CollisionSolver::Solve(SphereCollider* sphereA, SphereCollider* sphereB, Collision col) // this might not work.
	{
		if (!col.rbA->m_IsStatic) { //make sure the object your acting on isnt static

			//gets the direction and distance from the 2 inner most collision points
			glm::vec3 direction = col.points.B - col.points.A;
			float distance = std::sqrt(std::powf(direction.x, 2) + std::powf(direction.y, 2) + std::powf(direction.z, 2));

			//normalizes the direction 
			glm::vec3 normalizedDirection = glm::normalize(direction);

			//moves the entity A's transform position so that it is outside entity B's rigidbody
			col.transformA->position += normalizedDirection * distance;
		}
	};

	void CollisionSolver::Solve(SphereCollider* sphereA, PlaneCollider* sphereB, Collision col)
	{};

	void CollisionSolver::Solve(PlaneCollider* sphereA, SphereCollider* sphereB, Collision col)
	{};


}