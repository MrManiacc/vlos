#include "renderer_frontend.h"
#include "renderer_backend.h"
#include "core/logger.h"
#include "core/mem.h"

//Stores a pointer to our renderer backend
static renderer_backend *backend = nil;

bool renderer_begin_frame(f32 time);

b8 renderer_end_frame(f32 time);

b8 renderer_initialize(conststr application_name, struct platform_state *plat_state) {
    backend = mem_alloc(sizeof(renderer_backend), MEM_TAG_RENDERER);

    //TODO: make this configurable
    render_backend_create(RENDERER_BACKEND_TYPE_VULKAN, plat_state, backend);
    backend->frame_number = nil;

    if (!backend->initialize(backend, application_name, plat_state)) {
        verror("Failed to initialize renderer backend");
        return false;
    }

    return true;
}

void renderer_shutdown() {
    backend->shutdown(backend);
    mem_free(backend, sizeof(renderer_backend), MEM_TAG_RENDERER);
}

b8 renderer_end_frame(f32 time) {
    return backend->begin_frame(backend, time);
}

bool renderer_begin_frame(f32 time) {
    b8 result = backend->begin_frame(backend, time);
    backend->frame_number++;
    return result;
}

void renderer_resized(u16 width, u16 height) {
    if (backend) {
        backend->resized(backend, width, height);
    } else {
        vwarn("renderer backend does not exist to accept resize: %i %i", width, height);
    }
}


b8 renderer_draw_frame(render_packet *packet) {
    if (renderer_begin_frame(packet->delta_time)) {
        //End the frame, if it fails it's a fatal error and is not recoverable
        b8 result = renderer_end_frame(packet->delta_time);
        if (!result) {
            verror("Failed to end frame");
            return false;
        }
    }
    return true;
}