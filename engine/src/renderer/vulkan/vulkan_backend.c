//
// Created by jwraynor on 8/22/2023.
//

#include "vulkan_backend.h"
#include "vulkan_types.inl"
#include "core/logger.h"
#include "core/vstring.h"
#include "containers/darray.h"
#include "renderer/vulkan/vulkan_platform.h"

//internally store a static vulkan context
static vulkan_context context;
//Forward declare our debug callback
VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
        VkDebugUtilsMessageTypeFlagsEXT message_type,
        const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
        void *user_data
);


b8 vulkan_backend_initialize(renderer_backend *out_renderer_backend, conststr application_name,
                             struct platform_state *plat_state) {
    context.allocator = nil;

    VkApplicationInfo app_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    app_info.apiVersion = VK_API_VERSION_1_3;
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pApplicationName = application_name;
    app_info.pEngineName = "Vlos Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    create_info.pApplicationInfo = &app_info;
    const char **required_extensions = darray_create(const char*);
    darray_push(required_extensions, &VK_KHR_SURFACE_EXTENSION_NAME);
    platform_required_extensions(&required_extensions);
#if defined(DEBUG_BUILD)
    darray_push(required_extensions, &VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    for (u32 i = 0; i < darray_length(required_extensions); i++) {
        vinfo("Required extension: %s", required_extensions[i])
    }
#endif

    create_info.enabledExtensionCount = darray_length(required_extensions);
    create_info.ppEnabledExtensionNames = required_extensions;

    //validation layers
    const char **required_validation_layers = nil;
    u32 required_validation_layer_count = nil;


#if defined(DEBUG_BUILD)
    vinfo("Validation layer enabled, enumerating available layers")
    // The list of available validation layers required
    required_validation_layers = darray_create(const char*);
    darray_push(required_validation_layers, &"VK_LAYER_KHRONOS_validation");
    required_validation_layer_count = darray_length(required_validation_layers);

    //Obtain a list of available validation layer
    u32 avaiable_layer_counter = nil;
    VK_CHECK(vkEnumerateInstanceLayerProperties(&avaiable_layer_counter, nil));
    VkLayerProperties *avaiable_layers = darray_reserve(VkLayerProperties, avaiable_layer_counter);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&avaiable_layer_counter, avaiable_layers));

    //Verify al the required layers are present
    for (u32 i = 0; i < required_validation_layer_count; ++i) {
        vinfo("Searching for required layer: %s", required_validation_layers[i])
        b8 found = false;
        for (u32 j = 0; j < avaiable_layer_counter; ++j) {
            if (strings_equal(required_validation_layers[i], avaiable_layers[j].layerName)) {
                vinfo("Found required layer: %s", required_validation_layers[i])
                found = true;
                break;
            }
        }

        if (!found) {
            vfatal("Failed to find required layer: %s", required_validation_layers[i])
            return false;
        }
    }
    vdebug("All required validation layers are present.")
#endif
    create_info.ppEnabledLayerNames = required_validation_layers;
    create_info.enabledLayerCount = required_validation_layer_count;
    VK_CHECK(vkCreateInstance(&create_info, context.allocator, &context.instance))
    vinfo("Successfully created vulkan instance")

#if defined(DEBUG_BUILD)
    vdebug("Creating vulkan debugger");
    u32 log_severity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
                       | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
    debug_create_info.messageSeverity = log_severity;
    debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                                    | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                                    | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_create_info.pfnUserCallback = vk_debug_callback;
    debug_create_info.pUserData = nil;

    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
            context.instance, "vkCreateDebugUtilsMessengerEXT");
    VASSERT_MSG(func, "Failed to load vkCreateDebugUtilsMessengerEXT");
    VK_CHECK(func(context.instance, &debug_create_info, context.allocator, &context.debug_messenger))
    vdebug("Successfully created vulkan debugger");
#endif
    return true;
}

void vulkan_backend_shutdown(renderer_backend *backend) {
    vdebug("Destroying vulkan debugger instance")
    if (context.debug_messenger) {
        PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
                context.instance, "vkDestroyDebugUtilsMessengerEXT");
        func(context.instance, context.debug_messenger, context.allocator);
    }
    vdebug("Destroying vulkan instance")
    vkDestroyInstance(context.instance, context.allocator);
}

void vulkan_backend_resized(renderer_backend *backend, u16 width, u16 height) {

}

b8 vulkan_backend_begin_frame(renderer_backend *backend, f32 delta_time) {
    return true;
}

b8 vulkan_backend_end_frame(renderer_backend *backend, f32 delta_time) {
    return true;
}


VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
        VkDebugUtilsMessageTypeFlagsEXT message_type,
        const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
        void *user_data
) {
    switch (message_severity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            vdebug("Vulkan validation layer: %s", callback_data->pMessage)
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            vinfo("Vulkan validation layer: %s", callback_data->pMessage)
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            vwarn("Vulkan validation layer: %s", callback_data->pMessage)
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            verror("Vulkan validation layer: %s", callback_data->pMessage)
            break;
        default:
            verror("Vulkan validation layer: %s", callback_data->pMessage)
            break;
    }
    return VK_FALSE;
}
