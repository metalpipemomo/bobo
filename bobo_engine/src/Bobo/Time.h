#pragma once

#include "bobopch.h"
#include <chrono>
#include <thread>

namespace Bobo
{
	class BOBO_API Time
	{
	public:
		static Time* GetInstance()
		{
			static Time instance = Time();
			return &instance;
		}
		Time();
		void Update();
		void SetTimeScale(float timeScale);
		void ResetTimeSinceLoadScene() { m_RealtimeSinceSceneChange = 0; }
		bool DidFixedUpdate() { return m_DidFixedUpdate; }
		float RealTimeSinceStartup() { return m_RealtimeSinceStartup; }
		float RealTimeSinceSceneChange() { return m_RealtimeSinceSceneChange; }
		float FrameCount() { return m_FrameCount; }
		float DeltaTime() { return m_DeltaTime; }
		float UnscaledDeltaTime() { return m_UnscaledDeltaTime; }
		float FixedDeltaTime() { return m_FixedDeltaTime; }
	private:
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
}