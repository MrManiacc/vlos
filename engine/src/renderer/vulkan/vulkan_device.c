//
// Created by jwraynor on 8/22/2023.
//
#include "vulkan_device.h"
#include "core/logger.h"
#include "containers/darray.h"

typedef struct vulkan_physical_device_requirements {
    b8 graphics;
    b8 present;
    b8 compute;
    b8 transfer;
    const char **device_extension_names;
    b8 sampler_anisotropy;
    b8 discrete_gpu;
} vulkan_physical_device_requirements;


typedef struct vulkan_physical_device_queue_family_info {
    u32 graphics_family_index;
    u32 present_family_index;
    u32 compute_family_index;
    u32 transfer_family_index;
} vulkan_physical_device_queue_family_info;


b8 select_physical_device(vulkan_context *context);

b8 physical_device_meets_requirements(
        VkPhysicalDevice device,
        VkSurfaceKHR surface,
        const VkPhysicalDeviceProperties *properties,
        const VkPhysicalDeviceFeatures *features,
        const struct vulkan_physical_device_requirements *requirements,
        vulkan_physical_device_queue_family_info *out_queue_info,
        vulkan_swapchain_support_info *out_swapchain_support
);


b8 vulkan_device_create(vulkan_context *context) {

    return true;
}

void vulkan_device_destroy(vulkan_context *context) {

}

/**
 * Selects the appropriate physical device for the vulkan context
 * @param context
 * @return
 */
b8 select_physical_device(vulkan_context *context) {
    u32 device_count = 0;
    VK_CHECK(vkEnumeratePhysicalDevices(context->instance, &device_count, nil));
    if (device_count == 0) {
        vfatal("No physical devices found");
        return false;
    }
    VkPhysicalDevice physical_devices[device_count];
    VK_CHECK(vkEnumeratePhysicalDevices(context->instance, &device_count, physical_devices));
    for (u32 i = 0; i < device_count; ++i) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physical_devices[i], &properties);
        vinfo("Found physical device: %s", properties.deviceName);

        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(physical_devices[i], &features);

        VkPhysicalDeviceMemoryProperties memory;
        vkGetPhysicalDeviceMemoryProperties(physical_devices[i], &memory);

        vulkan_physical_device_requirements requirements = {};
        requirements.graphics = true;
        requirements.present = true;
        //requirements.compute = true; //TODO: Only enable if we need to use compute shaders
        requirements.transfer = true;
        //TODO: Add more requirements here
        requirements.sampler_anisotropy = true;
        requirements.discrete_gpu = true;
        requirements.device_extension_names = darray_create(const char *);
        darray_push(requirements.device_extension_names, &VK_KHR_SWAPCHAIN_EXTENSION_NAME);


        vulkan_physical_device_queue_family_info queue_family_info = {};


    }
    return true;
}


b8 physical_device_meets_requirements(
        VkPhysicalDevice device,
        VkSurfaceKHR surface,
        const VkPhysicalDeviceProperties *properties,
        const VkPhysicalDeviceFeatures *features,
        const struct vulkan_physical_device_requirements *requirements,
        vulkan_physical_device_queue_family_info *out_queue_info,
        vulkan_swapchain_support_info *out_swapchain_support
) {
    out_queue_info->compute_family_index = -1;
    out_queue_info->graphics_family_index = -1;
    out_queue_info->present_family_index = -1;
    out_queue_info->transfer_family_index = -1;

    if (requirements->discrete_gpu) {
        if (properties->deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            vinfo("Device [%s] is not a discrete gpu, skipping", properties->deviceName);
            return false;
        }
    }

    u32 queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nil);
    VkQueueFamilyProperties queue_families[queue_family_count];
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families);


    vinfo("Graphics | Present | Compute | Transfer | Name")
    u8 min_transfer_score = 255;
    for (u32 i = 0; i < queue_family_count; ++i) {
        u8 current_transfer_score = nil;
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            out_queue_info->graphics_family_index = i;
            ++current_transfer_score;
        }

        if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            out_queue_info->compute_family_index = i;
            ++current_transfer_score;
        }

        if (queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
            //Take the current lowest index, try to find dedicated transfer queue
            if (current_transfer_score <= min_transfer_score) {
                min_transfer_score = current_transfer_score;
                out_queue_info->transfer_family_index = i;
            }
        }

        VkBool32 supports_present = VK_FALSE;
        VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &supports_present));
        if (supports_present) {
            out_queue_info->present_family_index = i;
        }
    }

    vinfo("%8s | %7s | %7s | %8s | %s",
          out_queue_info->graphics_family_index != -1 ? "Yes" : "No",
          out_queue_info->present_family_index != -1 ? "Yes" : "No",
          out_queue_info->compute_family_index != -1 ? "Yes" : "No",
          out_queue_info->transfer_family_index != -1 ? "Yes" : "No",
          properties->deviceName);

    //TODO: start the checking, 29:41 https://youtu.be/6Kj3O2Ov1RU?list=PLv8Ddw9K0JPg1BEO-RS-0MYs423cvLVtj&t=1781

    return true;
}