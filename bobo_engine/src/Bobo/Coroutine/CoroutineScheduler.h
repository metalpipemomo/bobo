#pragma once

#include "Coroutine.h"
#include "Waits/WaitForSeconds.h"
#include "Waits/WaitUntil.h"

namespace Bobo
{
    class BOBO_API CoroutineScheduler
    {
    public:
        static CoroutineScheduler& GetInstance()
        {
            static CoroutineScheduler* instance = new CoroutineScheduler();
            return *instance;
        }

        template <typename T, typename... Args>
        bool StartCoroutine(Args&& ...args)
        {
            if (std::is_base_of<Coroutine, T>::value)
            {
                T* coroutine = new T(std::forward<Args>(args)...);
                m_Coroutines.push_back(coroutine);
                return true;
            }

            return false;
        }

        void Update()
        {
            // we iterate inside the loop because otherwise it will increment the iterator
            // after we erase a resolved Coroutine and then error out if there are no more
            // coroutines.
            auto it = m_Coroutines.begin();
            while (it != m_Coroutines.end())
            {
                if ((*it)->Resolve())
                {
                    (*it)->m_Function();
                    it = m_Coroutines.erase(it);
                } else
                {
                    it++;
                }
            }
        }

    private:
        CoroutineScheduler() {}
        std::vector<Coroutine*> m_Coroutines;
    };
}