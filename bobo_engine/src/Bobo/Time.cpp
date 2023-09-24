#include "bobopch.h"

#include "Time.h"

namespace Bobo
{
	Time::Time()
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

		BOBO_INFO("Time initialized...");
	}

	void Time::Update()
	{
		// Resets the frame after Fixed Update Occurs
		m_DidFixedUpdate = false;

		// Get the current time
		m_CurrentTime = std::chrono::high_resolution_clock::now();
		
		++m_FrameCount;

		// Calculate the time difference between the current and previous frames
		std::chrono::duration<float> deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(m_CurrentTime - m_PreviousTime);
		m_DeltaTime = deltaTime.count() * m_TimeScale;
		m_UnscaledDeltaTime = deltaTime.count();

		// Fixed Time
		std::chrono::duration<float> fixedDeltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(m_CurrentTime - m_PreviousFixedTime);
		if (fixedDeltaTime.count() >= m_FixedTimeStep)
		{
			m_PreviousFixedTime = m_CurrentTime;
			m_FixedDeltaTime = fixedDeltaTime.count();
			m_DidFixedUpdate = true;
		}

		// Tracking Time Elapsed
		m_RealtimeSinceStartup += m_UnscaledDeltaTime;
		m_RealtimeSinceSceneChange += m_UnscaledDeltaTime;
		BOBO_INFO("Time Scale: {}, Since Startup: {}, Since Scene Change: {}", m_TimeScale, 
			m_RealtimeSinceStartup, m_RealtimeSinceSceneChange);

		// Update the previous time for the next frame
		m_PreviousTime = m_CurrentTime;
	}

	void Time::SetTimeScale(float timeScale)
	{
		if (timeScale < 0)
		{
			BOBO_WARN("Attempt to Set a Negative Timescale was Blocked");
			return;
		}
		m_TimeScale = timeScale;
	}

	void Time::StartCallAfterTime(CallbackContainer callback, float wait)
	{
		std::thread callAfterTime = std::thread(&Time::CallAfterTime, this, callback, wait);
		callAfterTime.detach();
	}

	void Time::CallAfterTime(CallbackContainer callback, float wait)
	{
		std::chrono::duration<float> waitDuration(wait);
		std::this_thread::sleep_for(waitDuration);
		callback.m_Callback();
	}
}