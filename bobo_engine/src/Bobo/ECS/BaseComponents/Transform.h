#pragma once

#include "Bobo/ECS/Component.h"
#include <glm/glm.hpp>

namespace Bobo
{
	struct BOBO_API Transform : public Component
	{
		Transform(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
			:position(position), scale(scale), rotation(rotation) {}

		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;
	};
}