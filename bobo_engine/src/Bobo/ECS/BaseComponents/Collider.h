#pragma once

#include "Bobo/ECS/EntityManager.h"
#include "Bobo/ECS/Component.h"
#include "Bobo/ECS/BaseComponents/Transform.h"
namespace Bobo
{
	//forward declaration of derived types to allow the parent Collider to create virtual functions containing them.
	struct Collider;
	struct SphereCollider;
	struct PlaneCollider;


	// struct to hold the information on the points of collision
	struct CollisionPoints
	{
		glm::vec3 A;
		glm::vec3 B;
		glm::vec3 Normal;
		float Depth;
		bool HasCollision;
	};

	struct Collider : public Component
	{
		Collider() {}

		virtual CollisionPoints TestCollision(
			const Transform* transform,
			const Collider* collider,
			const Transform* colliderTransform
		) const = 0;
		virtual CollisionPoints TestCollision(
			const Transform* transform,
			const SphereCollider* sphere,
			const Transform* sphereTransform
		) const = 0;
		virtual CollisionPoints TestCollision(
			const Transform* transform,
			const PlaneCollider* plane,
			const Transform* planeTransform
		) const = 0;
	};
}