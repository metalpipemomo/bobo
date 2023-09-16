#include "bobopch.h"
#include "ModelData.h"

namespace Bobo
{
	/** Model Data Constructor that takes in vertices, uv, and normals values in order to 
	store model data for rendering. 
	@param vertices
	@param uvs
	@param normals
	*/

	ModelData::ModelData(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals)
	{
		m_Vertices = vertices;
		m_UVs = uvs;
		m_Normals = normals;
	}

	/* Model data destructor for use upon program termination.
	*/

	ModelData::~ModelData()
	{
		delete m_Vertices;
		delete m_UVs;
		delete m_Normals;
	}
}