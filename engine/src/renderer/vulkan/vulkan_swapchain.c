/**
 * Created by jraynor on 8/22/2023.
 */
#include "core/logger.h"
#include "core/mem.h"
#include "vulkan_swapchain.h"
#include "vulkan_device.h"
#include "vulkan_image.h"

void create(vulkan_context *context, u32 width, u32 height, vulkan_swapchain *swapchain);
void destroy(vulkan_context *context, vulkan_swapchain *swapchain);

void vulkan_swapchain_create(vulkan_context *context, u32 width, u32 height, vulkan_swapchain *out_swapchain) {
    create(context, width, height, out_swapchain);
}

void vulkan_swapchain_recreate(vulkan_context *context, u32 width, u32 height, vulkan_swapchain *swapchain) {
    destroy(context, swapchain);
    create(context, width, height, swapchain);
}

void vulkan_swapchain_destroy(vulkan_context *context, vulkan_swapchain *swapchain) {
    destroy(context, swapchain);
}

b8 vulkan_swapchain_acquire_next_image(
    vulkan_context *context,
    vulkan_swapchain *swapchain,
    u64 timeout,
    VkSemaphore image_available_semaphore,
    VkFence fence,
    u32 *out_image_index
) {
    VkResult result = vkAcquireNextImageKHR(
        context->device.logical_device,
        swapchain->handle,
        timeout,
        image_available_semaphore,
        fence,
        out_image_index
    );

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        vulkan_swapchain_recreate(context, context->framebuffer_width, context->framebuffer_height, swapchain);
        return false;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        vfatal("Failed to acquire next image");
        return false;
    }
    return true;
}

void vulkan_swapchain_present(
    vulkan_context *context,
    vulkan_swapchain *swapchain,
    VkQueue graphics_queue,
    VkQueue present_queue,
    VkSemaphore render_finished_semaphore,
    u32 present_image_index
) {
    VkPresentInfoKHR present_info = {VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &render_finished_semaphore;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &swapchain->handle;
    present_info.pImageIndices = &present_image_index;
    present_info.pResults = 0;

    VkResult result = vkQueuePresentKHR(present_queue, &present_info);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        vulkan_swapchain_recreate(context, context->framebuffer_width, context->framebuffer_height, swapchain);
    } else if (result != VK_SUCCESS) {
        vfatal("Failed to present swapchain image!");
    }
}

void create(vulkan_context *context, u32 width, u32 height, vulkan_swapchain *swapchain) {
    VkExtent2D extent = {width, height};
    swapchain->max_frames_in_flight = 2;

    b8 found = false;
    for (u32 i = 0; i < context->device.swapchain_support.format_count; ++i) {
        VkSurfaceFormatKHR format = context->device.swapchain_support.formats[i];
        if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            swapchain->image_format = format;
            found = true;
            break;
        }
    }
    if (!found) {
        swapchain->image_format = context->device.swapchain_support.formats[0];
        vwarn("Using suboptimal swapchain format!");
    }
    VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
    for (u32 i = 0; i < context->device.swapchain_support.present_mode_count; ++i) {
        VkPresentModeKHR mode = context->device.swapchain_support.present_modes[i];
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            present_mode = mode;
            vdebug("Using mailbox present mode :)")
            break;
        } else if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
            present_mode = mode;
        }
    }
    vulkan_device_query_swapchain_support(
        context->device.physical_device,
        context->surface,
        &context->device.swapchain_support
    );

    if (context->device.swapchain_support.capabilities.currentExtent.width != UINT32_MAX) {
        extent = context->device.swapchain_support.capabilities.currentExtent;
    }

    VkExtent2D min = context->device.swapchain_support.capabilities.minImageExtent;
    VkExtent2D max = context->device.swapchain_support.capabilities.maxImageExtent;
    extent.width = vclamp(extent.width, min.width, max.width);
    extent.height = vclamp(extent.height, min.height, max.height);

    u32 image_count = context->device.swapchain_support.capabilities.minImageCount + 1;
    if (context->device.swapchain_support.capabilities.maxImageCount > 0 &&
        image_count > context->device.swapchain_support.capabilities.maxImageCount) {
        image_count = context->device.swapchain_support.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR create_info = {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
    create_info.surface = context->surface;
    create_info.minImageCount = image_count;
    create_info.imageFormat = swapchain->image_format.format;
    create_info.imageColorSpace = swapchain->image_format.colorSpace;
    create_info.imageExtent = extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (context->device.graphics_queue_index != context->device.present_queue_index) {
        u32 queue_family_indices[] = {
            context->device.graphics_queue_index,
            context->device.present_queue_index
        };
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices;
    } else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount = 0;
        create_info.pQueueFamilyIndices = 0;
    }

    create_info.preTransform = context->device.swapchain_support.capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = present_mode;
    create_info.clipped = VK_TRUE;
    create_info.oldSwapchain = 0;
    VK_CHECK(vkCreateSwapchainKHR(context->device.logical_device,
                                  &create_info,
                                  context->allocator,
                                  &swapchain->handle));

    context->current_frame = 0;

    // Create our images
    swapchain->image_count = 0;
    VK_CHECK(vkGetSwapchainImagesKHR(
        context->device.logical_device,
        swapchain->handle,
        &swapchain->image_count,
        0));
    if (!swapchain->images) {
        swapchain->images = (VkImage *) mem_alloc(sizeof(VkImage) * swapchain->image_count, MEM_TAG_RENDERER);
    }
    if (!swapchain->image_views) {
        swapchain->image_views =
            (VkImageView *) mem_alloc(sizeof(VkImageView) * swapchain->image_count, MEM_TAG_RENDERER);
    }
    VK_CHECK(vkGetSwapchainImagesKHR(
        context->device.logical_device,
        swapchain->handle,
        &swapchain->image_count,
        swapchain->images));

    // Create our views
    for (u32 i = 0; i < swapchain->image_count; ++i) {
        VkImageViewCreateInfo view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
        view_info.image = swapchain->images[i];
        view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        view_info.format = swapchain->image_format.format;
        view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        view_info.subresourceRange.baseMipLevel = 0;
        view_info.subresourceRange.levelCount = 1;
        view_info.subresourceRange.baseArrayLayer = 0;
        view_info.subresourceRange.layerCount = 1;
        VK_CHECK(vkCreateImageView(context->device.logical_device,
                                   &view_info,
                                   context->allocator,
                                   &swapchain->image_views[i]));
    }

    //Depth image
    if (!vulkan_device_detect_depth_format(&context->device)) {
        context->device.depth_format = VK_FORMAT_UNDEFINED;
        vfatal("Failed to detect depth format!");
    }

    vulkan_image_create(
        context,
        VK_IMAGE_TYPE_2D,
        extent.width,
        extent.height,
        context->device.depth_format,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        true,
        VK_IMAGE_ASPECT_DEPTH_BIT,
        &swapchain->depth_attachment
    );
    vinfo("Swapchain created successfully.");
}

void destroy(vulkan_context *context, vulkan_swapchain *swapchain) {
    vulkan_image_destroy(context, &swapchain->depth_attachment);

    for (u32 i = 0; i < swapchain->image_count; ++i) {
        vkDestroyImageView(context->device.logical_device, swapchain->image_views[i], context->allocator);
    }

    vkDestroySwapchainKHR(context->device.logical_device, swapchain->handle, context->allocator);

    mem_free(swapchain->images, sizeof(VkImage) * swapchain->image_count, MEM_TAG_RENDERER);
    mem_free(swapchain->image_views, sizeof(VkImageView) * swapchain->image_count, MEM_TAG_RENDERER);

}
