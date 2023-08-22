//
// Created by jwraynor on 8/22/2023.
//

#pragma once

#include "defines.h"

/**
 * Defines all of our mouse buttons
 */
typedef enum buttons {
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_MIDDLE,
    BUTTON_MAX_BUTTONS
} buttons;

#define DEFINE_KEY(name, value) KEY_##name = value

/**
 * Defines all of our keys
 */
typedef enum keys {
    DEFINE_KEY(BACKSPACE, 0x08),
    DEFINE_KEY(TAB, 0x09),
    DEFINE_KEY(ENTER, 0x0D),
    DEFINE_KEY(SHIFT, 0x10),
    DEFINE_KEY(CTRL, 0x11),
    DEFINE_KEY(ALT, 0x12),
    DEFINE_KEY(PAUSE, 0x13),
    DEFINE_KEY(CAPSLOCK, 0x14),
    DEFINE_KEY(ESCAPE, 0x1B),
    DEFINE_KEY(SPACE, 0x20),
    DEFINE_KEY(PAGEUP, 0x21),
    DEFINE_KEY(PAGEDOWN, 0x22),
    DEFINE_KEY(END, 0x23),
    DEFINE_KEY(HOME, 0x24),
    DEFINE_KEY(LEFT, 0x25),
    DEFINE_KEY(UP, 0x26),
    DEFINE_KEY(RIGHT, 0x27),
    DEFINE_KEY(DOWN, 0x28),
    DEFINE_KEY(PRINTSCREEN, 0x2C),
    DEFINE_KEY(INSERT, 0x2D),
    DEFINE_KEY(DELETE, 0x2E),
    DEFINE_KEY(NUM0, 0x30),
    DEFINE_KEY(NUM1, 0x31),
    DEFINE_KEY(NUM2, 0x32),
    DEFINE_KEY(NUM3, 0x33),
    DEFINE_KEY(NUM4, 0x34),
    DEFINE_KEY(NUM5, 0x35),
    DEFINE_KEY(NUM6, 0x36),
    DEFINE_KEY(NUM7, 0x37),
    DEFINE_KEY(NUM8, 0x38),
    DEFINE_KEY(NUM9, 0x39),
    DEFINE_KEY(A, 0x41),
    DEFINE_KEY(B, 0x42),
    DEFINE_KEY(C, 0x43),
    DEFINE_KEY(D, 0x44),
    DEFINE_KEY(E, 0x45),
    DEFINE_KEY(F, 0x46),
    DEFINE_KEY(G, 0x47),
    DEFINE_KEY(H, 0x48),
    DEFINE_KEY(I, 0x49),
    DEFINE_KEY(J, 0x4A),
    DEFINE_KEY(K, 0x4B),
    DEFINE_KEY(L, 0x4C),
    DEFINE_KEY(M, 0x4D),
    DEFINE_KEY(N, 0x4E),
    DEFINE_KEY(O, 0x4F),
    DEFINE_KEY(P, 0x50),
    DEFINE_KEY(Q, 0x51),
    DEFINE_KEY(R, 0x52),
    DEFINE_KEY(S, 0x53),
    DEFINE_KEY(T, 0x54),
    DEFINE_KEY(U, 0x55),
    DEFINE_KEY(V, 0x56),
    DEFINE_KEY(W, 0x57),
    DEFINE_KEY(X, 0x58),
    DEFINE_KEY(Y, 0x59),
    DEFINE_KEY(Z, 0x5A),
    DEFINE_KEY(LEFTWINDOWS, 0x5B),
    DEFINE_KEY(RIGHTWINDOWS, 0x5C),
    DEFINE_KEY(NUMPAD0, 0x60),
    DEFINE_KEY(NUMPAD1, 0x61),
    DEFINE_KEY(NUMPAD2, 0x62),
    DEFINE_KEY(NUMPAD3, 0x63),
    DEFINE_KEY(NUMPAD4, 0x64),
    DEFINE_KEY(NUMPAD5, 0x65),
    DEFINE_KEY(NUMPAD6, 0x66),
    DEFINE_KEY(NUMPAD7, 0x67),
    DEFINE_KEY(NUMPAD8, 0x68),
    DEFINE_KEY(NUMPAD9, 0x69),
    DEFINE_KEY(MULTIPLY, 0x6A),
    DEFINE_KEY(ADD, 0x6B),
    DEFINE_KEY(SEPARATOR, 0x6C),
    DEFINE_KEY(SUBTRACT, 0x6D),
    DEFINE_KEY(DECIMAL, 0x6E),
    DEFINE_KEY(DIVIDE, 0x6F),
    DEFINE_KEY(F1, 0x70),
    DEFINE_KEY(F2, 0x71),
    DEFINE_KEY(F3, 0x72),
    DEFINE_KEY(F4, 0x73),
    DEFINE_KEY(F5, 0x74),
    DEFINE_KEY(F6, 0x75),
    DEFINE_KEY(F7, 0x76),
    DEFINE_KEY(F8, 0x77),
    DEFINE_KEY(F9, 0x78),
    DEFINE_KEY(F10, 0x79),
    DEFINE_KEY(F11, 0x7A),
    DEFINE_KEY(F12, 0x7B),
    DEFINE_KEY(NUMLOCK, 0x90),
    DEFINE_KEY(SCROLLLOCK, 0x91),
    DEFINE_KEY(LEFTSHIFT, 0xA0),
    DEFINE_KEY(RIGHTSHIFT, 0xA1),
    DEFINE_KEY(LEFTCTRL, 0xA2),
    DEFINE_KEY(RIGHTCTRL, 0xA3),
    DEFINE_KEY(LEFTALT, 0xA4),
    DEFINE_KEY(RIGHTALT, 0xA5),
    DEFINE_KEY(SEMICOLON, 0xBA),
    DEFINE_KEY(EQUALS, 0xBB),
    DEFINE_KEY(COMMA, 0xBC),
    DEFINE_KEY(MINUS, 0xBD),
    DEFINE_KEY(PERIOD, 0xBE),
    DEFINE_KEY(SLASH, 0xBF),
    DEFINE_KEY(GRAVE, 0xC0),
    DEFINE_KEY(MAX_KEYS, 0xFF)
} keys;

/**
 * Initialize our input system. This should include platform specific key and mouse initialization.
 */
b8 input_initialize();

/**
 * Shutdown our input system. This should include platform specific key and mouse shutdown.
 */
void input_shutdown();

/**
 * Internally poll/update the input mappings
 * @param delta_time the time since the last update
 */
void input_update(f64 delta_time);

/**
 * Returns whether or not the given key is down
 * @param key The key to check
 * @return Whether or not the key is down
 */
VAPI b8 input_is_key_down(keys key);

/**
 * Returns whether or not the given key is up
 * @param key The key to check
 * @return Whether or not the key is up
 */
VAPI b8 input_is_key_up(keys key);

/**
 * Returns whether or not the given key was pressed the last frame
 * @param key The key to check
 * @return Whether or not the key was pressed this frame
 */
VAPI b8 input_was_key_down(keys key);

/**
 * Returns whether or not the given key was released the last frame
 * @param key The key to check
 * @return Whether or not the key was released this frame
 */
VAPI b8 input_was_key_up(keys key);

/**
 * Used to internally update the input state based upon what's passed.
 * This is handled by the engine/platform
 * @param key the key to update the state for
 * @param pressed
 */
void input_process_key(keys key, b8 pressed);


/**
* Checks if the given mouse button is down
 * @param button The button to check
 * @return Whether or not the button is down
 */
VAPI b8 input_is_mouse_down(buttons button);

/**
 * Checks if the given mouse button is up
 * @param button The button to check
 * @return Whether or not the button is up
 */
VAPI b8 input_is_mouse_up(buttons button);

/**
 * Checks if the given mouse button was pressed the last frame
 * @param button The button to check
 * @return Whether or not the button was pressed last frame
 */
VAPI b8 input_was_mouse_down(buttons button);

/**
 * Checks if the given mouse button was released the last frame
 * @param button The button to check
 * @return Whether or not the button was released last frame
 */
VAPI b8 input_was_mouse_up(buttons button);
/**
 * Fills the given x and y with the current mouse position
 * @param x The x position of the mouse
 * @param y The y position of the mouse
 */
VAPI void input_mouse_position(i32 *x, i32 *y);

/**
 * Fills the given x and y with the last frame's mouse position
 * @param x The x delta of the mouse
 * @param y The y delta of the mouse
 */
VAPI void input_mouse_previous_position(i32 *x, i32 *y);

/**
 * Called by our platform to process a mouse button
 * @param button The button to process
 * @param pressed Whether or not the button is pressed
 */
void input_process_button(buttons button, b8 pressed);

/**
 * Called by our platform to process a mouse position
 * @param x The x position of the mouse
 * @param y The y position of the mouse
 */
void input_process_mouse_position(i16 x, i16 y);

/**
 * Called by our platform to process a mouse wheel
 * @param delta The delta of the mouse wheel
 */
void input_process_mouse_wheel(i8 delta);