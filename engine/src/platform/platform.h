#pragma once

#include "../defines.h"

struct Resolution
{
	u32 width  = 0;
	u32 height = 0;
};

int
platform_init(i32 width, i32 height, Resolution* resolution = nullptr);

void
platform_term();

void* platform_allocate(
  u64 size,
  u8 aligned
);

void platform_free(
  void* block,
  u8 aligned
);

void* platform_zero_memory(
  void* block,
  u64 size
);

void* platform_copy_memory(
  void* dest,
  const void* source,
  u64 size
);

void* platform_set_memory(
  void* dest,
  i32 value,
  u64 size
);

void platform_console_write(const char* message, u8 color);
void platform_console_write_error(const char* message, u8 color);

double platform_get_absolute_time();

void platform_sleep(u64 ms);

void
logger_impl(const char* msg);

void
platform_poll_events();

void
platform_draw_frame();