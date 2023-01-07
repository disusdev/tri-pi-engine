printf ("Premake generation started...")

LANG = "C++"
BUILD = ""
BIN_DIR = path.join(_MAIN_SCRIPT_DIR, ".bin/%{BUILD}")
LIB_DIR = path.join(_MAIN_SCRIPT_DIR, ".lib/%{BUILD}")
PROJ_DIR = path.join(_MAIN_SCRIPT_DIR, ".proj/%{BUILD}")
OBJ_DIR = path.join(_MAIN_SCRIPT_DIR, ".obj/%{BUILD}")

function configuration_settings()
  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"
  
  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"
end

function system_settings()
  filter {"system:windows", "action:vs*"}
    systemversion("latest")
end

function static_lib_header()
  kind "StaticLib"
  configuration_settings()
  system_settings()
  location (PROJ_DIR)
  language (LANG)
  targetdir (LIB_DIR)
  cppdialect "C++20"
  defines { "LIBRARY_EXPORTS" }
end

function shared_lib_header()
  kind "SharedLib"
  configuration_settings()
  system_settings()
  location (PROJ_DIR)
  language (LANG)
  targetdir (BIN_DIR)
  defines { "LIBRARY_EXPORTS" }
end

function console_app_header()
  kind "ConsoleApp"
  configuration_settings()
  system_settings()
  location (PROJ_DIR)
  language (LANG)
  cppdialect "C++20"
  targetdir (BIN_DIR)
  libdirs {LIB_DIR}
end

toolset "clang"

workspace "tri-pi-engine"
  BUILD = ""

  objdir (OBJ_DIR)


  startproject "game"
  
  configurations { "Debug", "Release" }

  -- Only Windows
  system "windows"
  
  -- Only x64
  architecture "x86_64"
  
	-- libs (nolibs)
	-- include("deps/tools/meshoptimizer_win32.lua")
  
	-- prog
  include("engine/engine.lua")
	include("editor/editor.lua")

printf ("Generation finished.")