#pragma once

#include "../Component.h"
#include <glm/glm.hpp>

class Transform : public Component
{
public:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

	Transform(glm::vec3 pos, glm::vec3 scale, glm::vec3 rot)
		: position(pos), scale(scale), rotation(rot) {}
};