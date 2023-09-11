#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Bobo
{
	class BOBO_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};
}

// Engine Log Macros

#define BOBO_CORE_ERROR(...)  ::Bobo::Log::GetCoreLogger()->error(__VA_ARGS__)
#define BOBO_CORE_WARN(...)   ::Bobo::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BOBO_CORE_INFO(...)   ::Bobo::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BOBO_CORE_TRACE(...)  ::Bobo::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define BOBO_CORE_FATAL(...)  ::Bobo::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Game Log Macros

#define BOBO_ERROR(...)       ::Bobo::Log::GetClientLogger()->error(__VA_ARGS__)
#define BOBO_WARN(...)        ::Bobo::Log::GetClientLogger()->warn(__VA_ARGS__)
#define BOBO_INFO(...)        ::Bobo::Log::GetClientLogger()->info(__VA_ARGS__)
#define BOBO_TRACE(...)       ::Bobo::Log::GetClientLogger()->trace(__VA_ARGS__)
#define BOBO_FATAL(...)       ::Bobo::Log::GetClientLogger()->fatal(__VA_ARGS__)