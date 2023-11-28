#pragma once

#include "../../../../bpch.h"
#include "../../Shader.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct SkyBoxShaderProps
{
    glm::mat4 view, projection;
    GLuint texture;
};

/** The skybox shader extends from the shader class
    and manages the skybox's shaders (fragment and vertex).
    */

class SkyBoxShader : Shader
{
public:
    /** Constructor that loads the vertex and fragment shaders into the shader program.*/
    SkyBoxShader()
    {
        m_Program = LoadProgram(
            "../src/engine/Renderer/Shaders/SkyBox/SkyBox.vert",
            "../src/engine/Renderer/Shaders/SkyBox/SkyBox.frag");
    }
    /** Uses and activates the shader. */
    void Use() override
    {
        glUseProgram(m_Program);
    }
    /** Takes in a set of shader properties and applies it to the shader program.
    @param ssp
    */
    void Data(SkyBoxShaderProps ssp)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_Program, "view"), 1, GL_FALSE, glm::value_ptr(ssp.view));
        glUniformMatrix4fv(glGetUniformLocation(m_Program, "projection"), 1, GL_FALSE, glm::value_ptr(ssp.projection));
        glUniform1i(glGetUniformLocation(m_Program, "skybox"), ssp.texture);
    }
};