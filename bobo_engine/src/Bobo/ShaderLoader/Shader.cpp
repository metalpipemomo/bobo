#include "Shader.h"

Shader::Shader(ShaderData* shaderdata)
{
	sd = shaderdata;
}

ShaderData& Shader::GetShaderData() {
	return sd;
}