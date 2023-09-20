#pragma once

#include "bobopch.h"
#include "ShaderData.h"

/** Represents a single shader. */

namespace Bobo
{
	class Shader
	{
	public:
		Shader(ShaderData* shaderdata);
		virtual ~Shader() {
			delete sd;
		};
	private:
		ShaderData* sd = nullptr;
	};
}