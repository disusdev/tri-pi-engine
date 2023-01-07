#pragma once

#include "../defines.h"

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

#if NDEBUG == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

typedef enum LOG_LEVEL
{
  LOG_LEVEL_FATAL = 0,
  LOG_LEVEL_ERROR = 1,
  LOG_LEVEL_WARN  = 2,
  LOG_LEVEL_INFO  = 3,
  LOG_LEVEL_DEBUG = 4,
  LOG_LEVEL_TRACE = 5,
} LOG_LEVEL;

LIB_API u8
logger_init(u64* mem_requirement, void* state);

LIB_API void
logger_term(void* state);

LIB_API void
logger_output(LOG_LEVEL level, const char* msg, ...);

#define LOG_FATAL(message_, ...) logger_output(LOG_LEVEL_FATAL, (message_), ##__VA_ARGS__)

#ifndef LOG_ERROR
#define LOG_ERROR(message_, ...) logger_output(LOG_LEVEL_ERROR, (message_), ##__VA_ARGS__)
#endif

#if LOG_WARN_ENABLED == 1
#define LOG_WARN(message_, ...) logger_output(LOG_LEVEL_WARN, (message_), ##__VA_ARGS__)
#else
#define LOG_WARN(message_, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
#define LOG_DEBUG(message_, ...) logger_output(LOG_LEVEL_DEBUG, (message_), ##__VA_ARGS__)
#else
#define LOG_DEBUG(message_, ...) printf("Problem?\n")
#endif

#if LOG_INFO_ENABLED == 1
#define LOG_INFO(message_, ...) logger_output(LOG_LEVEL_INFO, (message_), ##__VA_ARGS__)
#else
#define LOG_INFO(message_, ...)
#endif

#if LOG_TRACE_ENABLED == 1
#define LOG_TRACE(message_, ...) logger_output(LOG_LEVEL_TRACE, (message_), ##__VA_ARGS__)
#else
#define LOG_TRACE(message_, ...)
#endif