#pragma once

#include "Bobo/Coroutine/Coroutine.h"

namespace Bobo
{
    class BOBO_API WaitForEvaluation : public Coroutine
    {
    public:
        WaitForEvaluation(const std::function<bool()>& eval, const std::function<void()>& f)
            : m_EvalFunc(eval), Coroutine(f) {}

        bool CheckForResolve() override
        {
            return m_EvalFunc();
        }

    private:
        std::function<bool()> m_EvalFunc;
    };
}