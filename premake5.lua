workspace "Bobo"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    startproject "bobo"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "bobo/libs/GLFW/include"
IncludeDir["Glad"] = "bobo/libs/Glad/include"
IncludeDir["glm"] = "bobo/libs/glm"
IncludeDir["FMODApi"] = "bobo/libs/FMODAPI/api/core/inc"
IncludeDir["FMODStudio"] = "bobo/libs/FMODAPI/api/studio/inc"
IncludeDir["Bullet3D"] = "bobo/libs/bullet3d/src"
IncludeDir["ImGui"] = "bobo/libs/imgui"

include "bobo/libs/GLFW"
include "bobo/libs/Glad"

project "bobo"
    location "bobo"
    kind "ConsoleApp"

    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "bpch.h"
    pchsource "bobo/src/bpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
		"%{prj.name}/libs/imgui/*.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/libs/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.glm}",
		"%{IncludeDir.FMODApi}",
		"%{IncludeDir.FMODStudio}",
        "%{IncludeDir.Bullet3D}",
		"%{IncludeDir.ImGui}"
    }

    links
    {
        "GLFW",
        "Glad",
        "opengl32.lib",
		"bobo/libs/FMODAPI/api/core/lib/x64/fmod_vc.lib",
		"bobo/libs/FMODAPI/api/studio/lib/x64/fmodstudio_vc.lib"
    }

	filter "files:bobo/libs/imgui/*"
		flags {"NoPCH"}

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "off"
        systemversion "latest"

        defines
        {
            "BOBO_PLATFORM_WINDOWS",
            "GLFW_INCLUDE_NONE"
        }

        libdirs
        {
            "bobo/libs/FMODAPI/api/core/lib",
			"bobo/libs/FMODAPI/api/studio/lib"
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