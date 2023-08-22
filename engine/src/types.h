/**
 * Created by jraynor on 8/21/2023.
 */
#pragma once
#include "defines.h"
#include "core/application.h"


typedef struct app_host {
  /**
   * The configuration for the application
   */
  application_config config;
  /**
   * Creates the application. This is called once at the start of the application.
   * @param app The application to create
   * @return Whether or not the application was successfully created
   */
  b8 (*create)(struct app_host *app);
  /**
   * Updates the application. This is called every frame. This is where the application should update its state.
   * @param app The application to update
   * @param delta_time The time since the last frame
   * @return
   */
  b8 (*update)(struct app_host *app, f32 delta_time);
  /**
   * Renders the application. This is called every frame.
   * @param app The application to render
   * @param delta_time The time since the last frame
   * @return Whether or not the application was successfully rendered
   */
  b8 (*render)(struct app_host *app, f32 delta_time);
  /**
   * User specific state, created and managed by the app host.
   */
  void *state;
} app_host;
