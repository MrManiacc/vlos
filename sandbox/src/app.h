
#pragma once

#include "types.h"

typedef struct app_state {
    f32 delta_time;
} app_state;

b8 app_init(app_host *host);

b8 app_update(app_host *host, f32 delta_time);

b8 app_render(app_host *host, f32 delta_time);

b8 app_resize(app_host *host, u16 width, u16 height);