//
// Created by jwraynor on 8/21/2023.
//

#pragma once

#include "defines.h"

typedef struct platform_state {
    void *internal_state;
} platform_state;

/**
 * Internally initializes the platform state and returns it. This is per platform and should be called
 * by the platform specific code.
 * @param state The platform state to initialize
 * @param str The title of the window
 * @param x The x position of the window
 * @param y The y position of the window
 * @param width The width of the window
 * @param height The height of the window
 * @return  Whether or not the platform was successfully initialized
 */
b8 platform_startup(platform_state *state, string str, i32 x, i32 y, i32 width, i32 height);

/**
 * Internally shuts down the platform state. This is per platform and should be called
 * by the platform specific code.
 * @param state The platform state to shut down
 */
void platform_shutdown(platform_state *state);

/**
 * Allocates memory on the platform specific heap.
 * @param size  The size of the memory to allocate
 * @param aligned  Whether or not the memory should be aligned
 * @return  The pointer to the allocated memory
 */
void *platform_allocate(u64 size, b8 aligned);

/**
 * Frees memory on the platform specific heap. This should be called for every call to platform_allocate.
 * @param ptr The pointer to the memory to free
 * @param aligned Whether or not the memory was aligned
 * @return  The pointer to the allocated memory
 */
void platform_free(void *ptr, b8 aligned);

/**
 * Zeroes out the memory at the given pointer for the given size.
 * @param ptr The pointer to the memory to zero out
 * @param size The size of the memory to zero out
 * @return  The pointer to the memory
 */
void *platform_zero_memory(void *ptr, u64 size);

/**
 * Copies memory from src to dest for the given size. The memory should not overlap.
 * @param dest the destination pointer
 * @param src  the source pointer
 * @param size  the size of the memory to copy
 * @return  the destination pointer
 */
void *platform_copy_memory(void *dest, const void *src, u64 size);

/**
 * Sets the memory at the given pointer to the given value for the given size.
 * @param dest The pointer to the memory to set
 * @param value The value to set the memory to
 * @param size The size of the memory to set
 * @return The pointer to the memory
 */
void *platform_set_memory(void *dest, i32 value, u64 size);

/**
 * Returns the current time in milliseconds
 * @return The current time in milliseconds
 */
f64 platform_get_time();

/**
 * Sleeps the current thread for the given number of milliseconds
 * @param ms  The number of milliseconds to sleep
 */
void platform_sleep(u64 ms);