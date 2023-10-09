#pragma once

// std Libraries

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <typeindex>
#include <type_traits>
#include <chrono>
#include <thread>
#include <filesystem>

// std Data Structures

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

// Local

#include "engine/Log.h"
#include "engine/Macro.h"

#ifdef BOBO_PLATFORM_WINDOWS
	#include <Windows.h>
#endif