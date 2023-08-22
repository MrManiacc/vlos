/**
 * Created by jraynor on 8/21/2023.
 */
#include "event.h"
#include "mem.h"
#include "logger.h"
#include "containers/darray.h"

typedef struct registered_event {
  void *listener_instance;
  PF_EVENT_CALLBACK callback;
} registered_event;

typedef struct event_code_entry {
  registered_event *events;
} event_code_entry;

#define MAX_EVENT_CODES 16384

typedef struct event_system_state {
  event_code_entry registered[MAX_EVENT_CODES];
} event_system_state;

static b8 is_initialized = false;
static event_system_state state;

b8 event_initialize() {
  if (is_initialized) {
    verror("Event system already initialized");
    return false;
  }
  mem_zero(&state, sizeof(event_system_state));
  is_initialized = true;

  return true;
}

void event_shutdown() {
  for (u16 i = 0; i < MAX_EVENT_CODES; i++) {
    if (state.registered[i].events != 0) {
      darray_destroy(state.registered[i].events);
      state.registered[i].events = 0;
    }
  }
}

b8 event_register(u16 event_code, void *listener_instance, PF_EVENT_CALLBACK callback) {
  if (!is_initialized) {
    verror("Event system not initialized");
    return false;
  }
  if (event_code >= MAX_EVENT_CODES) {
    verror("Event code is out of range");
    return false;
  }

  if (state.registered[event_code].events == 0) {
    state.registered[event_code].events = darray_create(registered_event);
  }

  u64 registered_count = darray_length(state.registered[event_code].events);
  for (u64 i = 0; i < registered_count; i++) {
    if (state.registered[event_code].events[i].listener_instance == listener_instance) {
      vwarn("Listener already registered for event code: %d", event_code);
      return false;
    }
  }

  registered_event event = {
      .listener_instance = listener_instance,
      .callback = callback
  };
  darray_push(state.registered[event_code].events, event);
  return true;
}

b8 event_unregister(u16 event_code, void *listener_instance) {
  if (!is_initialized) {
    verror("Event system not initialized");
    return false;
  }
  if (event_code >= MAX_EVENT_CODES) {
    verror("Event code is out of range");
    return false;
  }

  if (state.registered[event_code].events == 0) {
    vwarn("No listeners registered for event code: %d", event_code);
    return false;
  }

  u64 registered_count = darray_length(state.registered[event_code].events);
  for (u64 i = 0; i < registered_count; i++) {
    if (state.registered[event_code].events[i].listener_instance == listener_instance) {
      registered_event event;
      darray_pop_at(state.registered[event_code].events, i, &event);
      return true;
    }
  }

  vwarn("No listeners registered for event code: %d", event_code);
  return false;
}

b8 event_trigger(u16 event_code, void *sender, event_context context_data) {
  if (!is_initialized) {
    verror("Event system not initialized");
    return false;
  }
  if (event_code >= MAX_EVENT_CODES) {
    verror("Event code is out of range");
    return false;
  }

  if (state.registered[event_code].events == 0) {
//    vwarn("No listeners registered for event code: %d", event_code);
    return false;
  }

  u64 registered_count = darray_length(state.registered[event_code].events);
  for (u64 i = 0; i < registered_count; i++) {
    registered_event event = state.registered[event_code].events[i];
    if (event.callback(event_code, sender, event.listener_instance, context_data)) {
      vwarn("Event callback returned true for event code: %d", event_code);
      return true;
    }
  }
  return false;
}