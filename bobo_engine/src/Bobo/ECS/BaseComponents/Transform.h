#pragma once

#include <glm/glm.hpp>
#include "Bobo/ECS/Component.h"

namespace Bobo
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