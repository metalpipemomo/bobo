#pragma once

#include "Coroutine.h"
#include "Waits/WaitForSeconds.h"
#include "Waits/WaitUntil.h"
#include "Waits/WaitForCoroutine.h"
#include "Waits/WaitForEvaluation.h"

namespace Bobo
{
    class BOBO_API CoroutineScheduler
    {
    public:
        static CoroutineScheduler* GetInstance()
        {
            static CoroutineScheduler instance = CoroutineScheduler();
            return &instance;
        }

        template <typename T, typename... Args>
        Coroutine* StartCoroutine(Args&& ...args)
        {
            if (std::is_base_of<Coroutine, T>::value)
            {
                T* coroutine = new T(std::forward<Args>(args)...);
                return StartCoroutine(coroutine);
            }

            return nullptr;
        }

        Coroutine* StartCoroutine(Coroutine* coroutine)
        {
            Log("New Coroutine: {}", typeid(*coroutine).name());
            m_Coroutines.push_back(coroutine);
            return coroutine;
        }

        void Update()
        {
            // we iterate inside the loop because otherwise it will increment the iterator
            // after we erase a resolved Coroutine and then error out if there are no more
            // coroutines.
            unsigned int i = 0;
            while (i < m_Coroutines.size())
            {
                Coroutine* c = m_Coroutines.at(i);
                c->Update();
                if (c->CheckForResolve())
                {
                    c->m_Function();

                    // Remove the coroutine from the vector 
                    m_Coroutines.erase(m_Coroutines.begin() + i);

                    // Add Chained coroutine if there is one 
                    if (c->GetNextCoroutine() != nullptr) { StartCoroutine(c->GetNextCoroutine()); }
                }
                else
                {
                    i++;
                }
            }
        }

    private:
        CoroutineScheduler() {}
        std::vector<Coroutine*> m_Coroutines;
    };
}