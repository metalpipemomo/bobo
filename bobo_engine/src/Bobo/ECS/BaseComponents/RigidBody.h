#pragma once

#include "Bobo/ECS/Component.h"

namespace Bobo
{
	struct BOBO_API RigidBody : public Component
	{
		// needs more field, just 4 example rn
		bool gravityEnabled;

		RigidBody(bool gravityEnabled) : gravityEnabled(gravityEnabled) {}
	};
}