#pragma once

#include "bobopch.h"
#include <glm/glm.hpp>

namespace Bobo
{
	class ModelData
	{
	public:
		ModelData(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals);
		virtual ~ModelData();
	private:
		std::vector<glm::vec3> m_Vertices;
		std::vector<glm::vec2> m_UVs;
		std::vector<glm::vec3> m_Normals;
	};
}