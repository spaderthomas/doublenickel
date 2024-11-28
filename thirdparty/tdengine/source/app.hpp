#ifndef APP_H
#define APP_H
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
  u32 target_fps;
} dn_app_config_t;

typedef struct {
  dn_path_t install_path;
  dn_path_t engine_path;
  dn_path_t write_path;
  dn_path_t app_path;
} dn_app_t;
dn_app_t dn_app;

DN_API void dn_app_init(dn_app_descriptor_t descriptor);
DN_API void dn_app_configure(dn_app_config_t config);
#endif

#ifdef APP_IMPLEMENTATION
void dn_app_init(dn_app_descriptor_t descriptor) {
  copy_string(descriptor.engine_path, dn_app.engine_path, DN_MAX_PATH_LEN);
  copy_string(descriptor.install_path, dn_app.install_path, DN_MAX_PATH_LEN);
  copy_string(descriptor.write_path, dn_app.write_path, DN_MAX_PATH_LEN);
  copy_string(descriptor.app_path, dn_app.app_path, DN_MAX_PATH_LEN);
}

void dn_app_configure(dn_app_config_t config) {
  dn_log("%s", __func__);

  dn_window_init(config.window);
  dn_imgui_init();
  dn_assets_init(config.asset);
  dn_audio_init(config.audio);
  dn_font_init(config.font);
  dn_gpu_init(config.gpu);
  dn_noise_init();
  init_texture_atlas(); // Invert control
  init_backgrounds(); // Invert control
  init_screenshots(); // Use the asset loader
  init_particles();
  init_fluid();
}

#endif