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
        Coroutine(const std::function<void()>& f) : m_Function(f) {}
        virtual bool Resolve() { return true; };

        std::function<void()> m_Function;
    };
}