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
add_subdirectory(libs/imgui)
add_subdirectory(libs/spdlog)
add_subdirectory(libs/jolt/build)
include_directories(libs/glm)
include_directories(libs/stb)
include_directories(libs/tinyobjloader)

# Define your executable target
add_executable(bobo
    src/main.cpp)

    # Preprocessor Macro
if (NOT MSVC)
    target_compile_definitions(bobo PRIVATE MAC_TARGET)
endif()

target_compile_definitions(bobo PRIVATE GLFW_INCLUDE_NONE)

# Handle linking
target_link_libraries(bobo glfw glad imgui spdlog::spdlog Jolt)

# PCH
target_precompile_headers(bobo PRIVATE src/bpch.h)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT bobo)

if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
                    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
                    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json)
endif()