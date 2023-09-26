#pragma once

#include "Bobo/ECS/Component.h"
#include "Bobo/Model/Model.h"

namespace Bobo
{
	struct Material : public Component
	{
		Model* modelData;
	};
}