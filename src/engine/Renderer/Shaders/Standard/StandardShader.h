#pragma once

#include "../../../../bpch.h"
#include "../../Shader.h"
#include "../../Light.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct StandardShaderProps
{
    glm::mat4 model, view, projection;
    GLuint texture;
    std::vector<SpotlightComponent*> spotlights;
    std::vector<PointlightComponent*> pointlights;
    GLfloat shininess;
    glm::vec3 cameraPos;
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
        glUseProgram(m_Program);
    }

    // Sets all the uniforms used for the standard shader
    void Data(StandardShaderProps ssp)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_Program, "model"), 1, GL_FALSE, glm::value_ptr(ssp.model));
        glUniformMatrix4fv(glGetUniformLocation(m_Program, "view"), 1, GL_FALSE, glm::value_ptr(ssp.view));
        glUniformMatrix4fv(glGetUniformLocation(m_Program, "projection"), 1, GL_FALSE, glm::value_ptr(ssp.projection));
        glUniform1i(glGetUniformLocation(m_Program, "texSampler"), ssp.texture);
        glUniform3fv(glGetUniformLocation(m_Program, "cameraPos"), 1, glm::value_ptr(ssp.cameraPos));
        glUniform1f(glGetUniformLocation(m_Program, "shininess"), ssp.shininess);
        glUniform1i(glGetUniformLocation(m_Program, "numSpotlights"), ssp.spotlights.size());
        glUniform1i(glGetUniformLocation(m_Program, "numPointlights"), ssp.pointlights.size());
        
        for (size_t i = 0; i < ssp.spotlights.size(); i++)
        {
            std::string baseName = "spotlights[" + std::to_string(i) + "].";
            glUniform3fv(glGetUniformLocation(m_Program, (baseName + "position").c_str()), 1, glm::value_ptr(ssp.spotlights[i]->spotlight.position));
            glUniform3fv(glGetUniformLocation(m_Program, (baseName + "direction").c_str()), 1, glm::value_ptr(ssp.spotlights[i]->spotlight.direction));
            glUniform1f(glGetUniformLocation(m_Program, (baseName + "cutoffAngle").c_str()), ssp.spotlights[i]->spotlight.cutoffAngle);
            glUniform3fv(glGetUniformLocation(m_Program, (baseName + "baseColor").c_str()), 1, glm::value_ptr(ssp.spotlights[i]->spotlight.baseColor));
        }

        for (size_t i = 0; i < ssp.pointlights.size(); i++)
        {
            std::string baseName = "pointlights[" + std::to_string(i) + "].";
            glUniform3fv(glGetUniformLocation(m_Program, (baseName + "position").c_str()), 1, glm::value_ptr(ssp.pointlights[i]->pointlight.position));
            glUniform3fv(glGetUniformLocation(m_Program, (baseName + "baseColor").c_str()), 1, glm::value_ptr(ssp.pointlights[i]->pointlight.baseColor));
            glUniform1f(glGetUniformLocation(m_Program, (baseName + "intensity").c_str()), ssp.pointlights[i]->pointlight.intensity);
        }
    }
};