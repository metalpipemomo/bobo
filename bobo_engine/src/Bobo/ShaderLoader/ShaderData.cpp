#include "bobopch.h"
#include "ShaderData.h"

namespace Bobo
{
	ShaderData::ShaderData() {
		vs = VertexShader();
		fs = FragmentShader();
	}
	/** Model Data Constructor that takes in vertices, uv, and normals values in order to
	store model data for rendering.
	@param vertices
	@param uvs
	@param normals
	*/

	ShaderData::ShaderData(VertexShader v, FragmentShader f)
	{
		vs = VertexShader();
		fs = FragmentShader();
	}

	/* Model data destructor for use upon program termination.
	*/

	ShaderData::~ShaderData()
	{
		delete vs;
		delete fs;
	}
}