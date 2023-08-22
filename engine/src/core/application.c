#include "types.h"
#include "application.h"
#include "platform/platform.h"
#include "logger.h"
#include "core/mem.h"
#include "core/event.h"
/**
 * Internally initializes the platform state and returns it. This is per platform and should be called
 * by the platform specific code. This will also initialize the logger any all other core systems.
 */
typedef struct application_state {
  app_host *host; //< The host application
  b8 running; //< Whether the application is running
  b8 suspended; //< Whether the application is suspended
  platform_state platform; //< The platform state
  i16 width; //< The width of the surface
  i16 height; //< The height of the surface
  f64 last_time; //< The last time the application was updated
} application_state;

static b8 initialized = false;
static application_state state;

/**
 * Internally initializes the platform state and returns it. This is per platform and should be called
 * by the platform specific code. This will also initialize the logger any all other core systems.
 * @param config The configuration for the application
 * @return Whether or not the platform was successfully initialized
 */
b8 application_create(app_host *host) {
  if (initialized) {
    verror("Application already initialized")
    return false;
  }
  if (!host) {
    verror("Application host is null")
    return false;
  }
  state.host = host;
  //Initialize our subsystems
  if (!logger_initialize()) {
    verror("Failed to initialize the logger")
    return false;
  }
  state.running = true;
  state.suspended = false;

  if (!event_initialize()) {
    verror("Event system failed initialization. Application cannot continue.");
    return false;
  }
  if (!platform_startup(&state.platform,
                        host->config.title,
                        host->config.x,
                        host->config.y,
                        host->config.width,
                        host->config.height)) {
    verror("Failed to initialize the platform")
    return false;
  }
  if (!state.host->create(state.host)) {
    vfatal("Failed to initialize the application")
    return false;
  }
  vinfo("Initialized the platform")
  initialized = true;
  return true;
}

/**
 * Internally runs the application. This is per platform and should be called by the platform specific code.
 * @return Whether or not the platform was successfully initialized
 */
b8 application_run() {
  if (!initialized) {
    verror("Application not initialized")
    return false;
  }
  vinfo(mem_usage_str())
  while (state.running) {
    platform_pump_messages(&state.platform);
    if (!state.suspended) {
      f64 current_time = platform_get_time();
      f64 delta_time = current_time - state.last_time;
      state.last_time = current_time;
      // Update and render the application
      if (!state.host->update(state.host, (f32) delta_time) ||
          !state.host->render(state.host, (f32) delta_time)) {
        vfatal("Failed to update the application")
        state.running = false;
        break;
      }
    }
  }
  vwarn("Shutting down the application")
  state.running = false;
  event_shutdown();
  platform_shutdown(&state.platform);
  // Shutdown logger last just in case we need to log anything during shutdown
  logger_shutdown();
}