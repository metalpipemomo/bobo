#pragma once


#include "Bobo/ECS/BaseComponents/Collider.h"
namespace Bobo
{
	struct PlaneCollider : public Collider
	{
		CollisionPoints TestCollision(
			const Transform* transform,
			const Collider* collider,
			const Transform* colliderTransform
		) {};
		CollisionPoints TestCollision(
			const Transform* transform,
			const SphereCollider* sphere,
			const Transform* sphereTransform
		) {};
		CollisionPoints TestCollision(
			const Transform* transform,
			const PlaneCollider* plane,
			const Transform* planeTransform
		) {};
	};
}