#pragma once

#include "../../bpch.h"
#include "Coroutine.h"

class CoroutineScheduler
{
public:
	static void Init()
	{
		GetInstance();
	}

	template <typename T, typename... Args>
	static Coroutine* StartCoroutine(Args&& ...args)
	{
		if (std::is_base_of<Coroutine, T>::value)
		{
			T* coroutine = new T(std::forward<Args>(args)...);
			return GetInstance()->StartCoroutine(static_cast<Coroutine*>(coroutine));
		}
	}

	static Coroutine* StartCoroutine(Coroutine* coroutine)
	{
		GetInstance()->m_Coroutines.push_back(coroutine);
		return coroutine;
	}

	static void Update()
	{
		auto c = GetInstance();

		auto it = c->m_Coroutines.begin();
		while (it != c->m_Coroutines.end())
		{
			// update the state of all coroutines
			(*it)->Update();
			// if the coroutine is done
			if ((*it)->Resolve())
			{
				// call its function
				(*it)->m_Function();

				// if there's a followup coroutine then start it
				if ((*it)->m_NextCoroutine != nullptr)
				{
					StartCoroutine((*it)->m_NextCoroutine);
				}

				// get rid of this current coroutine from the coroutine list
				it = c->m_Coroutines.erase(it);
			}
			// otherwise, move onto the next coroutine
			else
			{
				it++;
			}
		}
	}

private:
	static CoroutineScheduler* GetInstance()
	{
		static CoroutineScheduler* instance = new CoroutineScheduler();
		return instance;
	}

	CoroutineScheduler() {}
	
	std::vector<Coroutine*> m_Coroutines;
};