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
IncludeDir["Glad"] = "bobo_engine/external/Glad/include"
IncludeDir["glm"] = "bobo_engine/external/glm"
IncludeDir["FMODApi"] = "bobo_engine/external/FMODAPI/api/core/inc"
IncludeDir["FMODStudio"] = "bobo_engine/external/FMODAPI/api/studio/inc"

include "bobo_engine/external/GLFW"
include "bobo_engine/external/Glad"

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
        "%{IncludeDir.Glad}",
        "%{IncludeDir.glm}",
		"%{IncludeDir.FMODApi}",
		"%{IncludeDir.FMODStudio}"
    }

    links
    {
        "GLFW",
        "Glad",
        "opengl32.lib",
		"bobo_engine/external/FMODAPI/api/core/lib/x64/fmod_vc.lib",
		"bobo_engine/external/FMODAPI/api/studio/lib/x64/fmodstudio_vc.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "off"
        systemversion "latest"

        defines
        {
            "BOBO_PLATFORM_WINDOWS",
            "BOBO_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }
		
		libdirs 
		{
			"bobo_engine/external/FMODAPI/api/core/lib",
			"bobo_engine/external/FMODAPI/api/studio/lib"
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
        "%{IncludeDir.glm}",
		"%{IncludeDir.FMODApi}",
		"%{IncludeDir.FMODStudio}"
    }

    links
    {
        "bobo_engine",
		"./bobo_engine/external/FMODAPI/api/core/lib/x64/fmod_vc.lib",
		"./bobo_engine/external/FMODAPI/api/studio/lib/x64/fmodstudio_vc.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "off"
        systemversion "latest"

        defines
        {
            "BOBO_PLATFORM_WINDOWS"
        }

		libdirs 
		{
			"./bobo_engine/external/FMODAPI/api/core/lib",
			"./bobo_engine/external/FMODAPI/api/studio/lib"
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