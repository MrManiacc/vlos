
#pragma once

#include "defines.h"
/**
 * The memory tag is used to track memory allocations. This is used to track memory leaks.
 */
typedef enum mem_tag {
  MEM_TAG_ARRAY,
  MEM_TAG_DARRAY,
  MEM_TAG_DICT,
  MEM_TAG_RING_QUEUE,
  MEM_TAG_BST,
  MEM_TAG_APPLICATION,
  MEM_TAG_STRING,
  MEM_TAG_JOB,
  MEM_TAG_TEXTURE,
  MEM_TAG_MAT_INST,
  MEM_TAG_RENDERER,
  MEM_TAG_TRANSFORM,
  MEM_TAG_ENTITY,
  MEM_TAG_ENTITY_NODE,
  MEM_TAG_SCENE,
  MEM_TAG_MATERIAL,
  MEM_TAG_MESH,
  MEM_TAG_FLAGGED, //Also used as a marker for the total number of tags
} mem_tag;

/**
 * Initializes the memory system. This should be called before any other memory functions.
 */
VAPI void mem_init();

/**
 * Shutsdown the memory system. This should be called after all other memory functions.
 */
VAPI void mem_shutdown();

/**
 * Allocates memory on the platform specific heap.
 * @param size  The size of the memory to allocate
 * @param tag The tag to associate with the memory
 * @return  The pointer to the allocated memory
 */
VAPI void* mem_alloc(u64 size, mem_tag tag);

/**
 * Frees memory on the platform specific heap. This should be called for every call to platform_allocate.
 * @param pointer The pointer to the memory to free
 * @param size The size of the memory to free (for tracking purposes)
 * @param tag The tag to associate with the memory
 * @return  The pointer to the allocated memory
 */
VAPI void mem_free(void*pointer, u64 size, mem_tag tag);

/**
 * Zeroes out the memory at the given pointer for the given size.
 * @param pointer The pointer to the memory to zero out
 * @param size The size of the memory to zero out
 * @return  The pointer to the memory
 */
VAPI void* mem_zero(void*pointer, u64 size);

/**
 * Copies the memory from the source to the destination for the given size.
 * @param dest The destination to copy to
 * @param src The source to copy from
 * @param size The size of the memory to copy
 * @return  The pointer to the memory
 */
VAPI void* mem_copy(void*dest, constptr src, u64 size);

/**
 * Sets the memory at the given pointer to the given value for the given size.
 * @param dest_ptr The pointer to the memory to set
 * @param value The value to set the memory to
 * @param size The size of the memory to set
 * @return The pointer to the memory
 */
VAPI void* mem_set(void*dest_ptr, i32 value, u64 size);


/**
* Gets the debug memory usage string. This is used to track memory leaks.
*/
VAPI str mem_usage_str();
