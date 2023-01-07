local engine_dir = path.join(_SCRIPT_DIR, "src")
function insert_engine_lib()
  includedirs {engine_dir}
  links {"engine"}
end

project "engine"
  printf ("[engine] generating...")
  static_lib_header()
  includedirs
  {
    engine_dir,
    "../external/vulkan/include",
    "../external/glfw/include",
    "../external/gli",
    "../external/glad/include",
    "../external/stb",
    "../external/glm",
    "../external/imgui",
    "../external/ImGuizmo",
    "../external/glslang/glslang/Include",
    "../external/glslang",
    "../external/volk",
    "../external/glslang/OGLCompilersDLL"
  }
  files
  {
    "src/**.h",
    "src/**.hpp",
    "src/**.cpp",
    "src/**.c",
    "src/**.inl",
    "../external/glfw/cglfw.c",
    "../external/glad/src/*.c",
    "../external/ImGuizmo/*.cpp",
    "../external/ImGuizmo/*.h",
    "../external/imgui/*.cpp",
    "../external/imgui/*.h",
    "../external/imgui/backends/imgui_impl_vulkan.cpp",
    "../external/imgui/backends/imgui_impl_vulkan.h",
    "../external/imgui/backends/imgui_impl_glfw.cpp",
    "../external/imgui/backends/imgui_impl_glfw.h",
    "../external/glslang/glslang/GenericCodeGen/*.cpp",
    "../external/glslang/glslang/MachineIndependent/**.cpp",
    "../external/glslang/glslang/CInterface/*.cpp",
    "../external/glslang/SPIRV/**.cpp",
    "../external/glslang/OGLCompilersDLL/**.cpp",
    "../external/glslang/glslang/OSDependent/Windows/**.cpp",
    "../external/glslang/StandAlone/ResourceLimits.cpp",
    "../external/volk/volk.c",
    "../external/volk/volk.h"
  }