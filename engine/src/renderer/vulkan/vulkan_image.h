/**
 * Created by jraynor on 8/22/2023.
 */
#pragma once

#include "vulkan_types.inl"
/**
 * Creates an image with the given parameters.
 * @param context
 * @param image_type
 * @param width
 * @param height
 * @param format
 * @param tiling
 * @param usage
 * @param properties
 * @param create_view
 * @param view_aspect_flags
 * @param out_image
 */
void vulkan_image_create(
    vulkan_context *context,
    VkImageType image_type,
    u32 width,
    u32 height,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkMemoryPropertyFlags properties,
    b32 create_view,
    VkImageAspectFlags view_aspect_flags,
    vulkan_image *out_image
);

void vulkan_image_view_create(
    vulkan_context *context,
    VkFormat format,
    VkImageAspectFlags aspect_flags,
    vulkan_image *image
);

void vulkan_image_destroy(vulkan_context *context, vulkan_image *image);