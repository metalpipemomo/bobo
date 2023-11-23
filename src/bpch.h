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
#include <cmath>
#include <cstdlib>
#include <limits>
#include <optional>
#include <cstdint>
#include <cstddef>
#include <mutex>

// std Data Structures

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <set>
#include <cstring>

// Local

#include "engine/Log.h"
#include "engine/Macro.h"

#ifdef BOBO_PLATFORM_WINDOWS
	#include <Windows.h>
#endif