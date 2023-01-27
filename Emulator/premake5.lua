project "Emulator"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/obj/" .. outputdir .. "/%{prj.name}")
	
	files { "src/**.cpp", "src/**.h" }
	
	includedirs {
		"%{prj.location}/src",
		"%{wks.location}/vendor/glfw/include",
		"%{wks.location}/vendor/glad/include",
		"%{wks.location}/vendor/glm",
		"%{wks.location}/vendor/imgui",
		"%{wks.location}/vendor/stb_image",
	}
	
	links {
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32",
	}
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
	
	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		symbols "On"