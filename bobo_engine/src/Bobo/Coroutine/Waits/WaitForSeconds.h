#pragma once

#include "Bobo/Coroutine/Coroutine.h"
#include "Bobo/Time.h"

namespace Bobo
{
    class BOBO_API WaitForSeconds : public Coroutine
    {
    public:
        /*
            Time Mode determines whether to use Scaled Time (0), Unscaled Time (1), or Fixed Time (2)
        */
        WaitForSeconds(float waitTimeInSeconds, int timeMode, const std::function<void()>& f)
            : Coroutine(f), m_WaitTime(waitTimeInSeconds), m_TimeMode(timeMode)
        {
            m_ElapsedTime = 0;
        }

        bool CheckForResolve() override
        {
            // Log("Elapsed: {} / Wait: {}", m_ElapsedTime, m_WaitTime);
            return m_ElapsedTime > m_WaitTime;
        }

        void Update() override
        {
            switch (m_TimeMode)
            {
            case 0:
                // Scaled Time
                m_ElapsedTime += Time::GetInstance()->DeltaTime();
                break;
            case 1:
                // Unscaled Time
                m_ElapsedTime += Time::GetInstance()->UnscaledDeltaTime();
                break;
            case 2:
                // Fixed Time
                m_ElapsedTime += Time::GetInstance()->FixedDeltaTime();
                break;
            default:
                return;
            }
        }

    private:
        int m_TimeMode;
        float m_WaitTime;
        float m_ElapsedTime;
    };
}