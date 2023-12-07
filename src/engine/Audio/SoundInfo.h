#pragma once

#include "../../bpch.h"
#include <glm/glm.hpp>

struct SoundInfo
{
public:
    std::string m_UniqueID;

    // 3d info
    bool m_Is3D;
    glm::vec3 m_PosInfo;

    // normal info
    bool m_IsLoop;
    bool m_Muted;
    float m_Pitch;
    float m_Vol;

    // non-3d info
    float m_Pan;

    SoundInfo(const char* filePath = "") : m_UniqueID(filePath)
    {
        SetToDefaultValues();
    }

    void SetToDefaultValues()
    {
        m_Pitch = 1;
        m_Vol = 1;
        m_Pan = 0;
        m_IsLoop = false;
        m_Muted = false;
        m_Is3D = false;
        m_PosInfo = { 0, 0, 0 };
    }

    void Set3DCoords(float x, float y, float z)
    {
        m_PosInfo.x = x;
        m_PosInfo.y = y;
        m_PosInfo.z = z;
    }

    void Set3DCoords(glm::vec3 coords) { m_PosInfo = coords; }
};