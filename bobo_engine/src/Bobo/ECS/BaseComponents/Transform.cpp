#include "bobopch.h"

#include "Transform.h"

namespace Bobo
{
	Transform::Transform(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
	{
		this->m_Position = position;
		this->m_Scale = scale;
		this->m_Rotation = rotation;
	}
}