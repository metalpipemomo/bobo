#pragma once

#include "Bobo/Coroutine/Coroutine.h"

namespace Bobo
{
    class BOBO_API WaitForSeconds : public Coroutine
    {
    public:
        WaitForSeconds(float waitTimeInSeconds, const std::function<void()>& f) : Coroutine(f), m_WaitTime(waitTimeInSeconds)
        {
            m_StartTime = std::chrono::high_resolution_clock::now();
        }

        bool Resolve() override
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - m_StartTime);

            return elapsedTime.count() > m_WaitTime;
        }

    private:
        float m_WaitTime;
        std::chrono::steady_clock::time_point m_StartTime;
    };
}