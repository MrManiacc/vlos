#pragma once

#include "defines.h"

/**
 * Define the different types of backends our renderer supports
 */
typedef enum renderer_backend_type {
    RENDERER_BACKEND_TYPE_VULKAN,
    RENDERER_BACKEND_TYPE_OPENGL,
    RENDERER_BACKEND_TYPE_DIRECTX,
    RENDERER_BACKEND_TYPE_METAL,
} renderer_backend_type;

/**
 * Define a template for a renderer backend
 */
typedef struct renderer_backend {
    /**
     * Keep track of the type of platform we are using
     */
    struct platform_state *plat_state;

    /**
     * Tracks the total number of frames that have rendered
     */
    u64 frame_number;

    /**
     * Used to initialize the renderer backend
     * @param backend the actual backend
     * @param application_name the name of the application
     * @param plat_state the platform state
     * @return whether or not the backend was successfully initialized
     */
    b8 (*initialize)(struct renderer_backend *backend,
                     conststr application_name,
                     struct platform_state *plat_state);

    /**
     * Used to shutdown the renderer backend. This should be called before the application is shutdown.
     * @param backend the actual backend
     */
    void (*shutdown)(struct renderer_backend *backend);

    /**
     * Called when the application surface is resized. Mainly will be used on computers, not mobile devices.
     * @param backend the actual backend
     * @param width the new width of the surface
     * @param height the new height of the surface
     */
    void (*resized)(struct renderer_backend *backend, u16 width, u16 height);

    /**
     * Called when we want to begin a sequence of rendering commands
     * @param backend the actual backend
     * @param delta_time the time since the last frame
     * @return whether or not the frame was successfully started
     */
    b8 (*begin_frame)(struct renderer_backend *backend, f32 delta_time);


    /**
     * Called when we want to end a sequence of rendering commands and blit the frame to the screen
     * @param backend the actual backend
     * @param delta_time the time since the last frame
     * @return whether or not the frame was successfully started
     */
    b8 (*end_frame)(struct renderer_backend *backend, f32 delta_time);


} renderer_backend;

/**
 * The data that is passed to the render context, should contain all the information needed to render a frame
 */
typedef struct render_packet{
    f32 delta_time;
} render_packet;
