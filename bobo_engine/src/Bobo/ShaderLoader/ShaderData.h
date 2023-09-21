#pragma once

#include "bobopch.h"
#include "VertexShader.h";
#include "FragmentShader.h";

/** Contains all shader data (vertex and fragment shaders). */

namespace Bobo
{
	class ShaderData
	{
	public:
		ShaderData();
		ShaderData(VertexShader v, FragmentShader f);
		VertexShader GetVertex();
		FragmentShader GetFragment();
		virtual ~ShaderData();
	private:
		VertexShader vs;
		FragmentShader fs;
	};
}