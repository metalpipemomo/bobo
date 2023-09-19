#pragma once

#include "Bobo/ECS/EntityManager.h"
#include "Bobo/ECS/Component.h"

namespace Bobo
{
	struct Parent : public Component
	{
		Entity parentId;

		Parent(Entity parentId) : parentId(parentId) {}
	};
}