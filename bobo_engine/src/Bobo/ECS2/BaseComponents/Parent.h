#pragma once

#include "Bobo/ECS2/EntityManager.h"
#include "Bobo/ECS2/Component.h"

namespace Bobo2
{
	struct Parent : public Component
	{
		Entity parentId;

		Parent(Entity parentId) : parentId(parentId) {}
	};
}