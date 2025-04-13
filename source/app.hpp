#ifndef DN_APP_H
#define DN_APP_H
/* 
Since Doublenickel is intended to be consumed via source, and since it contains iterpreted code which needs
to be located at runtime (as opposed to just C++ source), we need a way to locate those sources. Also, a core
idea in my programs is to never use relative paths. They're too error prone. Instead, we build absolute paths
at initialization time.

The paths which we need are:
1. install_path: Where is the game installed, relative to the executable? In a packaged build, this will probably
   be the same directory, but in debug builds this isn't the case. This is needed to build any path relative to
   the top-level directory where the game is installed

2. engine_path: Where are the Doublenickel Lua sources? We need to bootstrap the engine, and to do so we need to
   know where the source code even is.

3. write_path: Where should runtime data be written to? In packaged builds, this is relative to the OS-approved
   write directory (i.e. AppData)

4. app_path: Where are the app's Lua sources?
*/
typedef struct {
  const char* install_path;
  const char* engine_path;
  const char* write_path;
  const char* app_path;
} dn_app_descriptor_t;

typedef struct {
  dn_window_config_t window;
  dn_audio_config_t audio;
  dn_font_config_t font;
  dn_gpu_config_t gpu;
  dn_asset_config_t asset;
  dn_steam_config_t steam;
  dn_image_config_t image;
  u32 target_fps;
} dn_app_config_t;

typedef struct {
  dn_path_t install_path;
  dn_path_t engine_path;
  dn_path_t write_path;
  dn_path_t app_path;
} dn_app_t;
dn_app_t dn_app;

DN_IMP void dn_app_init(dn_app_descriptor_t descriptor);
DN_IMP void dn_app_default();
DN_IMP void dn_main();
DN_API void dn_app_configure(dn_app_config_t config);
#endif

#ifdef DN_APP_IMPLEMENTATION
void dn_app_init(dn_app_descriptor_t descriptor) {
  snprintf(dn_app.install_path, DN_MAX_PATH_LEN, "%s", descriptor.install_path);
  snprintf(dn_app.engine_path, DN_MAX_PATH_LEN, "%s/%s", descriptor.install_path, descriptor.engine_path);
  snprintf(dn_app.write_path, DN_MAX_PATH_LEN, "%s/%s", descriptor.install_path, descriptor.write_path);
  snprintf(dn_app.app_path, DN_MAX_PATH_LEN, "%s/%s", descriptor.install_path, descriptor.app_path);
}

void dn_app_configure(dn_app_config_t config) {
  dn_log("%s", __func__);

  dn_window_init(config.window);
  dn_imgui_init();
  dn_assets_init(config.asset);
  dn_audio_init(config.audio);
  dn_font_init(config.font);
  dn_gpu_init(config.gpu);
  dn_images_init(config.image);
  dn_noise_init();
  // init_backgrounds(); // Invert control
  // init_screenshots(); // Use the asset loader
  // init_particles();
  // init_fluid();
  dn_steam_init(config.steam);
}

int dn_main(dn_app_descriptor_t app) {
  dn_allocators_init();
  dn_random_init();
  dn_test_init();
  dn_app_init(app);
  dn_paths_init();
  dn_log_init();
  dn_engine_init();
  dn_input_init();
  dn_time_metrics_init();
  dn_file_monitors_init();
  dn_lua_init();
  init_actions();
  dn_lua_init_game();

  while(!dn_engine_should_exit()) {
    dn_engine_update();
    dn_allocators_update();
    dn_file_monitors_update();
    dn_assets_update();
    dn_imgui_update();
    dn_input_update();
    update_actions();
    dn_lua_update();
    dn_time_metrics_update();
  }

  dn_audio_shutdown();
  dn_steam_shutdown();
  dn_imgui_shutdown();
  dn_window_shutdown();

  return 0;
}

#endif