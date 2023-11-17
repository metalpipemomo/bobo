#pragma once

#include "bpch.h"

/* ------ Engine Macros ------ */

#ifdef BOBO_DEBUG
#define BOBO_ERROR(...) Log::GetEngineLogger()->error(__VA_ARGS__)
#define BOBO_WARN(...) Log::GetEngineLogger()->warn(__VA_ARGS__)
#define BOBO_INFO(...) Log::GetEngineLogger()->info(__VA_ARGS__)
#define BOBO_TRACE(...) Log::GetEngineLogger()->trace(__VA_ARGS__)
#define BOBO_FATAL(...) Log::GetEngineLogger()->fatal(__VA_ARGS__)
#define BOBO_ASSERT(x, msg) if (!x) { BOBO_ERROR(msg); throw std::runtime_error("Assertion failed"); }; 
#endif 

#ifdef BOBO_RELEASE
#define BOBO_ERROR(...)
#define BOBO_WARN(...) 
#define BOBO_INFO(...) 
#define BOBO_TRACE(...)
#define BOBO_FATAL(...)
#define BOBO_ASSERT(x, msg)
#endif

/* ------ Game Macros ------ */

#define Log(...) Log::GetGameLogger()->trace(__VA_ARGS__)