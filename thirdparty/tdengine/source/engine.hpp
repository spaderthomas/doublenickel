#ifndef DN_ENGINE_H
#define DN_ENGINE_H
struct Engine {
  float32 target_fps;
  float32 dt;
  float32 elapsed_time;
  int32 frame = 0;
  bool exit_game = false;
  bool steam = false;
};
Engine engine;

typedef struct {
  const char* name;
  u32 size;
} dn_type_info_t;
#define dn_type_info(t) { .name = dn_macro_str(t), .size = sizeof(t) }

typedef struct {
  dn_type_info_t* data;
  u32 count;
} dn_type_info_list_t;

DN_API void                dn_engine_set_exit_game();
DN_API const char*         dn_engine_get_game_hash();
DN_API void                dn_engine_set_target_fps(double fps);
DN_API double              dn_engine_get_target_fps();
DN_API bool                dn_engine_exceeded_frame_time();
DN_API bool                dn_engine_should_exit();
DN_API dn_type_info_list_t dn_engine_query_types();
DN_IMP void                dn_engine_init();
DN_IMP void                dn_engine_update();
#endif



#ifdef DN_ENGINE_IMPLEMENTATION
void dn_engine_init() {
  dn_engine_set_target_fps(60);
}

void dn_engine_update() {
  dn_time_metric_begin("frame");

  // Clearly this is not representative of elapsed real time
  engine.elapsed_time += engine.dt;
  engine.frame++;
}

dn_type_info_list_t dn_engine_query_types() {
  dn_type_info_list_t list = dn_zero_initialize();

  static dn_type_info_t types [] = {
    dn_type_info(dn_app_config_t),
    dn_type_info(dn_app_descriptor_t),
    dn_type_info(dn_asset_t),
    dn_type_info(dn_asset_completion_status_t),
    dn_type_info(dn_asset_config_t),
    dn_type_info(dn_asset_data_t),
    dn_type_info(dn_asset_importer_t),
    dn_type_info(dn_asset_import_request_t),
    dn_type_info(dn_asset_name_t),
    dn_type_info(dn_audio_config_t),
    dn_type_info(dn_compressor_t),
    dn_type_info(dn_coord_data_t),
    dn_type_info(dn_fixed_array_t),
    dn_type_info(dn_font_config_t),
    dn_type_info(dn_font_descriptor_t),
    dn_type_info(dn_gen_arena_handle_t),
    dn_type_info(dn_gpu_backed_buffer_t),
    dn_type_info(dn_gpu_buffer_t),
    dn_type_info(dn_gpu_buffer_binding_t),
    dn_type_info(dn_gpu_buffer_descriptor_t),
    dn_type_info(dn_gpu_config_t),
    dn_type_info(dn_gpu_command_buffer_descriptor_t),
    dn_type_info(dn_gpu_draw_call_t),
    dn_type_info(dn_gpu_pipeline_descriptor_t),
    dn_type_info(dn_gpu_raster_state_t),
    dn_type_info(dn_gpu_render_pass_t),
    dn_type_info(dn_gpu_render_target_t),
    dn_type_info(dn_gpu_render_target_descriptor_t),
    dn_type_info(dn_gpu_renderer_state_t),
    dn_type_info(dn_gpu_scissor_state_t),
    dn_type_info(dn_gpu_shader_descriptor_t),
    dn_type_info(dn_gpu_storage_buffer_binding_t),
    dn_type_info(dn_gpu_uniform_t),
    dn_type_info(dn_gpu_uniform_binding_t),
    dn_type_info(dn_gpu_uniform_descriptor_t),
    dn_type_info(dn_gpu_uniform_data_t),
    dn_type_info(dn_gpu_vertex_attr_info_t),
    dn_type_info(dn_gpu_vertex_buffer_binding_t),
    dn_type_info(dn_low_pass_filter_t),    
    dn_type_info(dn_named_path_t),
    dn_type_info(dn_named_path_result_t),
    dn_type_info(dn_os_directory_entry_t),
    dn_type_info(dn_os_directory_entry_list_t),
    dn_type_info(dn_prepared_text_t),
    dn_type_info(dn_sdf_circle_t),
    dn_type_info(dn_sdf_combine_entry_t),
    dn_type_info(dn_sdf_combine_header_t),
    dn_type_info(dn_sdf_combine_op_t),
    dn_type_info(dn_sdf_header_t),
    dn_type_info(dn_sdf_instance_t),
    dn_type_info(dn_sdf_oriented_box_t),
    dn_type_info(dn_sdf_renderer_t),
    dn_type_info(dn_sdf_renderer_state_t),
    dn_type_info(dn_sdf_ring_t),
    dn_type_info(dn_sdf_shape_t),
    dn_type_info(dn_sdf_smoothing_kernel_t),
    dn_type_info(dn_type_info_t),
    dn_type_info(dn_type_info_list_t),
    dn_type_info(dn_window_config_t),
  };

  return {
    .data = types,
    .count = DN_ARR_LEN(types)
  };
}
void dn_engine_set_target_fps(float64 fps) {
  engine.target_fps = fps;
  engine.dt = 1.f / fps;
}


double dn_engine_get_target_fps() {
  return engine.target_fps;
}

void dn_engine_set_exit_game() {
  engine.exit_game = true;
}

const char* dn_engine_get_game_hash() {
  return "hehe";
  // return GIT_HASH;
}

bool dn_engine_should_exit() {
  bool done = false;
  done |= (bool)glfwWindowShouldClose(window.handle);
  done |= engine.exit_game;
  return done;
}

bool dn_engine_exceeded_frame_time() {
  auto& frame_timer = time_metrics["frame"];
  auto now = glfwGetTime();
  double elapsed = (now - frame_timer.time_begin);
  return elapsed >= engine.dt;
}

#endif