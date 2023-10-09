#pragma once

#include "bpch.h"

#include "../Coroutine.h"

class WaitForCoroutine : public Coroutine
{
public:
	WaitForCoroutine(const std::function<void()>& f, Coroutine* stall)
		: Coroutine(f), m_StallingCoroutine(stall) {}

	bool Resolve() override
	{
		if (m_StallingCoroutine == nullptr) return true;

		return m_StallingCoroutine->Resolve();
	}

private:
	Coroutine* m_StallingCoroutine;
};