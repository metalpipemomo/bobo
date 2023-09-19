#pragma once

#include "bobopch.h"

/* ------ Engine Macros ------ */

#ifdef BOBO_BUILD_DLL
	#ifdef BOBO_DEBUG
		#define BOBO_ERROR(...)  ::Bobo::Log::GetCoreLogger()->error(__VA_ARGS__)
		#define BOBO_WARN(...)   ::Bobo::Log::GetCoreLogger()->warn(__VA_ARGS__)
		#define BOBO_INFO(...)   ::Bobo::Log::GetCoreLogger()->info(__VA_ARGS__)
		#define BOBO_TRACE(...)  ::Bobo::Log::GetCoreLogger()->trace(__VA_ARGS__)
		#define BOBO_FATAL(...)  ::Bobo::Log::GetCoreLogger()->fatal(__VA_ARGS__)
		#define BOBO_ASSERT(x, msg)	  if (!x) { BOBO_ERROR(msg); throw std::runtime_error("Assert failed"); };
	#endif
#endif

/* ------ Game Macros ------ */

#define Log(...) Bobo::Log::GetClientLogger()->trace(__VA_ARGS__)