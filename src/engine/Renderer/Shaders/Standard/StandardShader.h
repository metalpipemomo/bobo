#pragma once

#include "../../../../bpch.h"
#include "../../Shader.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct StandardShaderProps
{
    glm::mat4 model, view, projection;
    GLuint texture;
};

class StandardShader : Shader
{
public:
    StandardShader()
    {
        m_Program = LoadProgram(
            "../src/engine/Renderer/Shaders/Standard/Standard.vert",
            "../src/engine/Renderer/Shaders/Standard/Standard.frag");
    }

    void Use() override
    {
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glUseProgram(m_Program);
    }

    void Data(StandardShaderProps ssp)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_Program, "model"), 1, GL_FALSE, glm::value_ptr(ssp.model));
        glUniformMatrix4fv(glGetUniformLocation(m_Program, "view"), 1, GL_FALSE, glm::value_ptr(ssp.view));
        glUniformMatrix4fv(glGetUniformLocation(m_Program, "projection"), 1, GL_FALSE, glm::value_ptr(ssp.projection));
        glUniform1i(glGetUniformLocation(m_Program, "texSampler"), ssp.texture);
    }
};