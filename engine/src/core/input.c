//
// Created by jwraynor on 8/22/2023.
//
#include "core/input.h"
#include "core/event.h"
#include "core/mem.h"
#include "core/logger.h"


typedef struct keyboard_state {
    b8 keys[KEY_MAX_KEYS];
} keyboard_state;

typedef struct mouse_state {
    b8 buttons[BUTTON_MAX_BUTTONS];
    i16 x;
    i16 y;
} mouse_state;


typedef struct input_state {
    keyboard_state keyboard_current;
    keyboard_state keyboard_previous;
    mouse_state mouse_current;
    mouse_state mouse_previous;
} input_state;


static b8 initialize = false;
static input_state state = {};

b8 input_initialize() {
    if (initialize) {
        vwarn("Input already initialized")
        return false;
    }
    mem_zero(&state, sizeof(input_state));
    initialize = true;
    return true;

}

void input_shutdown() {
    if (!initialize) {
        vwarn("Input not initialized")
        return;
    }
    initialize = false;
}

void input_update(f64 delta_time) {
    if (!initialize) {
        vwarn("Input not initialized")
        return;
    }
    //move our current states to our previous states
    mem_copy(&state.keyboard_previous, &state.keyboard_current, sizeof(keyboard_state));
    mem_copy(&state.mouse_previous, &state.mouse_current, sizeof(mouse_state));
}

void input_process_key(keys key, b8 pressed) {
    if (!initialize) {
        vwarn("Input not initialized")
        return;
    }
    if (state.keyboard_current.keys[key] != pressed) {
        //update the state
        state.keyboard_current.keys[key] = pressed;
        //Fire event for processing
        event_context context;
        context.u16[0] = key;
        event_trigger(pressed ? SYSTEM_EVENT_CODE_KEY_PRESS : SYSTEM_EVENT_CODE_KEY_RELEASE, 0, context);
    }
}

void input_process_button(buttons button, b8 pressed) {
    if (state.mouse_current.buttons[button] != pressed) {
        //update the state
        state.mouse_current.buttons[button] = pressed;
        //Fire event for processing
        event_context context;
        context.u16[0] = button;
        event_trigger(pressed ? SYSTEM_EVENT_CODE_BUTTON_PRESS : SYSTEM_EVENT_CODE_BUTTON_RELEASE, 0, context);
    }
}

void input_process_mouse_position(i16 x, i16 y) {
    if (state.mouse_current.x != x || state.mouse_current.y != y) {
        //update the mouse state
        state.mouse_current.x = x;
        state.mouse_current.y = y;
        //Fire the event for processing
        event_context context;
        context.i16[0] = x;
        context.i16[1] = y;
        event_trigger(SYSTEM_EVENT_CODE_MOUSE_MOVE, 0, context);
    }
}

void input_process_mouse_wheel(i8 delta) {
    //Fire the event for processing
    event_context context;
    context.i16[0] = delta;
    event_trigger(SYSTEM_EVENT_CODE_MOUSE_WHEEL, 0, context);
}

b8 input_is_key_down(keys key) {
    if (!initialize) {
        vwarn("Input not initialized")
        return false;
    }
    return state.keyboard_current.keys[key] == true;
}


b8 input_is_key_up(keys key) {
    if (!initialize) {
        vwarn("Input not initialized")
        return true; //true because it's our default state
    }
    return state.keyboard_current.keys[key] == false;
}


b8 input_was_key_down(keys key) {
    if (!initialize) {
        vwarn("Input not initialized")
        return false;
    }
    return state.keyboard_previous.keys[key] == true;
}


b8 input_was_key_up(keys key) {
    if (!initialize) {
        vwarn("Input not initialized")
        return false; //true because it's our default state
    }
    return state.keyboard_previous.keys[key] == false;
}


b8 input_is_mouse_down(buttons button) {
    if (!initialize) {
        vwarn("Input not initialized")
        return false;
    }
    return state.mouse_current.buttons[button] == true;
}


b8 input_is_mouse_up(buttons button) {
    if (!initialize) {
        vwarn("Input not initialized")
        return true; //true because it's our default state
    }
    return state.mouse_current.buttons[button] == false;
}

b8 input_was_mouse_down(buttons button) {
    if (!initialize) {
        vwarn("Input not initialized")
        return false;
    }
    return state.mouse_previous.buttons[button] == true;
}


b8 input_was_mouse_up(buttons button) {
    if (!initialize) {
        vwarn("Input not initialized")
        return false;
    }
    return state.mouse_previous.buttons[button] == false;
}


void input_mouse_position(i32 *x, i32 *y) {
    if (!initialize) {
        vwarn("Input not initialized")
        *x = 0;
        *y = 0;
        return;
    }
    *x = state.mouse_current.x;
    *y = state.mouse_current.y;
}


void input_mouse_previous_position(i32 *x, i32 *y) {
    if (!initialize) {
        vwarn("Input not initialized")
        return;
    }
    *x = state.mouse_previous.x;
    *y = state.mouse_previous.y;
}