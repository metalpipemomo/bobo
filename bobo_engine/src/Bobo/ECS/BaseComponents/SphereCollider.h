#pragma once

#include "Bobo/ECS/EntityManager.h"
#include "Bobo/ECS/Component.h"
#include "Bobo/ECS/BaseComponents/Transform.h"
#include "Bobo/ECS/BaseComponents/Collider.h"
namespace Bobo
{
	struct SphereCollider : public Collider
	{
		float m_Radius;
		glm::vec3 m_Center;

		CollisionPoints TestCollision( 
			const Transform* transform,
			const Collider* collider,
			const Transform* colliderTransform
		) {};
		CollisionPoints TestCollision( //sphere on sphere collision
			const Transform* transform,
			const SphereCollider* sphere,
			const Transform* sphereTransform
		) 
		{
			glm::vec3 direction = sphereTransform->position - transform->position; // get direction from point A to point B

	 		float depth = std::sqrt(std::powf(direction.x, 2) + std::powf(direction.y, 2) + std::powf(direction.z, 2)); // Get length of the direction Vector
	 		/*BOBO_INFO(std::to_string(depth));*/
	 		glm::vec3 normal = direction / depth; // Get Normal of the direction vector

	 		// determine if the objects are colliding
	 		// possibly change this so if they are not collding return null
	 		bool hasCollision = false;

	 		if (depth <= this->m_Radius + sphere->m_Radius)
	 			hasCollision = true;

			glm::vec3 A = transform->position + (direction * this->m_Radius); //deepest point of sphere a in sphere b
			glm::vec3 B = sphereTransform->position - (direction * this->m_Radius); //deepest point of sphere b in sphere a
	 		CollisionPoints collisionpoints{ A, B, normal, depth, hasCollision };
	 		return collisionpoints;

	 		//get normal between a and b
	 		// deepest point a will equal transform + radius(in direction of b)
	 		// deepest point b will equal transformb+radiusb(indirection of a);
		
		};
		CollisionPoints TestCollision( //sphere on plane collision
			const Transform* transform,
			const PlaneCollider* plane,
			const Transform* planeTransform
		) {};
	};
}