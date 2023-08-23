
#include "entry.h"
#include "app.h"

b8 create_application(app_host *app) {
  //configure our application
  app->config.x = 800;
  app->config.y = 400;
  app->config.width = 800;
  app->config.height = 600;
  app->config.title = "Vulkan Engine";
  app->create = app_init;
  app->update = app_update;
  app->render = app_render;
  app->on_resize = app_resize;
  // create our application state
  app->state = mem_alloc(sizeof(app_state), MEM_TAG_APPLICATION);
  return true;
}