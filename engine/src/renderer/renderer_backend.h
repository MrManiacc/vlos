//
// Created by jwraynor on 8/22/2023.
//

#pragma once

#include "renderer/renderer_types.inl"

/**
 * Initialize our renderer backend. This will be implemented per renderer implementation.
 * @param type the type of renderer backend to create
 * @param plat_state the platform state
 * @param out_renderer_backend the renderer backend to create
 * @return whether or not the backend was successfully initialized
 */
b8 render_backend_create(renderer_backend_type type, struct platform_state *plat_state,
                         renderer_backend *out_renderer_backend);

/**
 * Destroy our renderer backend. This will be implemented per renderer implementation.
 * @param backend the renderer backend to destroy
 */
void render_backend_destroy(renderer_backend *backend);