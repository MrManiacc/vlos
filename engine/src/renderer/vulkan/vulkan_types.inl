#include "defines.h"

#include <vulkan/vulkan.h>
#include "core/asserts.h"

#define VK_CHECK(expr)               \
    {                                \
        VASSERT(expr == VK_SUCCESS); \
    }


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
     * If we're on a debug build, we should use our debug messenger
     */
#if defined(DEBUG_BUILD)
    VkDebugUtilsMessengerEXT debug_messenger;
#endif

} vulkan_context;


