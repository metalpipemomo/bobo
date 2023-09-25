#pragma once

#include "bobopch.h"

//
// Keep Coroutine implementations in one file, they're just not big enough for a .cpp
// No file bloat plox T_T
//

namespace Bobo
{
    class Coroutine
    {
    public:
        Coroutine(const std::function<void()>& f) : m_Function(f) { m_ChainedCoroutine = nullptr; }
        virtual bool CheckForResolve() { return true; };
        virtual void Update() {};

        Coroutine* AddCoroutineToChain(Coroutine* coroutine) 
        {
            if (m_ChainedCoroutine == nullptr)
            {
                m_ChainedCoroutine = coroutine; 
                return this; 
            }
            else 
            {
                return m_ChainedCoroutine->AddCoroutineToChain(coroutine);
            }
        }

        Coroutine* GetNextCoroutine()
        {
            return m_ChainedCoroutine;
        }

        Coroutine* GetCoroutineInChain(unsigned int positionInChain)
        {
            if (m_ChainedCoroutine == nullptr || positionInChain == 0)
                return this;
            return m_ChainedCoroutine->GetCoroutineInChain(--positionInChain);
        }

        std::function<void()> m_Function;
        Coroutine* m_ChainedCoroutine;
    };
}