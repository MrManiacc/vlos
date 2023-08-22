//
// Created by jwraynor on 8/22/2023.
//

#pragma once

#include "renderer/renderer_types.inl"

struct static_mesh_data;

struct platform_state;

/**
 * Initialize our renderer backend.
 * @param application_name  the name of the application
 * @param plat_state  the platform state
 * @return whether or not the backend was successfully initialized
 */
b8 renderer_initialize(conststr application_name, struct platform_state *plat_state);

/**
 * Shutdown the renderer backend. This should be called before the application is shutdown.
 */
void renderer_shutdown();

/**
 * Called when an application surface is resized. Mainly will be used on computers, not mobile devices.
 * @param width the new width of the surface
 * @param height the new height of the surface
 */
void renderer_resized(u16 width, u16 height);

/**
 * Called once per frame to kick off all rendering commands. This should be called before any other rendering
 * commands are called.
 * @param packet a packet full of information needed to render the frame
 * @return whether or not the frame was successfully started
 */
b8 renderer_draw_frame(render_packet* packet);