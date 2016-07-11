#include <pebble.h>
#include "effect_layer.h"

static Window *s_main_window;
static GDrawCommandImage *s_command_image;
static Layer *s_canvas_layer;
EffectLayer* h1;


static void update_proc(Layer *layer, GContext *ctx) {
  // Set the origin offset from the context for drawing the image
  GPoint origin = GPoint(0, 0);

  // Draw the GDrawCommandImage to the GContext
  gdraw_command_image_draw(ctx, s_command_image, origin);
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

   // Create the canvas Layer
  s_canvas_layer = layer_create(GRect(0, 0, bounds.size.w, bounds.size.h));

  // Set the LayerUpdateProc
  layer_set_update_proc(s_canvas_layer, update_proc);

  // Add to parent Window
  layer_add_child(window_layer, s_canvas_layer);
}

static void main_window_unload(Window *window) {
  layer_destroy(s_canvas_layer);
  gdraw_command_image_destroy(s_command_image);
}

static void init() {
  // Create the object from resource file
  s_command_image = gdraw_command_image_create_with_resource(RESOURCE_ID_FROG_CLOUD);
  
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlueMoon);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
  
  h1 = effect_layer_create(GRect(0,0,110,168));
  
  EffectColorpair colorpair;
  colorpair.firstColor = GColorWhite;
  colorpair.secondColor = GColorGreen;  
  effect_layer_add_effect(h1, effect_colorswap, &colorpair);
  layer_add_child(window_get_root_layer(s_main_window), effect_layer_get_layer(h1));
}

static void deinit() {
  window_destroy(s_main_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
