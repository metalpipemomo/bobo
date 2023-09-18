#pragma once

// std Libraries

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <typeindex>

// std Data Structures

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

// Local

#include "Bobo/Log.h"
#include "Bobo/Macro.h"

// Platform specific

#ifdef BOBO_PLATFORM_WINDOWS
	#include <Windows.h>
#endif