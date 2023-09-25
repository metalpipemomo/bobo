#pragma once

#include "Bobo/Coroutine/Coroutine.h"

namespace Bobo
{
    class BOBO_API WaitForCoroutine : public Coroutine
    {
    public:
        WaitForCoroutine(Coroutine* waitingFor, const std::function<void()>& f) : Coroutine(f), m_WaitingFor(waitingFor) {}

        bool CheckForResolve() override
        {
            if (m_WaitingFor == nullptr)
                return true;
            return m_WaitingFor->CheckForResolve();
        }

    private:
        Coroutine* m_WaitingFor;
    };
}