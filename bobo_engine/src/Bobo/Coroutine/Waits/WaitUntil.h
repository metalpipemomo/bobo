#pragma once

#include "Bobo/Coroutine/Coroutine.h"

namespace Bobo
{
    template <typename T>
    class BOBO_API WaitUntil : public Coroutine
    {
    public:
        WaitUntil(T* valueToWatch, T valueToMatch, const std::function<void()>& f)
            : Coroutine(f), m_ValueToWatch(valueToWatch), m_ValueToMatch(valueToMatch)
        {}

        bool Resolve() override
        {
            return *m_ValueToWatch == m_ValueToMatch;
        }

    private:
        T* m_ValueToWatch;
        T m_ValueToMatch;
    };
}