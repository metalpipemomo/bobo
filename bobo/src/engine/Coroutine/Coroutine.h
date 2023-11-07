#pragma once

#include "bpch.h"

class Coroutine
{
public:
	Coroutine(const std::function<void()>& f) : m_Function(f) { m_NextCoroutine = nullptr; }

	virtual bool Resolve() { return true; }

	virtual void Update() {};

	bool m_repeat = false;

	Coroutine* ChainCoroutine(Coroutine* coroutine)
	{
		if (m_NextCoroutine == nullptr)
		{
			m_NextCoroutine = coroutine;
			return this;
		}
		else
		{
			return m_NextCoroutine->ChainCoroutine(coroutine);
		}
	}

	std::function<void()> m_Function;
	Coroutine* m_NextCoroutine;
};