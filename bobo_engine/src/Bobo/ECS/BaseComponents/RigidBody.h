#pragma once

#include "Bobo/ECS/EntityManager.h"
#include "Bobo/ECS/Component.h"
#include "Bobo/ECS/BaseComponents/Transform.h"
#include "Bobo/ECS/BaseComponents/Collider.h"
namespace Bobo
{
	struct RigidBody : public Component
	{
		Transform* p_Transform;
		glm::vec3 m_Velocity;
		glm::vec3 m_Force;
		float m_Mass;
		bool m_IsStatic;
		Collider* p_Collider;

		RigidBody(Transform* transform) : p_Transform(transform) {}
	};
}