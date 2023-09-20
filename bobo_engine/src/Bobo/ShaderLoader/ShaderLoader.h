#pragma once

#include "bobopch.h"
#include "Shader.h"

/** Loads Shaders */

namespace Bobo
{
	class BOBO_API ShaderLoader
	{
	public:
		static ShaderLoader& GetInstance()
		{
			static ShaderLoader* instance = new ShaderLoader();
			return *instance;
		}
		ShaderLoader();
		virtual ~ShaderLoader() {
			delete m_LoadedShaders;
		};
		void LoadShader(std::string filelocation);
	private:
		ShaderLoader();
		std::string shaderfile;
		std::unordered_map<std::string, Shader*> m_LoadedShaders;
	};
}