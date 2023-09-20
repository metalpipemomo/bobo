#pragma once

#include "Bobo/ECS/EntityManager.h"
#include "Bobo/ECS/Component.h"
#include "Bobo/ECS/BaseComponents/Transform.h"
namespace Bobo
{
	struct RigidBody : public Component
	{
		Transform* transform;

		RigidBody(Transform* transform) : transform(transform) {};
	};
}