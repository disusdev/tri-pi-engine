#include "str.h"
#include "mem.h"

#include <stdio.h>
#include <stdarg.h>

u64 string_length(const char* str)
{
  char* p = (char*)str;
  while (*p != '\0') p++;
  return (p - str);
}

char* string_duplicate(const char* str)
{
  u64 length = string_length(str);
  char* copy = (char*) mem_alloc(length + 1, MEM_TAG_STRING);
  mem_copy(copy, str, length + 1);
  return copy;
}

u8 strings_equal(const char* str0, const char* str1)
{
  char* p0 = (char*)str0;
  char* p1 = (char*)str1;
  int cmp = 0;
  while (cmp == 0 && (*p0 != '\0' || *p1 != '\0'))
  {
    cmp = *p0 - *p1;
    p0++; p1++;
  };
  return cmp == 0;
}

i32 string_format(char* dest, const char* format, ...)
{
  if (dest)
  {
    __builtin_va_list arg_ptr;
    va_start(arg_ptr, format);
    i32 written = string_format_v(dest, format, arg_ptr);
    va_end(arg_ptr);
    return written;
  }
  return -1;
}

i32 string_format_v(char* dest, const char* format, void* arg_list)
{
  if (dest)
  {
    char buffer[32000];
    i32 written = vsnprintf(buffer, 32000, format, (va_list)arg_list);
    buffer[written] = 0;
    mem_copy(dest, buffer, written + 1);

    return written;
  }
  return -1;
}