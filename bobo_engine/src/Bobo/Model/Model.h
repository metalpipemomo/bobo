#pragma once

#include "bobopch.h"
#include <glm/glm.hpp>

namespace Bobo
{
	class BOBO_API Model
	{
	public:
		std::vector<glm::vec3> m_Vertices;
		std::vector<glm::vec2> m_TexCoords;
		std::vector<glm::vec3> m_Normals;

		Model(std::vector<glm::vec3> verts, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals)
			: m_Vertices(verts), m_TexCoords(uvs), m_Normals(normals)
		{}
	};
}