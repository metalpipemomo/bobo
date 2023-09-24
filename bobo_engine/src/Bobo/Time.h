#pragma once

#include "bobopch.h"
#include <chrono>
#include <thread>

namespace Bobo
{
	class CallbackContainer
	{
	public:
		std::function<void()> m_Callback;
		CallbackContainer(std::function<void()> callback) : m_Callback(callback) {};
	};

	class Time
	{
	public:
		static Time& GetInstance()
		{
			static Time* instance = new Time();
			return *instance;
		}
		Time();
		void Update();
		void SetTimeScale(float timeScale);
		void ResetTimeSinceLoadScene() { m_RealtimeSinceSceneChange = 0; }
		bool DidFixedUpdate() { return m_DidFixedUpdate; }
		float RealTimeSinceStartup() { return m_RealtimeSinceStartup; }
		float RealTimeSinceSceneChange() { return m_RealtimeSinceSceneChange; }
		void StartCallAfterTime(CallbackContainer callback, float wait);
	private:
		void CallAfterTime(CallbackContainer callback, float wait);
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