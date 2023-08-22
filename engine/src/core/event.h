#pragma once

#include "defines.h"
/**
 * Event context data that is passed to the event callback
 */
typedef struct event_context {
  union {
    i64 i64[2];
    u64 u64[2];
    f64 f64[2];

    i32 i32[4];
    u32 u32[4];
    f32 f32[4];

    i16 i16[8];
    u16 u16[8];

    i8 i8[16];
    u8 u8[16];

    char c[16];
  };
} event_context;

b8 event_initialize();

void event_shutdown();

/**
 * Register an event listener for a specific event code
 * @param event_code The event code to listen for
 * @param listener_instance The instance of the listener
 * @param callback The callback to call when the event is triggered
 * @return Whether or not the listener was successfully registered
 *
 * @note The listener instance is used to determine which listener to unregister
 */
typedef b8 (*PF_EVENT_CALLBACK)(u16 event_code, void *sender, void *listener_instance, event_context context_data);

/**
 * Register to listen for when events with the provided event code are triggered. Events with
 * duplicate event codes will NOT be registered again and will return false.
 * @param event_code The event code to listen for
 * @param listener_instance The instance of the listener
 * @param callback  The callback to call when the event is triggered
 * @return Whether or not the listener was successfully registered
 */
VAPI b8 event_register(u16 event_code, void *listener_instance, PF_EVENT_CALLBACK callback);


/**
 * Unregister an event listener for a specific event code
 * @param event_code The event code to unregister
 * @param listener_instance The instance of the listener
 * @return Whether or not the listener was successfully unregistered
 */
VAPI b8 event_unregister(u16 event_code, void *listener_instance);

/**
 * Trigger an event with the provided event code and context data
 * @param event_code The event code to trigger
 * @param context_data The context data to pass to the event callback
 * @return Whether or not the event was marked as handled.
 */
VAPI b8 event_trigger(u16 event_code, void *sender, event_context context_data);

//System internal codes that are used to trigger events. Starts at 0x01 and goes up to 0xFF (255).
typedef enum system_event_code {
  //Shuts down the application on the next frame.
  SYSTEM_EVENT_CODE_QUIT = 0x01,
  //A key was pressed.
  // Usage:
  //    u16 key_code = context_data.u16[0];
  SYSTEM_EVENT_CODE_KEY_PRESS = 0x02,
  //A key was released.
  // Usage:
  //    u16 key_code = context_data.u16[0];
  SYSTEM_EVENT_CODE_KEY_RELEASE = 0x03,
  //A mouse button was pressed.
  // Usage:
  //    u16 key_code = context_data.u16[0];
  SYSTEM_EVENT_CODE_BUTTON_PRESS = 0x04,
  //A mouse button was released.
  // Usage:
  //    u16 key_code = context_data.u16[0];
  SYSTEM_EVENT_CODE_BUTTON_RELEASE = 0x05,
  //The mouse was moved.
  // Usage:
  //    i16 x = context_data.i16[0];
  //    i16 y = context_data.i16[1];
  SYSTEM_EVENT_CODE_MOUSE_MOVE = 0x06,
  //The mouse wheel was scrolled.
  // Usage:
  //    i16 x = context_data.i16[0];
  //    i16 y = context_data.i16[1];
  SYSTEM_EVENT_CODE_MOUSE_WHEEL = 0x07,
  //The window was resized.
  // Usage:
  //    i16 width = context_data.i16[0];
  //    i16 height = context_data.i16[1];
  SYSTEM_EVENT_CODE_WINDOW_RESIZE = 0x08,
  //The window was moved.
  // Usage:
  //    i16 x = context_data.i16[0];
  //    i16 y = context_data.i16[1];
  SYSTEM_EVENT_CODE_WINDOW_MOVED = 0x08,

  // The maximum number of system event codes
  SYSTEM_EVENT_CODE_COUNT = 0xFF
} system_event_code;
