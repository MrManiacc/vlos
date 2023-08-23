#include "types.h"
#include "application.h"
#include "platform/platform.h"
#include "core/logger.h"
#include "core/mem.h"
#include "core/event.h"
#include "core/input.h"
#include "core/clock.h"
#include "renderer/renderer_frontend.h"

/**
 * Internally initializes the platform state and returns it. This is per platform and should be called
 * by the platform specific code. This will also initialize the logger any all other core systems.
 */
typedef struct application_state {
    app_host *host; //< The host application
    b8 running; //< Whether the application is running
    b8 suspended; //< Whether the application is suspended
    platform_state platform; //< The platform state
    i16 width; //< The width of the surface
    i16 height; //< The height of the surface
    clock clock; //< The clock for the application. internally tracks the elapsed time
    f64 last_time; //< The last time the application was updated
} application_state;

static b8 initialized = false;
static application_state state;


//Application event handlers
b8 application_on_event(u16 code, ptr sender, ptr listener_inst, event_context context);

b8 application_on_key(u16 code, ptr sender, ptr listener_inst, event_context context);

b8 application_on_resized(u16 code, void *sender, void *listener_inst, event_context context);

/**
 * Internally initializes the platform state and returns it. This is per platform and should be called
 * by the platform specific code. This will also initialize the logger any all other core systems.
 * @param config The configuration for the application
 * @return Whether or not the platform was successfully initialized
 */
b8 application_create(app_host *host) {
    if (initialized) {
        vfatal("Application already initialized")
        return false;
    }
    if (!host) {
        vfatal("Application host is null")
        return false;
    }
    state.host = host;
    //Initialize our subsystems
    if (!logger_initialize()) {
        vfatal("Failed to initialize the logger")
        return false;
    }
    state.running = true;
    state.suspended = false;

    if (!event_initialize()) {
        vfatal("Event system failed initialization. Application cannot continue.");
        return false;
    }
    event_register(SYSTEM_EVENT_CODE_QUIT, 0, application_on_event);
    event_register(SYSTEM_EVENT_CODE_KEY_PRESS, 0, application_on_key);
    event_register(SYSTEM_EVENT_CODE_KEY_RELEASE, 0, application_on_key);
    event_register(SYSTEM_EVENT_CODE_WINDOW_RESIZE, 0, application_on_resized);
    if (!platform_startup(&state.platform,
                          host->config.title,
                          host->config.x,
                          host->config.y,
                          host->config.width,
                          host->config.height)) {
        vfatal("Failed to initialize the platform")
        return false;
    }

    if (!renderer_initialize(host->config.title, &state.platform)) {
        vfatal("Failed to initialize the renderer")
        return false;
    }


    if (!input_initialize()) {
        vfatal("Failed to initialize the input system")
        return false;
    }

    if (!state.host->create(state.host)) {
        vfatal("Failed to initialize the application")
        return false;
    }


    state.host->on_resize(state.host, state.width, state.height);

    vinfo("Initialized the platform")
    initialized = true;
    return true;
}

/**
 * Internally runs the application. This is per platform and should be called by the platform specific code.
 * @return Whether or not the platform was successfully initialized
 */
b8 application_run() {
    if (!initialized) {
        verror("Application not initialized")
        return false;
    }
    clock_start(&state.clock);
    clock_update(&state.clock);
    state.last_time = state.clock.elapsed_time;
    f64 running_time = 0;
    u8 frame_count = 0;
    f64 frame_target = 1.0 / 60.0;
    vinfo(mem_usage_str())
    while (state.running) {
        if (!platform_pump_messages(&state.platform)) {
            vfatal("Failed to pump platform messages")
            state.running = false;
            break;
        }
        if (!state.suspended) {
            clock_update(&state.clock);
            f64 current_time = state.clock.elapsed_time;
            f64 delta_time = current_time - state.last_time;
            f64 frame_start_time = platform_get_time();

            // Update and render the application
            if (!state.host->update(state.host, (f32) delta_time) ||
                !state.host->render(state.host, (f32) delta_time)) {
                vfatal("Failed to update/render the application")
                state.running = false;
                break;
            }

            render_packet packet;
            packet.delta_time = (f32) delta_time;
            renderer_draw_frame(&packet);

            f64 frame_end_time = platform_get_time();
            f64 frame_time = frame_end_time - frame_start_time;
            running_time += frame_time;
            f64 remaining_time = frame_target - frame_time;


            if (remaining_time > 0) {
                u64 remaining_ms = (remaining_time * 1000);
                b8 limit_frames = false;
                //TODO: This is a hack to limit the frames to 60. This should be done in a better way
                if (remaining_ms > 0 && limit_frames) {
                    platform_sleep(remaining_ms - 1);
                }
                frame_count++;
            }

            // Update our input state. Input state handling should always be done after
            // Any input has been recorded.
            input_update(delta_time);

            state.last_time = current_time;
        }
    }
    vwarn("Shutting down the application")
    mem_free(state.host->state, 4, MEM_TAG_APPLICATION);
    state.running = false;
    event_unregister(SYSTEM_EVENT_CODE_QUIT, application_on_event);
    event_unregister(SYSTEM_EVENT_CODE_KEY_PRESS, application_on_key);
    event_unregister(SYSTEM_EVENT_CODE_KEY_RELEASE, application_on_key);
    event_unregister(SYSTEM_EVENT_CODE_WINDOW_RESIZE, application_on_resized);
    event_shutdown(); //shutdown the event subsystem
    input_shutdown(); //shutdown the input subsystem
    renderer_shutdown(); //shutdown the renderer
    platform_shutdown(&state.platform); //shutdown the platform
    // Shutdown logger last just in case we need to log anything during shutdown
    logger_shutdown();
    return true;
}

void application_get_framebuffer_size(u32 *width, u32 *height) {
    *width = state.width;
    *height = state.height;
}

b8 application_on_event(u16 code, ptr sender, ptr listener_inst, event_context context) {
    if (code == SYSTEM_EVENT_CODE_QUIT) {
        state.running = false;
        vinfo("Received quit event. Shutting down the application")
        return true;
    }
    return false;
}

b8 application_on_key(u16 code, ptr sender, ptr listener_inst, event_context context) {
    if (code == SYSTEM_EVENT_CODE_KEY_PRESS) {
        keys key = (keys) context.u16[0];
        if (key == KEY_ESCAPE) {
            event_context data = {};
            event_trigger(SYSTEM_EVENT_CODE_QUIT, 0, data);
            return true;
        } else {
            vinfo("Key pressed: %c", key)
        }
    } else if (code == SYSTEM_EVENT_CODE_KEY_RELEASE) {
        keys key = (keys) context.u16[0];
        vinfo("Key released: %c", key)
    }
    return false;
}

b8 application_on_resized(u16 code, void *sender, void *listener_inst, event_context context) {
    if (code == SYSTEM_EVENT_CODE_WINDOW_RESIZE) {
        u16 width = context.u16[0];
        u16 height = context.u16[1];

        // Check if different. If so, trigger a resize event.
        if (width != state.width || height != state.height) {
            state.width = width;
            state.height = height;

            vdebug("Window resize: %i, %i", width, height);

            // Handle minimization
            if (width == 0 || height == 0) {
                vinfo("Window minimized, suspending application.");
                state.suspended = true;
                return true;
            } else {
                if (state.suspended) {
                    vinfo("Window restored, resuming application.");
                    state.suspended = false;
                }
                state.host->on_resize(state.host, width, height);
                renderer_resized(width, height);
            }
        }
    }

    // Event purposely not handled to allow other listeners to get this.
    return false;
}