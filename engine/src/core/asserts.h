#pragma once

#include "../defines.h"

// [NEED_PLATFORM_IMPL] TODO: some static_assert on NEED_
// #define DEBUG_BREAK()

#if _MSC_VER
#include <intrin.h>
#define DEBUG_BREAK() __debugbreak()
#else
#define DEBUG_BREAK() __builtin_trap()
#endif

LIB_API void
report_assertion_failure
(
  const char* expression,
  const char* msg,
  const char* file,
  int line
);

#define ASSERT(expr_) {\
if (expr_)\
{}\
else\
{\
   report_assertion_failure(#expr_, "", __FILE__, __LINE__);\
   DEBUG_BREAK();\
}}

#define ASSERT_MSG(expr_, message_) {\
if (expr_)\
{}\
else\
{\
   report_assertion_failure(#expr_, message_, __FILE__, __LINE__);\
   DEBUG_BREAK();\
}}

#ifdef _DEBUG
#define ASSERT_DEBUG(expr_) {\
if (expr_)\
{}\
else\
{\
   report_assertion_failure(#expr_, "", __FILE__, __LINE__);\
   DEBUG_BREAK();\
}}
#else
#define ASSERT_DEBUG(expr_)
#endif