#pragma once

#include "../defines.h"

LIB_API u64 string_length(const char* str);

LIB_API char* string_duplicate(const char* str);

LIB_API u8 strings_equal(const char* str0, const char* str1);

LIB_API i32 string_format(char* dest, const char* format, ...);

LIB_API i32 string_format_v(char* dest, const char* format, void* va_list);