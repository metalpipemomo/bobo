#pragma once

#include "bpch.h"
#include <glm/glm.hpp>

struct SoundInfo
{
public:
    // Constructor
    SoundInfo(const char* filePath = "", bool isLoop = false, bool is3D = false, float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : m_FilePath(filePath), m_IsLoop(isLoop), m_Is3D(is3D), m_PosInfo(x, y, z)
    {
        m_UniqueID = filePath;
    }

    // convienience method to set the 3D coordinates of the sound.
    void Set3DCoords(float x, float y, float z)
    {
        m_PosInfo.x = x;
        m_PosInfo.y = y;
        m_PosInfo.z = z;
    }

    // convienience method to set the 3D coordinates of the sound.
    void Set3DCoords(glm::vec3 coords)
    {
        m_PosInfo = coords;
    }

    std::string m_UniqueID;

    const char* m_FilePath;

    bool m_IsLoop;
    bool m_Is3D;
    bool m_IsLoaded;

    glm::vec3 m_PosInfo;
};