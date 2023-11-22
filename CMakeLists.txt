cmake_minimum_required(VERSION 3.25)

if(NOT "${CMAKE_BINARY_DIR}" MATCHES "/build$")
    message(FATAL_ERROR "Please create a 'build' directory and run CMake from there. This helps to keep the source directory clean.")
endif()

project(bobo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Disable GLFW's unnecessary build targets
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)


# Add externals
add_subdirectory(libs/glfw)
add_subdirectory(libs/glad)
add_subdirectory(libs/spdlog)
add_subdirectory(libs/Jolt/build)
include_directories(libs/glm)
include_directories(libs/glfw/include)
add_subdirectory(libs/imgui)
include_directories(libs/stb)
include_directories(libs/tinyobjloader)
include_directories(libs/FMODAPI/api/core/inc)
include_directories(libs/FMODAPI/api/studio/inc)
link_directories(libs/FMODAPI/api/core/lib/x64)
link_directories(libs/FMODAPI/api/studio/lib/x64)

file(GLOB_RECURSE ENGINE_SOURCES "src/engine/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "src/engine/*.h")
file(GLOB_RECURSE GAME_SOURCES "src/game/*.cpp")
file(GLOB_RECURSE GAME_HEADERS "src/game/*.h")

# Define your executable target
add_executable(bobo
    ${ENGINE_HEADERS}
    ${ENGINE_SOURCES}
    ${GAME_HEADERS}
    ${GAME_SOURCES}
    src/main.cpp)

    # Preprocessor Macro
if (NOT MSVC)
    target_compile_definitions(bobo PRIVATE MAC_TARGET)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(bobo PRIVATE BOBO_DEBUG)
endif()

target_compile_definitions(bobo PRIVATE GLFW_INCLUDE_NONE)

# Handle linking
target_link_libraries(bobo glfw glad imgui spdlog::spdlog Jolt fmod_vc fmodstudio_vc)

target_include_directories(imgui PRIVATE
    # Include GLFW headers
    "libs/glfw/include"
)

# PCH
target_precompile_headers(bobo PRIVATE src/bpch.h)

# Get the absolute path to the 'libs' directory
get_filename_component(FMOD_LIBS_PATH libs/FMODAPI ABSOLUTE)

# Then use this absolute path in your custom command
add_custom_command(TARGET bobo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${FMOD_LIBS_PATH}/api/core/lib/x64/fmod.dll"
    $<TARGET_FILE_DIR:bobo>)

add_custom_command(TARGET bobo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${FMOD_LIBS_PATH}/api/studio/lib/x64/fmodstudio.dll"
    $<TARGET_FILE_DIR:bobo>)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT bobo)

if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
                    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
                    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json)
endif()