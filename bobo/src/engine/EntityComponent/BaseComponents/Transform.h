#pragma once

#include "../Component.h"
#include <glm/glm.hpp>

class Transform : public Component
{
public:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

	Transform(glm::vec3 pos = glm::vec3{0, 0, 0}, glm::vec3 scale = glm::vec3{1, 1, 1}, glm::vec3 rot = glm::vec3{0, 0, 0})
		: position(pos), scale(scale), rotation(rot) {}
};