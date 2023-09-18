#pragma once

#include <glm/glm.hpp>
#include "Bobo/ECS2/Component.h"

namespace Bobo2
{
	struct Transform : public Component
	{
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;

		Transform(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
			: position(position), scale(scale), rotation(rotation) {}
	};
}