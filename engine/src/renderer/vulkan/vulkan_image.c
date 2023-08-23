/**
 * Created by jraynor on 8/22/2023.
 */
#include "vulkan_image.h"
#include "vulkan_device.h"

#include "core/mem.h"
#include "core/logger.h"

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
) {
    VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
    image_create_info.imageType = VK_IMAGE_TYPE_2D;
    image_create_info.extent.width = width;
    image_create_info.extent.height = height;
    image_create_info.extent.depth = 1;
    image_create_info.mipLevels = 4; //TODO: make this configurable
    image_create_info.arrayLayers = 1; //TODO: make this configurable
    image_create_info.format = format;
    image_create_info.tiling = tiling;
    image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_create_info.usage = usage;
    image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VK_CHECK(vkCreateImage(context->device.logical_device, &image_create_info, context->allocator, &out_image->handle));

    VkMemoryRequirements memory_requirements;
    vkGetImageMemoryRequirements(context->device.logical_device, out_image->handle, &memory_requirements);

    i32 memory_type = context->find_memory_index(memory_requirements.memoryTypeBits, properties);
    if (memory_type == -1) {
        verror("Failed to find suitable memory type. Image creation failed.");
    }

    VkMemoryAllocateInfo memory_allocate_info = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    memory_allocate_info.allocationSize = memory_requirements.size;
    memory_allocate_info.memoryTypeIndex = memory_type;
    //Allocate the memory for the image
    VK_CHECK(vkAllocateMemory(context->device.logical_device,
                              &memory_allocate_info,
                              context->allocator,
                              &out_image->memory));
    //Bind the memory for the image
    VK_CHECK(vkBindImageMemory(context->device.logical_device, out_image->handle, out_image->memory, 0));

    if (create_view) {
        out_image->view = 0;
        vulkan_image_view_create(context, format, view_aspect_flags, out_image);
    }
}
void vulkan_image_view_create(
    vulkan_context *context,
    VkFormat format,
    VkImageAspectFlags aspect_flags,
    vulkan_image *image
) {
    VkImageViewCreateInfo view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    view_info.image = image->handle;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = format;
    view_info.subresourceRange.aspectMask = aspect_flags;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;
    VK_CHECK(vkCreateImageView(context->device.logical_device,
                               &view_info,
                               context->allocator,
                               &image->view));
}

void vulkan_image_destroy(vulkan_context *context, vulkan_image *image) {
    if (image->view) {
        vkDestroyImageView(context->device.logical_device, image->view, context->allocator);
        image->view = 0;
    }
    if (image->memory) {
        vkFreeMemory(context->device.logical_device, image->memory, context->allocator);
        image->memory = 0;
    }
    if (image->handle) {
        vkDestroyImage(context->device.logical_device, image->handle, context->allocator);
        image->handle = 0;
    }
}