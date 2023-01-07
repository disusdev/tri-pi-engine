project "editor"
  printf ("[editor] generating...")
  console_app_header()
  insert_engine_lib()

  includedirs
  {
    "../engine/src",
    "../external/vulkan/include",
    "../external/glfw/include",
    "../external/gli",
    "../external/glad/include",
    "../external/stb",
    "../external/glm",
    "../external/imgui",
    "../external/ImGuizmo",
    "../external/glslang/glslang/Include",
    "../external/assimp/include",
    "../external/glslang",
    "../external/volk"
  }

  files { "src/**.h", "src/**.hpp", "src/**.cpp" }