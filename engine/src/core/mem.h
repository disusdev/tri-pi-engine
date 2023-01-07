#pragma once

#include "../defines.h"

typedef enum mem_tag
{
  MEM_TAG_UNKNOW,
  MEM_TAG_ARRAY,
  MEM_TAG_LINEAR_ALLOCATOR,
  MEM_TAG_DARRAY,
  MEM_TAG_DICT,
  MEM_TAG_RING_QUEUE,
  MEM_TAG_BST,
  MEM_TAG_STRING,
  MEM_TAG_APPLICATION,
  MEM_TAG_JOB,
  MEM_TAG_TEXTURE,
  MEM_TAG_MATERIAL_INSTANCE,
  MEM_TAG_RENDERER,
  MEM_TAG_GAME,
  MEM_TAG_TRANSFORM,
  MEM_TAG_ENTITY,
  MEM_TAG_ENTITY_NODE,
  MEM_TAG_SCENE,

  MEM_TAG_COUNT
} mem_tag;

LIB_API void mem_init(u64* mem_requirement, void* state);
LIB_API void mem_term(void* state);

LIB_API void* mem_alloc(u64 size, mem_tag tag);

LIB_API void mem_free(void* block, u64 size, mem_tag tag);

LIB_API void* mem_zero(void* block, u64 size);

LIB_API void* mem_copy(void* dst, const void* src, u64 size);

LIB_API void* mem_set(void* dst, i32 value, u64 size);

LIB_API char* get_memory_usage_str();

LIB_API u64 get_memory_alloc_count();