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
			(*it)->Update();
			if ((*it)->Resolve())
			{
				(*it)->m_Function();

				if ((*it)->m_NextCoroutine != nullptr)
				{
					StartCoroutine((*it)->m_NextCoroutine);
				}
				it = c->m_Coroutines.erase(it);
			}
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