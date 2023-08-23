//
// Created by jwraynor on 8/22/2023.
//

#include "renderer_backend.h"

#include "vulkan/vulkan_backend.h"

b8 render_backend_create(renderer_backend_type type, struct platform_state *plat_state,
                         renderer_backend *out_renderer_backend) {
    out_renderer_backend->plat_state = plat_state;
    if (type == RENDERER_BACKEND_TYPE_VULKAN) {
        out_renderer_backend->initialize = vulkan_renderer_backend_initialize;
        out_renderer_backend->shutdown = vulkan_renderer_backend_shutdown;
        out_renderer_backend->resized = vulkan_renderer_backend_on_resized;
        out_renderer_backend->begin_frame = vulkan_renderer_backend_begin_frame;
        out_renderer_backend->end_frame = vulkan_renderer_backend_end_frame;
        return true;
    }
    //Failed to create the renderer or invalid type was given
    return false;

}

void render_backend_destroy(renderer_backend *backend) {
    backend->initialize = nil;
    backend->shutdown = nil;
    backend->resized = nil;
    backend->begin_frame = nil;
    backend->end_frame = nil;
}