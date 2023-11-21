#pragma once

#include "../../../bpch.h"

#include "../Coroutine.h"

class WaitUntil : public Coroutine
{
public:
	WaitUntil(const std::function<void()>& f, const std::function<bool()>& res)
		: Coroutine(f), m_ResolverFunction(res) {}

	bool Resolve() override
	{
		return m_ResolverFunction();
	}

private:
	std::function<bool()> m_ResolverFunction;
};