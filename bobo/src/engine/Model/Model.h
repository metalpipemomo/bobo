#pragma once

#include "bpch.h"
#include "glm/glm.hpp"

struct Model
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;

	Model(std::vector<glm::vec3> verts, std::vector<glm::vec2> tcs, std::vector<glm::vec3> norms)
		: vertices(verts), texCoords(tcs), normals(norms) {}
};