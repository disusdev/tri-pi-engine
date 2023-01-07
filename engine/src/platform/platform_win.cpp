

#include "../defines.h"

#include "platform.h"

#include <windows.h>
#include <windowsx.h>

// #include "core/input.h"
// #include "core/event.h"
#include "../core/logger.h"

#include "../core/str.h"

// #include <glad/gl.h>
// #include <GLFW/glfw3.h>

#define VK_NO_PROTOTYPES
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../rendering/api/utils.h"
#include "../math/mathm_wrapper.hpp"

// #include "rendering/vk/vkUtils.h"
// #include "rendering/vk/vkResources.h"

// #include "core/input.h"

// #include "rendering/vk/vkApp.h"


// #include
// #include <glm/glm.hpp>
// #include <glm/ext.hpp>

int platform_init(i32 width, i32 height, Resolution* resolution)
{
 // glslang_initialize_process();

	//volkInitialize();

	//if (!glfwInit())
	//	exit(EXIT_FAILURE);

	//if (!glfwVulkanSupported())
	//	exit(EXIT_FAILURE);

	//glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//if (resolution)
	//{
	//	*resolution = detectResolution(width, height);
	//	width  = resolution->width;
	//	height = resolution->height;
	//}

	//state_ptr->window = glfwCreateWindow(width, height, "VulkanApp", nullptr, nullptr);
	//if (!state_ptr->window)
	//{
	//	glfwTerminate();
	//	exit(EXIT_FAILURE);
	//}

	//glfwSetMouseButtonCallback(
	//	state_ptr->window,
	//	gltf_set_mouse_button_callback
	//);

	//glfwSetKeyCallback(
	//	state_ptr->window,
	//	gltf_key_callback
	//);

  return true;
}

void
platform_term()
{
	//glslang_finalize_process();
	//glfwTerminate();
}

void* platform_allocate(
  u64 size,
  u8 aligned
)
{
  if (aligned == 1)
  {
    return _aligned_malloc(size, aligned);
  }
  else
  {
    return malloc(size);
  }
}

void platform_free(
  void* block,
  u8 aligned
)
{
  if (aligned == 1)
  {
    return _aligned_free(block);
  }
  else
  {
    return free(block);
  }
}

void* platform_zero_memory(
  void* block,
  u64 size
)
{
  return memset(block, 0, size);
}

void* platform_copy_memory(
  void* dest,
  const void* source,
  u64 size
)
{
  return memcpy(dest, source, size);
}

void* platform_set_memory(
  void* dest,
  i32 value,
  u64 size
)
{
  return memset(dest, value, size);
}

void platform_console_write(const char* message, u8 color)
{
  HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
  static u8 levels[6] = { 64, 4, 6, 2, 1, 8 };
  SetConsoleTextAttribute(console_handle, levels[color]);
  OutputDebugStringA(message);
  u64 length = string_length(message);
  LPDWORD number_written = 0;
  WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, number_written, 0);
  SetConsoleTextAttribute(console_handle, 7);
}

void platform_console_write_error(const char* message, u8 color)
{
  HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
  static u8 levels[6] = { 64, 4, 6, 2, 1, 8 };
  SetConsoleTextAttribute(console_handle, levels[color]);
  OutputDebugStringA(message);
  u64 length = string_length(message);
  LPDWORD number_written = 0;
  WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)length, number_written, 0);
  SetConsoleTextAttribute(console_handle, 7);
}

// double platform_get_absolute_time()
// {
//   LARGE_INTEGER time;
//   QueryPerformanceCounter(&time);
//   // double nanoseconds_per_count = 1.0e9 / static_cast<double>(frequency.QuadPart);
//   return (double)time.QuadPart * clock_frequency; // seconds
// }

 double platform_get_absolute_time()
 {
   return glfwGetTime();
 }

void platform_sleep(u64 ms)
{
  Sleep(ms);
}

void
logger_impl(const char* msg)
{
  AllocConsole();
  LPSTR lpBuff = (LPSTR)msg;
  DWORD dwSize = 0;
  WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), lpBuff, strlen(lpBuff), &dwSize, NULL);
}

void platform_poll_events()
{
	glfwPollEvents();
}
