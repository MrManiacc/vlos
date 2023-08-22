#pragma once

#include "defines.h"
#include "core/asserts.h"
#include <vulkan/vulkan.h>


#define VK_CHECK(expr)               \
    {                                \
        VASSERT(expr == VK_SUCCESS); \
    }

/**
 * Helps us track the swapchain support for a given device
 */
typedef struct vulkan_swapchain_support_info {
    VkSurfaceCapabilitiesKHR capabilities;
    u32 format_count;
    VkSurfaceFormatKHR *formats;
    u32 present_mode_count;
    VkPresentModeKHR *present_modes;

} vulkan_swapchain_support_info;

/**
 * Internally tracks our vulkan device state
 */
typedef struct vulkan_device {
    VkPhysicalDevice physical_device;
    VkDevice logical_device;
} vulkan_device;

/**
 * Internally track the global vulkan context state
 */
typedef struct vulkan_context {

    /**
     * The actual vulkan instance it's self
     */
    VkInstance instance;

    /**
     * Allow for manual allocation of memory
     */
    VkAllocationCallbacks *allocator;

    /**
     * Allows for the management of the surface. Also known as the window
     */
    VkSurfaceKHR surface;

    /**
     * If we're on a debug build, we should use our debug messenger
     */
#if defined(DEBUG_BUILD)
    VkDebugUtilsMessengerEXT debug_messenger;
#endif

    /**
     * Store a reference to our vulkan device
     */
    vulkan_device device;

} vulkan_context;


