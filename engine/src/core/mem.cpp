#include "mem.h"

#include "logger.h"

#include "../platform/platform.h"

#include "str.h"

// T-254 custom string
// #include <string.h>
#include <stdio.h>

struct
mem_stats
{
  u64 total_allocated;
  u64 tagged_allocations[MEM_TAG_COUNT];
};

static const char*
memory_tag_strings[MEM_TAG_COUNT] =
{
  "UNKNOWN    ",
  "ARRAY      ",
  "LINEAR_ALLC"
  "DARRAY     ",
  "DICT       ",
  "RING_QUEUE ",
  "BST        ",
  "STRING     ",
  "APPLICATION",
  "JOB        ",
  "TEXTURE    ",
  "MAT_INST   ",
  "RENDERER   ",
  "GAME       ",
  "TRANSFORM  ",
  "ENTITY     ",
  "ENTITY_NODE",
  "SCENE      "
};

// static struct
// mem_state stats;

typedef struct
mem_system_state
{
  struct mem_stats stats;
  u64 alloc_count;
} mem_system_state;

static mem_system_state* state_ptr;

void
mem_init(u64* mem_requirement, void* state)
{
  *mem_requirement = sizeof(mem_system_state);
  if (state == 0)
  {
    return;
  }

  state_ptr = (mem_system_state*) state;
  state_ptr->alloc_count = 0;
  platform_zero_memory(&state_ptr->stats, sizeof(state_ptr->stats));
}

void
mem_term(void* state)
{
  state_ptr = 0;
}

void*
mem_alloc(u64 size, mem_tag tag)
{
  if (tag == MEM_TAG_UNKNOW)
  {
    LOG_WARN("mem_alloc called using MEMORY_TAG_UNKNOWN.");
  }

  if (state_ptr)
  {
    state_ptr->stats.total_allocated += size;
    state_ptr->stats.tagged_allocations[tag] += size;
    state_ptr->alloc_count++;
  }

  void* block = platform_allocate(size, false);
  platform_zero_memory(block, size);
  return block;
}

void mem_free(void* block, u64 size, mem_tag tag)
{
  if (tag == MEM_TAG_UNKNOW)
  {
      LOG_WARN("mem_free called using MEMORY_TAG_UNKNOWN.");
  }

  state_ptr->stats.total_allocated -= size;
  state_ptr->stats.tagged_allocations[tag] -= size;

  platform_free(block, 0);
}

void* mem_zero(void* block, u64 size)
{
  return platform_zero_memory(block, size);
}

void* mem_copy(void* dst, const void* src, u64 size)
{
  return platform_copy_memory(dst, src, size);
}

void* mem_set(void* dst, i32 value, u64 size)
{
  return platform_set_memory(dst, value, size);
}

char* get_memory_usage_str()
{
  const u64 gib = 1024 * 1024 * 1024;
  const u64 mib = 1024 * 1024;
  const u64 kib = 1024;

  char buffer[8000] = "System memory use (tagged):\n";
  u64 offset = string_length(buffer);

  for (u32 i = 0; i < MEM_TAG_COUNT; ++i)
  {
    char unit[4] = "XiB";
    float amount = 1.0f;
    if (state_ptr->stats.tagged_allocations[i] >= gib)
    {
      unit[0] = 'G';
      amount = state_ptr->stats.tagged_allocations[i] / (float)gib;
    } 
    else if (state_ptr->stats.tagged_allocations[i] >= mib)
    {
      unit[0] = 'M';
      amount = state_ptr->stats.tagged_allocations[i] / (float)mib;
    }
    else if (state_ptr->stats.tagged_allocations[i] >= kib)
    {
      unit[0] = 'K';
      amount = state_ptr->stats.tagged_allocations[i] / (float)kib;
    }
    else
    {
      unit[0] = 'B';
      unit[1] = 0;
      amount = (float)state_ptr->stats.tagged_allocations[i];
    }
    i32 length = snprintf(buffer + offset, 8000, "  %s: %.2f%s\n", memory_tag_strings[i], amount, unit);
    offset += length;
  }

  char* out_string = string_duplicate(buffer);
  return out_string;
}

u64 get_memory_alloc_count()
{
  if (state_ptr)
  {
    return state_ptr->alloc_count;
  }

  return 0;
}