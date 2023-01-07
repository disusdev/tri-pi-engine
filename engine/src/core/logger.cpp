#include "../defines.h"

#include "../platform/platform.h"
#include "logger.h"
#include "asserts.h"

#include <stdio.h>
#include <stdarg.h>

typedef struct logger_system_state
{
  b8 initialize;
} logger_system_state;

static logger_system_state* state_ptr;

u8
logger_init(u64* mem_requirement, void* state)
{
  *mem_requirement = sizeof(logger_system_state);
  if (state == 0)
  {
    return true;
  }

  state_ptr = (logger_system_state*)state;
  state_ptr->initialize = true;
  return true;
}

void
logger_term(void* state)
{
  state_ptr = 0;
}

void
logger_output(
  LOG_LEVEL level,
  const char* msg,
  ...)
{
  const char* level_strings[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};
  u8 is_error = level < LOG_LEVEL_WARN;

  const i32 msg_length = 32000;
  static char s_printf_buf[msg_length];
  
  va_list args;
  va_start(args, msg);
  _vsnprintf(s_printf_buf, sizeof(s_printf_buf), msg, args);
  va_end(args);

  char s_sprintf_buf[msg_length];
  sprintf(s_sprintf_buf, "%s%s\n", level_strings[level], s_printf_buf);

  if (is_error)
  {
    platform_console_write_error(s_sprintf_buf, (u8)level);
  }
  else
  {
    platform_console_write(s_sprintf_buf, (u8)level);
  }
}

void
report_assertion_failure(
  const char* expression,
  const char* msg,
  const char* file,
  int line)
{
  logger_output(
    LOG_LEVEL_FATAL,
    "AssertionFailure: %s, message: '%s', in file: %s, line: %d\n",
    expression, msg, file, line
  );
}