/**
 * Created by jraynor on 8/21/2023.
 */
#include "types.h"
#include "application.h"
#include "platform/platform.h"
#include "logger.h"

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
  initialize_logger();
  vinfo("Initialized the logger")
  state.running = true;
  state.suspended = false;
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
  while (state.running) {
    platform_pump_messages(&state.platform);
    if (!state.suspended) {
      f64 current_time = platform_get_time();
      f64 delta_time = current_time - state.last_time;
      state.last_time = current_time;
      if (!state.host->update(state.host, (f32) delta_time)) {
        vfatal("Failed to update the application")
        state.running = false;
        break;
      }
      if (!state.host->render(state.host, (f32) delta_time)) {
        vfatal("Failed to render the application")
        state.running = false;
        break;
      }
//      platform_update(&app_state.platform, delta_time);
    }
  }
  vwarn("Shutting down the application")
  platform_shutdown(&state.platform);
  // Shutdown logger last just in case we need to log anything during shutdown
  shutdown_logger();
}