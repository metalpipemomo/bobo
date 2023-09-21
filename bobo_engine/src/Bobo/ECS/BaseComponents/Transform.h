#pragma once

#include "Bobo/ECS/Component.h"
#include <glm/glm.hpp>

namespace Bobo
{
	struct BOBO_API Transform : public Component
	{
		Transform(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);

		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		glm::vec3 m_Rotation;
	};
}