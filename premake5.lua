workspace "Brbanje"

	architecture "x64"
	configurations {"Debug","Release","Dist"}
	startproject "Sandbox"

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


IncludeDir = {}
IncludeDir["GLFW"] = "Brbanje/Vendor/GLFW/Include"
IncludeDir["GLAD"] = "Brbanje/Vendor/GLAD/include"
IncludeDir["ImGui"] = "Brbanje/Vendor/imgui"
IncludeDir["glm"] = "Brbanje/Vendor/glm"

group "Dependencies"
include "Brbanje/Vendor/GLFW"
include "Brbanje/Vendor/GLAD"
include "Brbanje/Vendor/imgui"
group ""


project "Brbanje"
	location "Brbanje"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

	pchheader "brpch.h"
	pchsource "Brbanje/src/brpch.cpp"

	files{
		
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	defines{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs { "%{prj.name}/Vendor/spdlog/include", "%{prj.name}/src", 
					IncludeDir["GLFW"], IncludeDir["GLAD"], IncludeDir["ImGui"],IncludeDir["glm"]}

	links {"GLFW","GLAD","ImGui", "opengl32.lib"}

	filter "system:windows"
		
		staticruntime "on"
		systemversion "latest"
		defines{
			"BR_PLATFORM_WINDOWS",
			
			"GLFW_INCLUDE_NONE"
		}

		

	filter "configurations:Debug"
		defines "BR_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "BR_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "BR_DIST"
		runtime "Release"
		optimize "On"
			

project "Sandbox"
	
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

	files{
		
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs { "Brbanje/Vendor/spdlog/include", "Brbanje/src",
	IncludeDir["GLFW"], IncludeDir["GLAD"], IncludeDir["ImGui"], IncludeDir["glm"]}

	links {"Brbanje"}

	filter "system:windows"
		
		staticruntime "on"
		systemversion "latest"
		defines{
			"BR_PLATFORM_WINDOWS"
		}

		

	filter "configurations:Debug"
		defines "BR_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "BR_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "BR_DIST"
		runtime "Release"
		optimize "On"
	
project "Brbeetor"
	
	location "Brbeetor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

	files{
		
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs { "Brbanje/Vendor/spdlog/include", "Brbanje/src",
	IncludeDir["GLFW"], IncludeDir["GLAD"], IncludeDir["ImGui"], IncludeDir["glm"]}

	links {"Brbanje"}

	filter "system:windows"
		
		staticruntime "on"
		systemversion "latest"
		defines{
			"BR_PLATFORM_WINDOWS"
		}

		

	filter "configurations:Debug"
		defines "BR_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "BR_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "BR_DIST"
		runtime "Release"
		optimize "On"