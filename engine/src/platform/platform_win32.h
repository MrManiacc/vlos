//
// Created by jwraynor on 8/21/2023.
//

#pragma once

#include "platform/platform.h"

#if PLATFORM_WINDOWS

#include <windows.h>
#include <windowsx.h>

typedef struct internal_state {
    HINSTANCE hInstance; // current instance
    HWND hWnd;  // handle to window to open
} internal_state;

b8 platform_startup(platform_state *platform_state, string str, i32 x, i32 y, i32 width, i32 height) {
    //initialize our internal state to the size of the internal windows state
    platform_state->internal_state = malloc(sizeof(internal_state));

    internal_state *state = (internal_state *) platform_state->internal_state;
    //gets handle to application currently executing code (our owned application)
    state->hInstance = GetModuleHandle(
            NULL);

}

#endif