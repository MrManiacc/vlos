//
// Created by jwraynor on 8/22/2023.
//

#pragma once

#include "renderer/renderer_backend.h"

/**
 * Initialize our vulkan renderer backend.
 * @param out_renderer_backend the renderer backend to create
 * @param application_name the name of the application
 * @param plat_state the platform state
 * @return whether or not the backend was successfully initialized
 */
b8 vulkan_backend_initialize(renderer_backend *out_renderer_backend, conststr application_name,
                             struct platform_state *plat_state);

/**
 * Shutdown our vulkan renderer backend.
 * @param backend  the renderer backend to shutdown
 */
void vulkan_backend_shutdown(renderer_backend *backend);
/**
 * Called when an application surface is resized. Mainly will be used on computers, not mobile devices.
 * @param backend the actual backend
 * @param width the new width of the surface
 * @param height the new height of the surface
 */
void vulkan_backend_resized(renderer_backend *backend, u16 width, u16 height);

/**
 * Called once per frame to kick off all rendering commands. This should be called before any other rendering
 * @param backend the actual backend
 * @param delta_time the time since the last frame
 * @return whether or not the frame was successfully started
 */
b8 vulkan_backend_begin_frame(renderer_backend *backend, f32 delta_time);

/**
 * Called once per frame to kick off all rendering commands. This should be called before any other rendering
 * @param backend the actual backend
 * @param delta_time the time since the last frame
 * @return whether or not the frame was successfully started
 */
b8 vulkan_backend_end_frame(renderer_backend *backend, f32 delta_time);

