#pragma once
#include "Bobo/ECS/EntityManager.h"
#include "Bobo/ECS/Component.h"
#include "Bobo/ECS/BaseComponents/Transform.h"

namespace Bobo
{
	struct RigidBody : public Component
	{
	public:
		Transform* transform;
		glm::vec3 velocity;
		glm::vec3 force;
		float mass;
		bool isStatic;
		RigidBody(Transform* transform, float mass, bool isStatic) : transform(transform), mass(mass), isStatic(isStatic), velocity(glm::vec3(0,0,0)), force(glm::vec3(0,0,0)) {}
	};
}