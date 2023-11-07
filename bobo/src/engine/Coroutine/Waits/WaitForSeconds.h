#pragma once

#include "bpch.h"

#include "../Coroutine.h"
#include "engine/Time.h"

enum class TimeMode
{
	DeltaTime,
	UnscaledDeltaTime,
	FixedDeltaTime
};

class WaitForSeconds : public Coroutine
{
public:

	WaitForSeconds(const std::function<void()>& f, float timeInSeconds, TimeMode timeMode = TimeMode::DeltaTime, bool repeat = false)
		: Coroutine(f), m_WaitTime(timeInSeconds), m_TimeMode(timeMode)
	{
		m_repeat = repeat;
		m_ElapsedTime = 0;
	}

	bool Resolve() override
	{
		return m_ElapsedTime > m_WaitTime;
	}

	void Update() override
	{
		switch (m_TimeMode)
		{
		case TimeMode::DeltaTime:
			m_ElapsedTime += Time::DeltaTime();
			break;
		case TimeMode::UnscaledDeltaTime:
			m_ElapsedTime += Time::UnscaledDeltaTime();
			break;
		case TimeMode::FixedDeltaTime:
			m_ElapsedTime += Time::FixedDeltaTime();
			break;
		default:
			break;
		}
	}

private:
	float m_WaitTime;
	float m_ElapsedTime;
	TimeMode m_TimeMode;
};