#pragma once

#include "../../../../bpch.h"
#include "../../Shader.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct SkyBoxShaderProps
{
    glm::mat4 model, view, projection;
    GLuint texture;
};

class SkyBoxShader : Shader
{
public:
    SkyBoxShader()
    {
        m_Program = LoadProgram(
            "../src/engine/Renderer/Shaders/SkyBox/SkyBox.vert",
            "../src/engine/Renderer/Shaders/SkyBox/SkyBox.frag");
    }

    void Use() override
    {
        glUseProgram(m_Program);
    }

    void Data(SkyBoxShaderProps ssp)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_Program, "model"), 1, GL_FALSE, glm::value_ptr(ssp.model));
        glUniformMatrix4fv(glGetUniformLocation(m_Program, "view"), 1, GL_FALSE, glm::value_ptr(ssp.view));
        glUniformMatrix4fv(glGetUniformLocation(m_Program, "projection"), 1, GL_FALSE, glm::value_ptr(ssp.projection));
        glUniform1i(glGetUniformLocation(m_Program, "texSampler"), ssp.texture);
    }
};