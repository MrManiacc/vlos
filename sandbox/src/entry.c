/**
 * Created by jraynor on 8/21/2023.
 */
#include "entry.h"
#include "app.h"
#include "platform/platform.h"


b8 create_application(app_host *app) {
  app->config.x = 100;
  app->config.y = 100;
  app->config.width = 800;
  app->config.height = 600;
  app->config.title = "Vulkan Engine";
  app->create = app_init;
  app->update = app_update;
  app->render = app_render;
  app->state = platform_allocate(sizeof(app_state), false);
  return true;
}