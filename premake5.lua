workspace "Bobo"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }
    startproject "bobo_game"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "bobo_engine/external/GLFW/include"
IncludeDir["glm"] = "bobo_engine/external/glm"

include "bobo_engine/external/GLFW"

project "bobo_engine"
    location "bobo_engine"
    kind "SharedLib"

    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "bobopch.h"
    pchsource "bobo_engine/src/bobopch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/external/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glm}"
    }

    links
    {
        "GLFW",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "off"
        systemversion "latest"

        defines
        {
            "BOBO_PLATFORM_WINDOWS",
            "BOBO_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/bobo_game/\"")
        }

    filter "configurations:Debug"
        defines "BOBO_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "BOBO_RELEASE"
        optimize "On"
        
    filter "configurations:Dist"
        defines "BOBO_DIST"
        optimize "On"

project "bobo_game"
    location "bobo_game"
    kind "ConsoleApp"

    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "bobo_engine/src",
        "bobo_engine/external/spdlog/include",
        "%{IncludeDir.glm}"
    }

    links
    {
        "bobo_engine",
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "off"
        systemversion "latest"

        defines
        {
            "BOBO_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "BOBO_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "BOBO_RELEASE"
        optimize "On"
    
    filter "configurations:Dist"
        defines "BOBO_DIST"
        optimize "On"