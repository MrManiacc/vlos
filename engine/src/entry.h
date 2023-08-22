#pragma once


#include "core/application.h"
#include "core/logger.h"
#include "core/mem.h"
#include "types.h"

/**
 * Externally create the application. This is called once at the start of the application.
 * This is defined outside of the context of this assembly. This is defined by the user.
 *
 * @param app The application to create
 * @return Whether or not the application was successfully created
 */
extern b8 create_application(app_host *app);

int main(void) {
  mem_init();
  app_host app;
  // Create the application
  if (!create_application(&app)) {
    verror("Failed to create application");
    return -1;
  }
  vdebug("Created the application")
  // Check that the application has the required functions
  if (!app.render || !app.update || !app.create) {
    vfatal("Application is missing required functions");
    return -2;
  }
  vdebug("Application has required functions")
  //Initialize our subsystems
  if (!application_create(&app)) {
    vfatal("Failed to create application");
    return 1;
  }
  vdebug("Initialized the application")
  // Begin the game loop
  if (!application_run()) {
    vwarn("Failed to shutdown gracefully");
    return 2;
  }
  mem_shutdown();
  return 0;
}

