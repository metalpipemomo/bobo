#pragma once

#include "bpch.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

struct Spotlight
{
    glm::vec3 position;
    glm::vec3 direction;
    GLfloat cutoffAngle;
    glm::vec3 baseColor;
};

struct Pointlight
{
    glm::vec3 position;
    glm::vec3 baseColor;
};