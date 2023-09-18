#pragma once

#include "bobopch.h"

namespace Bobo
{
	class Shader
	{
	public:
		Shader();
		Shader(VertexShader v, FragmentShader f, String fl);
		void UpdateShader();
		void LoadShader(String filelocation);
		void LoadShader(VertexShader v, FragmentShader f);
		virtual ~Shader() {
			delete shaderfile;
			delete vs;
			delete fs;
		};
	private:
		String shaderfile;
		VertexShader vs;
		FragmentShader fs;
	};
}