#pragma once

#ifdef BOBO_PLATFORM_WINDOWS
	#ifdef BOBO_BUILD_DLL
		#define BOBO_API __declspec(dllexport)
	#else
		#define BOBO_API __declspec(dllimport)
	#endif
#else
	#error You can only peer at Bobo through a window(s operating system)!
#endif