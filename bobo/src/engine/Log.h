#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

class Log
{
public:
	static void Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		GetEngineLogger()->set_level(spdlog::level::trace);
		GetGameLogger()->set_level(spdlog::level::trace);
	}

	static std::shared_ptr<spdlog::logger>& GetEngineLogger()
	{
		static std::shared_ptr<spdlog::logger> s_EngineLogger = spdlog::stdout_color_mt("ENGINE");
		return s_EngineLogger;
	}
	static std::shared_ptr<spdlog::logger>& GetGameLogger()
	{
		static std::shared_ptr<spdlog::logger> s_GameLogger = spdlog::stdout_color_mt("GAME");
		return s_GameLogger;
	}

private:
};