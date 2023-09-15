#pragma once

#include "bobopch.h"

/* ------ Engine Macros ------ */

#ifdef BOBO_BUILD_DLL
	#define BOBO_CORE_ERROR(...)  ::Bobo::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define BOBO_CORE_WARN(...)   ::Bobo::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define BOBO_CORE_INFO(...)   ::Bobo::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define BOBO_CORE_TRACE(...)  ::Bobo::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define BOBO_CORE_FATAL(...)  ::Bobo::Log::GetCoreLogger()->fatal(__VA_ARGS__)
	#define BOBO_ASSERT(x, msg)	  if (!x) { BOBO_CORE_ERROR(msg); throw std::runtime_error("Assert failed"); };
#endif

/* ------ Game Macros ------ */

#define Log(...) Bobo::Log::GetClientLogger()->trace(__VA_ARGS__)