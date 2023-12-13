#pragma once

#include "bpch.h"
#include <glad/glad.h>

// Shader class definition
class Shader
{
public:
    Shader() = default;
    ~Shader() { glDeleteProgram(m_Program); }
    virtual void Use() = 0;
    std::vector<char> ReadFile(const std::string& path);
    GLuint LoadShader(GLenum type, const std::string& shaderSource);
    GLuint LoadProgram(const std::string& vertSource, const std::string& fragSource);
protected:
    GLuint m_Program;
};

// Reads a file into a vector of characters, basically a string
std::vector<char> Shader::ReadFile(const std::string& path)
{
    std::ifstream file{ path, std::ios::ate };

    if (!file.is_open())
    {
        throw std::runtime_error("file [Stuff] aint workin dawg");
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

// Compiles a shader, boilerplate
GLuint Shader::LoadShader(GLenum type, const std::string& shaderSource)
{
    GLuint shader = glCreateShader(type);

    if (shader == 0)
    {
        throw std::runtime_error("shader [Stuff] aint workin 1");
        return -1;
    }

    auto fileShader = ReadFile(shaderSource);
    const char* shaderSrc = fileShader.data();
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
            std::cout << infoLog << std::endl;
            throw std::runtime_error("shader [Stuff] aint workin 2");
            free(infoLog);
        }

        glDeleteShader(shader);

        return -1;
    }

    return shader;
}

// Loads a program from a vertex and a fragment shader, boilerplate
GLuint Shader::LoadProgram(const std::string& vertSource, const std::string& fragSource)
{
    GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, vertSource);
    if (vertexShader == -1)
    {
        throw std::runtime_error("program [Stuff] aint workin 1");
        return -1;
    }

    GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragSource);
    if (fragmentShader == -1)
    {
        throw std::runtime_error("program [Stuff] aint workin 2");
        glDeleteShader(vertexShader);
        return -1;
    }

    // Need this to stay in memory
    GLuint program = glCreateProgram();
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
            throw std::runtime_error("program [Stuff] aint workin 2");
            free(infoLog);
        }

        glDeleteProgram(program);
        return -1;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}