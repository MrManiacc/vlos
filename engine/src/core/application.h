
#pragma once

#include "defines.h"

struct app_host;
/**
 * Our application configuration struct
 * @note This is not platform specific
 */
typedef struct application_config{
  i16 x; //< The x position of the window
  i16 y; //< The y position of the window
  i16 width; //< The width of the window
  i16 height; //< The height of the window
  str title; //< The title of the window
} application_config;

/**
 * Internally initializes the platform state and returns it. This is per platform and should be called
 * by the platform specific code. This will also initialize the logger any all other core systems.
 * @param config The configuration for the application
 * @return Whether or not the platform was successfully initialized
 */
VAPI b8 application_create(struct app_host *host);
/**
 * Internally runs the application. This is per platform and should be called by the platform specific code.
 * @return Whether or not the platform was successfully initialized
 */
VAPI b8 application_run();