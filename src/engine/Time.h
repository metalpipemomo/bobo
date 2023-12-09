#pragma once

#include "../bpch.h"

class Time
{
public:
	static void Init()
	{
		GetInstance();
	}

	static void Update()
	{
		auto t = GetInstance();

		// Resets the frame after Fixed Update Occurs
		t->m_DidFixedUpdate = false;

		// Get the current time
		t->m_CurrentTime = std::chrono::high_resolution_clock::now();

		++t->m_FrameCount;

		// Calculate the time difference between the current and previous frames
		std::chrono::duration<float> deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(t->m_CurrentTime - t->m_PreviousTime);
		t->m_DeltaTime = deltaTime.count() * t->m_TimeScale;
		t->m_UnscaledDeltaTime = deltaTime.count();

		// Fixed Time
		std::chrono::duration<float> fixedDeltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(t->m_CurrentTime - t->m_PreviousFixedTime);
		if (fixedDeltaTime.count() >= t->m_FixedTimeStep)
		{
			t->m_PreviousFixedTime = t->m_CurrentTime;
			t->m_FixedDeltaTime = fixedDeltaTime.count();
			t->m_DidFixedUpdate = true;
		}

		// Tracking Time Elapsed
		t->m_RealtimeSinceStartup += t->m_UnscaledDeltaTime;
		t->m_RealtimeSinceSceneChange += t->m_UnscaledDeltaTime;


		// Update the previous time for the next frame
		t->m_PreviousTime = t->m_CurrentTime;
	}

	static void SetTimeScale(float timeScale)
	{
		auto t = GetInstance();
		if (timeScale < 0)
		{
			BOBO_WARN("Attempt to Set a Negative Timescale was ignored");
			return;
		}
		t->m_TimeScale = timeScale;
	}

	static void ResetTimeSinceLoadScene() { GetInstance()->m_RealtimeSinceSceneChange = 0; }
	static bool DidFixedUpdate() { return GetInstance()->m_DidFixedUpdate; }
	static float RealTimeSinceStartup() { return GetInstance()->m_RealtimeSinceStartup; }
	static float RealTimeSinceSceneChange() { return GetInstance()->m_RealtimeSinceSceneChange; }
	static float FrameCount() { return GetInstance()->m_FrameCount; }
	static float DeltaTime() { return GetInstance()->m_DeltaTime; }
	static float UnscaledDeltaTime() { return GetInstance()->m_UnscaledDeltaTime; }
	static float FixedDeltaTime() { return GetInstance()->m_FixedDeltaTime; }
	static float GetTimeScale() { return GetInstance()->m_TimeScale; }

private:
	static Time* GetInstance()
	{
		static Time* instance = new Time();
		return instance;
	}
	Time()
	{
		m_TimeAtStartup = std::chrono::high_resolution_clock::now();
		m_CurrentTime = std::chrono::high_resolution_clock::now();
		m_PreviousTime = std::chrono::high_resolution_clock::now();
		m_PreviousFixedTime = std::chrono::high_resolution_clock::now();

		m_TimeScale = 1;
		m_DidFixedUpdate = false;
		m_FrameCount = 0;
		m_RealtimeSinceSceneChange = 0;
		m_RealtimeSinceStartup = 0;
		m_FixedTimeStep = .02;

		m_DeltaTime = 0;
		m_UnscaledDeltaTime = 0;
		m_FixedDeltaTime = 0;

		BOBO_INFO("Time initialized!");
	}

	std::chrono::high_resolution_clock::time_point m_TimeAtStartup;
	std::chrono::high_resolution_clock::time_point m_PreviousTime;
	std::chrono::high_resolution_clock::time_point m_PreviousFixedTime;
	std::chrono::high_resolution_clock::time_point m_CurrentTime;
	float m_TimeScale;
	float m_DeltaTime;
	float m_FixedDeltaTime;
	float m_UnscaledDeltaTime;
	float m_FixedTimeStep;
	unsigned int m_FrameCount;
	float m_RealtimeSinceStartup;
	float m_RealtimeSinceSceneChange;
	bool m_DidFixedUpdate;
};