#pragma once

#include "../../bpch.h"

#include <glad/glad.h>

class ShaderLoader
{
public:
	static void Init()
	{
		auto sl = GetInstance();
		sl->LoadProgram("../src/engine/Renderer/Shaders/Standard.vert", "../src/engine/Renderer/Shaders/Standard.frag");
		BOBO_INFO("Shader Loader initialized!");
	}

	static GLuint GetProgram() { return GetInstance()->program; }

private:
	static ShaderLoader* GetInstance()
	{
		static ShaderLoader* instance = new ShaderLoader();
		return instance;
	}

	ShaderLoader() {}

	std::string ReadShader(const std::string& path)
	{
		std::ifstream file{ path };

		if (!file.is_open())
		{
			BOBO_ERROR("Failed to open shader file at {}", path);
			return "";
		}

		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	GLuint LoadShader(GLenum type, const std::string& shaderSource)
	{
		GLuint shader = glCreateShader(type);

		if (shader == 0)
		{
			BOBO_ERROR("Failed to create shader out of \n{}", shaderSource);
			return -1;
		}
		auto fileShader = ReadShader(shaderSource);
		const char* shaderSrc = fileShader.c_str();
		glShaderSource(shader, 1, &shaderSrc, NULL);
		glCompileShader(shader);

		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

			if (infoLen > 1)
			{
				char* infoLog = (char*)malloc(sizeof(char) * infoLen);
				glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
				BOBO_ERROR("Shader Compilation Error: \n{}", infoLog);
				free(infoLog);
			}

			glDeleteShader(shader);

			return -1;
		}

		return shader;
	}

	GLuint LoadProgram(const std::string& vertSource, const std::string& fragSource)
	{
		GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, vertSource);
		if (vertexShader == -1)
		{
			BOBO_ERROR("Failed to load vertex shader in LoadProgram...");
			return -1;
		}

		GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragSource);
		if (fragmentShader == -1)
		{
			BOBO_ERROR("Failed to load fragment shader in LoadProgram...");
			glDeleteShader(vertexShader);
			return -1;
		}

		// Need this to stay in memory
		program = glCreateProgram();
		if (program == 0)
		{
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			return -1;
		}

		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		GLint linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);

		if (!linked)
		{
			GLint infoLen = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);

			if (infoLen > 1)
			{
				char* infoLog = (char*)malloc(sizeof(char) * infoLen);
				glGetProgramInfoLog(program, infoLen, NULL, infoLog);
				BOBO_ERROR("Shader Linking Error: \n{}", infoLog);
				free(infoLog);
			}

			glDeleteProgram(program);
			return -1;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return program;
	}

	GLuint program;
};