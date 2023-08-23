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
  /**
   * The vulkan physical device
   */
  VkPhysicalDevice physical_device;

  /**
   * The vulkan logical device
   */
  VkDevice logical_device;

  /**
   * The swapchain support for the device
   */
  vulkan_swapchain_support_info swapchain_support;

  /**
   * The properties of the physical device
   */
  VkPhysicalDeviceProperties properties;
  /**
   * The features of the physical device
   */
  VkPhysicalDeviceFeatures features;
  /**
   * The memory properties of the physical device
   */
  VkPhysicalDeviceMemoryProperties memory;
  /**
   * The graphics queue for the device
   */
  VkQueue graphics_queue;
  /**
   * The present queue for the device
   */
  VkQueue present_queue;
  /**
   * The transfer queue for the device
   */
  VkQueue transfer_queue;
  /**
   * The index into the queue family for the graphics queue
   */
  u32 graphics_queue_index;
  /**
   * The index into the queue family for the present queue
   */
  u32 present_queue_index;

  /**
   * The index into the queue family for the transfer queue
   */
  u32 transfer_queue_index;

  /**
   * The index into the queue family for the compute queue
   */
  u32 compute_queue_index;

  /**
   * The depth format for the device
   */
  VkFormat depth_format;
} vulkan_device;

/**
 * Internally track the vulkan image state
 */
typedef struct vulkan_image {
  /**
   * The handle to the image
   */
  VkImage handle;
  /**
   * The memory for the image
   */
  VkDeviceMemory memory;
  /**
   * The image view for the image
   */
  VkImageView view;
  u32 width;
  u32 height;
} vulkan_image;

/**
 * Defines an internal state to track the renderpass
 */
typedef enum vulkan_render_pass_state {
  RENDER_PASS_READY, // The renderpass is ready to be used
  RENDER_PASS_RECORDING, // The renderpass is currently being recorded
  RENDER_PASS_IN_RENDER_PASS, // The renderpass is currently being used
  RENDER_PASS_RECORDING_ENDED, // The renderpass has finished recording
  RENDER_PASS_SUBMITTED, // The renderpass has been submitted to the queue
  RENDER_PASS_UNALLOCATED // The renderpass has not been allocated
} vulkan_render_pass_state;

/**
 * Internally track the renderpass state. This is used to track the state of the renderpass
 * and to ensure that we're not trying to use the renderpass in multiple places at once.
 */
typedef struct vulkan_render_pass {
  VkRenderPass handle; // The handle to the renderpass
  f32 x, y, w, h; // The viewport for the renderpass
  f32 r, g, b, a; // The clear color for the renderpass
  f32 depth; // The clear depth for the renderpass
  u32 stencil; // The clear stencil for the renderpass
  vulkan_render_pass_state state; // The state of the renderpass
} vulkan_render_pass;
/**
 * Tracks the state of the command buffer
 */
typedef enum vulkan_command_buffer_state {
  COMMAND_BUFFER_READY,
  COMMAND_BUFFER_RECORDING,
  COMMAND_BUFFER_IN_RENDER_PASS,
  COMMAND_BUFFER_RECORDING_ENDED,
  COMMAND_BUFFER_SUBMITTED,
  COMMAND_BUFFER_UNALLOCATED
} vulkan_command_buffer_state;

/**
 * Internally track the command buffer state. This is used to track the state of the command buffer
 * and to ensure that we're not trying to use the command buffer in multiple places at once.
 */

typedef struct vulkan_command_buffer {
  VkCommandBuffer handle; // The handle to the command buffer
  vulkan_command_buffer_state state; // The state of the command buffer
} vulkan_command_buffer;

/**
 * Internally track the swapchain state.
 */
typedef
struct vulkan_swapchain {
  /**
   * The type of format we're using for the swapchain image.
   */
  VkSurfaceFormatKHR image_format;
  /**
   * The extent of the swapchain image.
   */
  u8 max_frames_in_flight;

  /**
   * A handle to the swapchain
   */
  VkSwapchainKHR handle;

  /**
   * The images in the swapchain
   */
  u32 image_count;

  /**
   * The images in the swapchain
   */
  VkImage *images;

  /**
   * The vulkan image views for the swapchain
   */
  VkImageView *image_views;
  /**
   * The depth attachment for the swapchain
   */
  vulkan_image depth_attachment;

} vulkan_swapchain;

/**
 * Internally track the global vulkan context state
 */
typedef struct vulkan_context {

  /**
   * The width of the framebuffer. The current renderable width of the window
   */
  u32 framebuffer_width;

  /**
   * The height of the framebuffer. The current renderable height of the window
   */
  u32 framebuffer_height;

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

  /**
   * We'll need to store the swapchain for the context
   */
  vulkan_swapchain swapchain;

  /**
   * The main renderpass for the swapchain
   */
  vulkan_render_pass main_render_pass;

  /**
   * The current image index
   */
  u32 image_index;

  /**
   * The current frame index
   */
  u32 current_frame;

  /**
   * Track the state of creation for the swapchain
   */
  b8 creating_swapchain;

  /**
   * Finds the memory index for the given memory type bits and properties
   * @param memory_type_bits the memory type bits
   * @param property_flags the properties of the memory type
   * @return the index of the memory
   */
  i32 (*find_memory_index)(u32 memory_type_bits, u32 property_flags);
} vulkan_context;


