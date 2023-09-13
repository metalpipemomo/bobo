#include "bobopch.h"
#include "ModelData.h"

namespace Bobo
{
	ModelData::ModelData(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals)
	{
		m_Vertices = vertices;
		m_UVs = uvs;
		m_Normals = normals;
	}

	ModelData::~ModelData()
	{

	}
}