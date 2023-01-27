workspace "Emulator"
   architecture "x64"
   configurations { "Debug", "Release" }
   startproject "Emulator"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group ""
   include "Emulator"
   include "Assembler"

group "Dependancies"
   include "vendor/glfw"
   include "vendor/glad"
   include "vendor/imgui"