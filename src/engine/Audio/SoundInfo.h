#pragma once

#include "../../bpch.h"
#include <glm/glm.hpp>

struct SoundInfo
{
public:
    std::string m_UniqueID;

    // 3d info
    bool m_Is3D;

    // normal info
    bool m_Muted;
    bool m_IsLoop;
    float m_Pitch;

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
    
    glm::vec3 GetPosInfo() { return m_PosInfo; }

    void SetVolume(float vol) { m_Vol = vol > 3 ? 3 : vol; }

    float GetVolume() { return m_Vol; }
private:
    float m_Vol;
    glm::vec3 m_PosInfo;
};