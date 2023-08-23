//
// Created by jwraynor on 8/22/2023.
//

#pragma once

#include "defines.h"
#include "renderer/vulkan/vulkan_types.inl"

/**
 * This should create our physical vulkan device
 * @param context the context to create the device for
 * @return whether or not the device was successfully created
 */
b8 vulkan_device_create(vulkan_context *context);

/**
 * Destroy our physical vulkan device. This should be called before the context is destroyed.
 * @param context the context to destroy the device for
 */
void vulkan_device_destroy(vulkan_context *context);

/**
 * Queries the swapchain support for the given physical device and surface.
 * @param physical_device  The physical device to query
 * @param surface The surface to query
 * @param out_support_info  The swapchain support info to fill out
 */
void vulkan_device_query_swapchain_support(VkPhysicalDevice physical_device,
                                           VkSurfaceKHR surface,
                                           vulkan_swapchain_support_info *out_support_info);
/**
 * Queries the physical device for the best depth format.
 * @param device the device to query
 * @return whether we have the depth format
 */
b8 vulkan_device_detect_depth_format(vulkan_device *device);