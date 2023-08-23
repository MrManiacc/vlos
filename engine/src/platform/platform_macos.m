
#include "platform/platform.h"
#include "core/logger.h"
#include <stdio.h>

#if VPLATFORM_MACOS

#include "core/input.h"
#import <Cocoa/Cocoa.h>
#include "defines.h"
#import "renderer/vulkan/vulkan_types.inl"
#include <mach/mach_time.h>
#include <unistd.h>

@interface VlosWindowDelegate : NSObject <NSWindowDelegate>
@end

@implementation VlosWindowDelegate
- (BOOL)windowShouldClose:(id)sender {
    // Handle window close event here
    return YES;
}
@end

typedef struct internal_state {
    NSWindow *window;
    VlosWindowDelegate *delegate;
} internal_state;


b8 platform_pump_messages(platform_state *platform) {
    // macOS specific message/event polling code goes here
    // Since we're using [app run] in platform_startup, this might not be needed.
    return true;
}

void *platform_allocate(u64 size, b8 aligned) {
    // macOS specific memory allocation code goes here
    return malloc(size);
}


#endif // VPLATFORM_MACOS


// Console writing functions


// Memory manipulation functions





b8 platform_startup(platform_state *platform, conststr str, i32 x, i32 y, i32 width, i32 height) {
    // Allocate the internal state
    platform->internal_state = malloc(sizeof(internal_state));
    internal_state *state = (internal_state *) platform->internal_state;

    // Initialize the NSApplication object
    [NSApplication sharedApplication];

    // Create the main application window
    NSUInteger styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskResizable | NSWindowStyleMaskClosable;
    state->window = [[NSWindow alloc] initWithContentRect:NSMakeRect(x, y, width, height)
                                                styleMask:styleMask
                                                  backing:NSBackingStoreBuffered
                                                    defer:NO];

    // Set the window's title
    [state->window setTitle:[NSString stringWithUTF8String:str]];

    // Display the window
    [state->window makeKeyAndOrderFront:nil];

    return true;
}

void platform_shutdown(platform_state *platform) {
    internal_state *state = (internal_state *) platform->internal_state;

    // Release the NSWindow object
    [state->window release];
    state->window = nil;

    // Free the internal state
    free(state);
    platform->internal_state = NULL;
}


void platform_free(void *ptr, b8 aligned) {
    free(ptr);
}

void platform_console_write(conststr str, u8 color) {
    // ANSI escape codes for console colors
    // FATAL, ERROR, WARN, INFO, DEBUG, TRACE
    static const char *colors[7] = {"[1;31m", "[1;35m", "[1;33m", "[1;36m", "[1;34m", "[1;32m", "[0m"};

    if (color < 7) {
        printf("%s%s[0m", colors[color], str);
    } else {
        printf("%s", str);
    }
}

void platform_console_write_error(conststr str, u8 color) {
    // ANSI escape codes for console colors
    // FATAL, ERROR, WARN, INFO, DEBUG, TRACE
    static const char *colors[7] = {"[1;31m", "[1;35m", "[1;33m", "[1;36m", "[1;34m", "[1;32m", "[0m"};

    if (color < 7) {
        fprintf(stderr, "%s%s[0m", colors[color], str);
    } else {
        fprintf(stderr, "%s", str);
    }
}

f64 platform_get_time() {
    static mach_timebase_info_data_t timebase;
    if (timebase.denom == 0) {
        (void) mach_timebase_info(&timebase);
    }
    return (f64) mach_absolute_time() * (f64) timebase.numer / (f64) timebase.denom / 1e9; // Convert to seconds
}

void platform_sleep(u64 ms) {
    usleep(ms * 1000); // Convert milliseconds to microseconds
}

const char **platform_required_extensions(u32 *count) {
    static const char *extensions[] = {};
    *count = sizeof(extensions) / sizeof(extensions[0]);
    return extensions;
}

b8 platform_create_vulkan_surface(platform_state *plat_state, vulkan_context *context) {
    // Simply cold-cast to the known type.
    internal_state *state = (internal_state *) plat_state->internal_state;

//    VkMacOSSurfaceCreateInfoMVK create_info = {};
//    create_info.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
//    create_info.pNext = NULL;
//    create_info.flags = 0;
//    create_info.pView = state->window;
//
//    if (vkCreateMacOSSurfaceMVK(context->instance, &create_info, NULL, &context->surface) != VK_SUCCESS) {
//        return false;
//    }

    return true;
}

void *platform_copy_memory(void *dest, const void *src, u64 size) {
    memcpy(dest, src, size);
}

void* platform_set_memory(void *dest, int value, u64 size) {
    memset(dest, value, size);
}

void *platform_zero_memory(void *dest, u64 size) {
    memset(dest, 0, size);
}

