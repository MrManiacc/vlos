/**
 * Created by jraynor on 8/22/2023.
 */
#pragma once

#include "defines.h"
#include "vulkan_types.inl"

/**
 * This is used to internally create our swapchain. This should be called after the device is created.
 * @param context the context to create the swapchain for
 * @param width the width of the swapchain
 * @param height the height of the swapchain
 * @param out_swapchain the swapchain to create
 */
void vulkan_swapchain_create(vulkan_context *context, u32 width, u32 height, vulkan_swapchain *out_swapchain);

/**
 * This is used for when the swapchain needs to be recreated. This should be called after the device is created.
 * This should be called when the window is resized.
 * @param context the context to create the swapchain for
 * @param width the width of the swapchain
 * @param height the height of the swapchain
 * @param swapchain the swapchain to recreate
 */
void vulkan_swapchain_recreate(vulkan_context *context, u32 width, u32 height, vulkan_swapchain *swapchain);
/**
 * This is used to internally destroy our swapchain. This should be called before the device is destroyed.
 * @param context the context to destroy the swapchain for
 * @param swapchain the swapchain to destroy
 */
void vulkan_swapchain_destroy(vulkan_context *context, vulkan_swapchain *swapchain);

/**
 * This is used to internally acquire the next image from the swapchain. This should be called before the device is destroyed.
 * @param context the context to acquire the image for
 * @param swapchain the swapchain to acquire the image from
 * @param timeout the timeout to use
 * @param image_available_semaphore the semaphore to use
 * @param fence the fence to use
 * @param out_image_index the image index to acquire
 * @return
 */
b8 vulkan_swapchain_acquire_next_image(
    vulkan_context *context,
    vulkan_swapchain *swapchain,
    u64 timeout,
    VkSemaphore image_available_semaphore,
    VkFence fence,
    u32 *out_image_index
);

/**
 * This is used to internally present the image to the swapchain. This should be called before the device is destroyed.
 * @param context the context to present the image for
 * @param swapchain the swapchain to present the image to
 * @param graphics_queue the graphics queue to use
 * @param present_queue the present queue to use
 * @param render_finished_semaphore the semaphore to use
 * @param present_image_index the image index to present
 * @return
 */
void vulkan_swapchain_present(
    vulkan_context *context,
    vulkan_swapchain *swapchain,
    VkQueue graphics_queue,
    VkQueue present_queue,
    VkSemaphore render_finished_semaphore,
    u32 present_image_index
);

