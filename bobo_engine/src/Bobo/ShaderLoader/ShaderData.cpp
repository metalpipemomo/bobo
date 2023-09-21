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

	VertexShader ShaderData::GetVertex() {
		return vs;
	}

	FramgentShader ShaderData::GetFragment() {
		return fs;
	}

	/* Model data destructor for use upon program termination.
	*/

	ShaderData::~ShaderData()
	{
		delete vs;
		delete fs;
	}
}