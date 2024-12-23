#ifndef DN_AUDIO_H
#define DN_AUDIO_H

typedef enum {
  DN_AUDIO_FILTER_MODE_FIRST_ORDER = 0,
  DN_AUDIO_FILTER_MODE_BUTTERWORTH = 1,
} dn_audio_filter_mode_t;

typedef struct {
  dn_audio_filter_mode_t mode;
  bool enabled;
  float cutoff_frequency;
  float cutoff_alpha;
  float a0, a1, a2, b1, b2;
  float input_history [2];
  float output_history [2];
} dn_low_pass_filter_t;

typedef struct {
  float threshold;
  float ratio;
  float attack_time;
  float release_time;
  bool enabled;
} dn_compressor_t;

typedef dn_gen_arena_handle_t dn_audio_info_handle_t;
typedef struct {
  dn_asset_name_t name;
  dn_hash_t hash;
  u32 num_channels;
  u32 sample_rate;
  u64 num_frames;
  u32 num_samples;
  float* samples;
  u32 generation;
  double file_mod_time;
} dn_audio_info_t;

typedef dn_gen_arena_handle_t dn_audio_instance_handle_t;
typedef struct {
  dn_audio_info_handle_t info;
  dn_audio_instance_handle_t next;
  u32 next_sample;
  bool loop;
  float volume;
  dn_low_pass_filter_t filter;
  bool paused;
  i32 sample_buffer_offset;
  i32 samples_from_next;
  
  bool occupied;
  u32 generation;
} dn_audio_instance_t;

typedef struct {
  dn_path_t* dirs;
  u32 num_dirs;
  dn_compressor_t compressor;
  dn_low_pass_filter_t filter;
  float sample_frequency;
  float master_volume;
  float master_volume_mod;
} dn_audio_config_t;

typedef struct {
  dn_compressor_t compressor;
  dn_low_pass_filter_t filter;
  float sample_frequency;
  float master_volume;
  float master_volume_mod;
  FileMonitor* file_monitor;
  dn_array_t<float> sample_buffer;
  dn_array_t<dn_audio_info_t, 1024> sounds;
  dn_array_t<dn_audio_instance_t, 128> instances;
} dn_audio_t;
dn_audio_t dn_audio;
std::recursive_mutex dn_audio_mutex;

DN_API void                       dn_audio_set_compressor_threshold(float t);
DN_API void                       dn_audio_set_compressor_ratio(float v);
DN_API void                       dn_audio_set_compressor_attack(float v);
DN_API void                       dn_audio_set_compressor_release(float v);
DN_API void                       dn_audio_set_sample_rate(float v);
DN_API float                      dn_audio_get_master_volume();
DN_API void                       dn_audio_set_master_volume(float v);
DN_API float                      dn_audio_get_master_volume_mod();
DN_API void                       dn_audio_set_master_volume_mod(float v);
DN_API float                      dn_audio_get_master_filter_cutoff();
DN_API void                       dn_audio_set_master_filter_cutoff(float v);
DN_API void                       dn_audio_set_master_filter_cutoff_enabled(bool enabled);
DN_API void                       dn_audio_set_master_filter_mode(dn_audio_filter_mode_t mode);
DN_API void                       dn_audio_set_volume(dn_audio_instance_handle_t handle, float volume);
DN_API void                       dn_audio_set_filter_cutoff(dn_audio_instance_handle_t handle, float cutoff);
DN_API void                       dn_audio_set_filter_mode(dn_audio_instance_handle_t handle, float cutoff);
DN_API void                       dn_audio_set_filter_enabled(dn_audio_instance_handle_t handle, bool enabled);
DN_API dn_audio_instance_handle_t dn_audio_play_sound(const char* name);
DN_API dn_audio_instance_handle_t dn_audio_play_looped(const char* name);
DN_API void                       dn_audio_queue(dn_audio_instance_handle_t current, dn_audio_instance_handle_t next);
DN_API void                       dn_audio_stop(dn_audio_instance_handle_t handle);
DN_API void                       dn_audio_stop_all();
DN_API void                       dn_audio_pause(dn_audio_instance_handle_t handle);
DN_API void                       dn_audio_resume(dn_audio_instance_handle_t handle);
DN_API bool                       dn_audio_is_playing(dn_audio_instance_handle_t handle);
DN_API bool                       dn_audio_is_any_playing();
DN_API void                       dn_audio_load(const char* file_path, const char* file_name);
DN_API void                       dn_audio_load_dir(const char* path);
DN_API void                       dn_low_pass_filter_set_mode(dn_low_pass_filter_t* filter, dn_audio_filter_mode_t mode);
DN_API void                       dn_low_pass_filter_set_cutoff(dn_low_pass_filter_t* filter, float cutoff);
DN_API float                      dn_low_pass_filter_apply(dn_low_pass_filter_t* filter, float input);
DN_API dn_audio_config_t          dn_audio_config_default();
DN_API void                       dn_audio_init(dn_audio_config_t config);
DN_IMP void                       dn_audio_update(float* buffer, int frames_requested, int num_channels);
DN_IMP void                       dn_audio_shutdown();
DN_IMP dn_audio_info_t*           dn_audio_find(const char* name);
DN_IMP dn_audio_info_t*           dn_audio_find_no_default(const char* name);
DN_IMP dn_audio_instance_t*       dn_audio_resolve(dn_audio_instance_handle_t handle);
DN_IMP dn_audio_instance_handle_t dn_audio_reserve();
DN_IMP dn_audio_instance_handle_t dn_audio_play_sound_ex(dn_audio_info_t* sound, bool loop);
DN_IMP void                       dn_audio_stop_ex(dn_audio_instance_t* active_sound);
#endif



#ifdef DN_AUDIO_IMPLEMENTATION
using sound_iterator = std::function<void(const char*)>;

dn_audio_config_t dn_audio_config_default() {
  return {
    .dirs = NULL,
    .num_dirs = 0,
    .compressor = {
      .threshold = 0.5f,
      .ratio = 2.0f,
      .attack_time = 0.95f,
      .release_time = 1.0f,
      .enabled = true
    },
    .filter = {
      .mode = DN_AUDIO_FILTER_MODE_BUTTERWORTH,
      .enabled = false,
      .cutoff_frequency = 10000,
      .cutoff_alpha = 0,
      .a0 = 0.f, .a1 = 0.f, .a2 = 0.f, .b1 = 0.f, .b2 = 0.f,
      .input_history = { 0 },
      .output_history = { 0 },
    },
    .sample_frequency = 44100,
    .master_volume = 1.0f,
    .master_volume_mod = 1.0f,
  };
}

void dn_audio_init(dn_audio_config_t config) {
  dn_log("%s", __func__);

  dn_audio = {
    .compressor = config.compressor,
    .filter = config.filter,
    .sample_frequency = config.sample_frequency,
    .master_volume = config.master_volume,
    .master_volume_mod = config.master_volume_mod,
    .file_monitor = dn_file_monitors_add(),
    .sample_buffer = {0}
  };

  dn_array_init(&dn_audio.sounds);
  dn_array_init(&dn_audio.instances);

  dn_low_pass_filter_set_cutoff(&dn_audio.filter, 10000);

  auto on_file_event = [](FileMonitor* monitor, FileChange* event, void* userdata) {
    dn_log("dn_audio_init: Reloading audio file; file = %s", __func__, event->file_path);
    dn_audio_load(event->file_path, event->file_name);
  };

  auto events = DN_FILE_CHANGE_EVENT_ADDED | DN_FILE_CHANGE_EVENT_MODIFIED;
  dn_audio.file_monitor->init(on_file_event, events, nullptr);
  
  dn_log("%s: Loading default audio directory; directory = %s", __func__, dn_paths_resolve("dn_audio"));
  dn_audio_load_dir(dn_paths_resolve("dn_audio"));
  for (u32 i = 0; i < config.num_dirs; i++) {
    dn_audio_load_dir(config.dirs[i]);
  }

  // Initialize the audio backend
  dn_log("%s: Initializing audio backend", __func__);
  saudio_desc descriptor = { 0 };
  descriptor.num_channels = 2;
  descriptor.buffer_frames = 2048;
  descriptor.logger.func = slog_func;
  descriptor.stream_cb = dn_audio_update;
  saudio_setup(&descriptor);

  auto max_samples_requested = saudio_expect() * saudio_channels() * 2;
  dn_array_init(&dn_audio.sample_buffer, max_samples_requested);
  dn_audio.sample_buffer.size = dn_audio.sample_buffer.capacity;


  // We use this array like a free list, so it is always full and we determine which elements are current
  // based on a flag in the element itself.
  dn_audio.instances.size = dn_audio.instances.capacity;
  dn_array_for(dn_audio.instances, active_sound) {
    active_sound->occupied = false;
  }


}

void dn_audio_add_samples(dn_audio_instance_t* active_sound, int samples_requested, int offset) {
  for (i32 i = 0; i < samples_requested; i++) {
    auto info = dn_audio.sounds[active_sound->info.index];
    auto index = active_sound->next_sample++;

    if (index == info->num_samples) {
      if (active_sound->loop) {
        active_sound->next_sample = 0;
        index = 0;
      }
      else if (dn_gen_arena_handle_valid(active_sound->next)) {
        active_sound->sample_buffer_offset = i;
        active_sound->samples_from_next = samples_requested - i;
        break;
      }
      else {
        dn_audio_stop_ex(active_sound);
        break;
      }
    }

    // Take the next sample from the sound and add it to the sample buffer
    auto sample = info->samples[index];
    sample = sample * active_sound->volume;
    sample = dn_low_pass_filter_apply(&active_sound->filter, sample);

    *dn_audio.sample_buffer[i + offset] += sample;
  }
  
}
  

void dn_audio_update(float* buffer, int frames_requested, int num_channels) {
  if (!frames_requested) return;

  // Cap the number of samples so we don't overwrite the buffer
  i32 samples_requested = frames_requested * num_channels;
  if (samples_requested > dn_audio.sample_buffer.capacity) {
    dn_log("requested too many audio samples: %d", samples_requested);
    samples_requested = dn_audio.sample_buffer.capacity;
  }

  // You must write zeros, or else whatever the last requested samples were will linger
  if (!dn_audio_is_any_playing()) {
    memset(buffer, 0, samples_requested * sizeof(float));
    return;
  }

  std::unique_lock lock(dn_audio_mutex);

  dn_array_for(dn_audio.instances, active_sound) {
    if (!active_sound->occupied) continue;
    if (active_sound->paused) continue;
    if (!dn_gen_arena_handle_valid(active_sound->info)) continue;
    
    auto info = dn_audio.sounds[active_sound->info.index];
    if (active_sound->info.generation != info->generation) {
      dn_audio_stop_ex(active_sound);
      continue;
    }

    
    dn_audio_add_samples(active_sound, samples_requested, 0);
  }

  bool chaining_sounds = true;
  while (chaining_sounds) {
    chaining_sounds = false;
    
    dn_array_for(dn_audio.instances, active_sound) {
      if (!active_sound->occupied) continue;
      if (active_sound->paused) continue;

      if (active_sound->samples_from_next) {
        chaining_sounds = true;
        
        auto next_sound = dn_audio_resolve(active_sound->next);
        if (!next_sound) continue;
        
        dn_audio_resume(active_sound->next);
        dn_audio_add_samples(next_sound, active_sound->samples_from_next, active_sound->sample_buffer_offset);
        dn_audio_stop_ex(active_sound);
      }
    }
  }

  lock.unlock();
  
  float envelope = 0.0f;
  float gain = 1.0f;
  for (int i = 0; i < samples_requested; i++) {
    auto sample = *dn_audio.sample_buffer[i];
    sample *= dn_audio.master_volume * dn_audio.master_volume_mod;

    auto abs_sample = std::abs(sample);
    if (dn_audio.compressor.enabled) {
      envelope = dn_math_lerp(envelope, abs_sample, dn_audio.compressor.attack_time);

      if (envelope > dn_audio.compressor.threshold) {
        auto decibel = 10 * std::log10(envelope / dn_audio.compressor.threshold);
        auto target_decibel = decibel / dn_audio.compressor.ratio;
        auto target_bel = target_decibel / 10;
        auto target_envelope = std::pow(10, target_bel) * dn_audio.compressor.threshold;
        gain = target_envelope / envelope;
      }
      else {
        gain = dn_math_lerp(gain, 1.0f, dn_audio.compressor.release_time);
      }
    }

    sample *= gain;
    sample = dn_low_pass_filter_apply(&dn_audio.filter, sample);
    sample = dn_math_clamp(sample, -1.f, 1.f);
    
    *dn_audio.sample_buffer[i] = sample;
  }

  memcpy(buffer, dn_audio.sample_buffer.data, samples_requested * sizeof(float));
  dn_array_fill(&dn_audio.sample_buffer, 0.f);
}

void dn_audio_shutdown() {
  saudio_shutdown();
}

bool dn_audio_is_any_playing() {
  std::unique_lock lock(dn_audio_mutex);
  
  dn_array_for(dn_audio.instances, active_sound) {
    if (active_sound->occupied) return true;
  }

  return false;
}

dn_audio_info_t* dn_audio_find_no_default(const char* name) {
  std::unique_lock lock(dn_audio_mutex);

  auto hash = dn_hash_cstr_dumb(name);
  dn_array_for(dn_audio.sounds, info) {
    if (info->hash == hash) return info;
  }

  return nullptr;
}

dn_audio_info_t* dn_audio_find(const char* name) {
  auto sound = dn_audio_find_no_default(name);
  if (!sound) sound = dn_audio_find_no_default("debug.wav");
  return sound;
}

dn_audio_instance_handle_t dn_audio_reserve() {
  std::unique_lock lock(dn_audio_mutex);
  
  for (u32 index = 0; index < dn_audio.instances.size; index++) {
    auto active_sound = dn_audio.instances[index];
    if (!active_sound->occupied) {
      active_sound->occupied = true;
      return { 
        .index = index, 
        .generation = active_sound->generation 
      };
    }
  }

  return dn_gen_arena_invalid_handle();
}

dn_audio_instance_t* dn_audio_resolve(dn_audio_instance_handle_t handle) {
  std::unique_lock lock(dn_audio_mutex);

  if (!dn_gen_arena_handle_valid(handle)) return nullptr;
  
  auto active_sound = dn_audio.instances[handle.index];
  if (handle.generation != active_sound->generation) return nullptr;

  return active_sound;
}

dn_audio_instance_handle_t dn_audio_play_sound_ex(dn_audio_info_t* sound, bool loop) { 
  std::unique_lock lock(dn_audio_mutex);
  
  auto handle = dn_audio_reserve();
  if (!dn_gen_arena_handle_valid(handle)) return handle;

  auto active_sound = dn_audio_resolve(handle);
  active_sound->info = { dn_array_indexof(&dn_audio.sounds, sound), sound->generation };
  active_sound->volume = 1.f;
  active_sound->next_sample = 0;
  active_sound->loop = loop;
  active_sound->paused = false;
  active_sound->next = dn_gen_arena_invalid_handle();
  active_sound->samples_from_next = 0;
  active_sound->sample_buffer_offset = 0;
  active_sound->filter = {
    .mode = DN_AUDIO_FILTER_MODE_BUTTERWORTH,
    .enabled = false,
    .a0 = 0.f, .a1 = 0.f, .a2 = 0.f, .b1 = 0.f, .b2 = 0.f,
    .input_history = { 0 },
    .output_history = { 0 },
  };
  dn_low_pass_filter_set_cutoff(&active_sound->filter, dn_audio_get_master_filter_cutoff());

  return handle;
}

void dn_audio_stop_ex(dn_audio_instance_t* active_sound) {
  std::unique_lock lock(dn_audio_mutex);  

  if (!active_sound) return;
  
  active_sound->occupied = false;
  active_sound->generation++;

  dn_audio_stop_ex(dn_audio_resolve(active_sound->next));
}

dn_audio_info_t* alloc_sound(const char* file_name) {
  auto sound = dn_audio_find_no_default(file_name);
  if (!sound) sound = dn_array_push(&dn_audio.sounds);
  sound->generation++;

  return sound;
}

void dn_audio_load_dir(const char* path) {
  dn_os_directory_entry_list_t entries = dn_os_scan_directory(dn_string_literal(path));
  for (u32 i = 0; i < entries.count; i++) {
    dn_os_directory_entry_t entry = entries.data[i];
    if (entry.attributes & DN_OS_FILE_ATTR_DIRECTORY) {
      // @string
      dn_audio.file_monitor->add_directory(dn_string_to_cstr_ex(entry.file_path, &dn_allocators.bump));
      dn_audio_load_dir(dn_string_to_cstr_ex(entry.file_path, &dn_allocators.bump));
    }
    else {
      // @string
      dn_audio_load(dn_string_to_cstr_ex(entry.file_path, &dn_allocators.bump), dn_string_to_cstr_ex(entry.file_name, &dn_allocators.bump));
    }
  }
}

void dn_audio_load(const char* file_path, const char* file_name) {
  std::unique_lock lock(dn_audio_mutex);

  auto sound = alloc_sound(file_name);
  strncpy(sound->name, file_name, DN_ASSET_NAME_LEN);
  sound->hash = dn_hash_cstr_dumb(sound->name);
  
  sound->samples = drwav_open_file_and_read_pcm_frames_f32(file_path, &sound->num_channels, &sound->sample_rate, &sound->num_frames, NULL);
  if (!sound->samples) {
    dn_log("failed to load sound file sound file: %s", file_path);
  }
  sound->num_samples = sound->num_frames * sound->num_channels;
};


//////////////////////
// LOW PASS FILTER  //
//////////////////////
void dn_low_pass_filter_set_mode(dn_low_pass_filter_t* filter, dn_audio_filter_mode_t mode) {
  filter->mode = mode;
  dn_low_pass_filter_set_cutoff(filter, filter->cutoff_frequency);
}

void dn_low_pass_filter_set_cutoff(dn_low_pass_filter_t* filter, float cutoff) {
  // The low pass filters are unstable at frequencies higher than the Nyquist frequency, so clamp. Add a little wiggle room
  // to make sure we're not close to it, because it sounds a little bad that high anyway.
  float nyquist = dn_audio.sample_frequency / 2.1;
  filter->cutoff_frequency = std::min(cutoff, nyquist);

  // Butterworth filter
  float omega = 2.0f * 3.14159 * filter->cutoff_frequency / dn_audio.sample_frequency;
  float cos_omega = cos(omega);
  float sin_omega = sin(omega);
  float alpha = sin_omega / sqrt(2.0f); // Butterworth filter (sqrt(2) damping factor)

  filter->a0 = (1.0f - cos_omega) / 2.0f;
  filter->a1 = 1.0f - cos_omega;
  filter->a2 = (1.0f - cos_omega) / 2.0f;
  filter->b1 = -2.0f * cos_omega;
  filter->b2 = 1.0f - alpha;

  float a0_inv = 1.0f / (1.0f + alpha);
  filter->a0 *= a0_inv;
  filter->a1 *= a0_inv;
  filter->a2 *= a0_inv;
  filter->b1 *= a0_inv;
  filter->b2 *= a0_inv;

  // Simple first order low pass filter
  filter->cutoff_alpha = 2.0f * 3.14159 * filter->cutoff_frequency / (dn_audio.sample_frequency + 2.0f * 3.14159 * filter->cutoff_frequency);

}

float dn_low_pass_filter_apply(dn_low_pass_filter_t* filter, float input) {
  if (!filter->enabled) return input;
  
  switch (filter->mode) {
    case DN_AUDIO_FILTER_MODE_BUTTERWORTH: {
      float output = 0;
      output += (filter->a0 * input) + (filter->a1 * filter->input_history[0]) + (filter->a2 * filter->input_history[1]);
      output -= (filter->b1 * filter->output_history[0]) + (filter->b2 * filter->output_history[1]);

      filter->input_history[1] = filter->input_history[0];
      filter->input_history[0] = input;

      filter->output_history[1] = filter->output_history[0];
      filter->output_history[0] = output;

      return output;
    } break;

    case DN_AUDIO_FILTER_MODE_FIRST_ORDER: {
      float output = 0;
      output += filter->cutoff_alpha * input;
      output += (1.0f - filter->cutoff_alpha) * filter->output_history[0];
    
      filter->output_history[0] = output;
    
      return output;
    } break;

    default: {
      DN_ASSERT(false);
      return input;
    }
  }
}


/////////////
// LUA API //
/////////////
dn_audio_instance_handle_t dn_audio_play_sound(const char* name) {
  auto info = dn_audio_find(name);
  return dn_audio_play_sound_ex(info, false);
}

dn_audio_instance_handle_t dn_audio_play_looped(const char* name) {
  auto info = dn_audio_find(name);
  return dn_audio_play_sound_ex(info, true);
}

void dn_audio_stop(dn_audio_instance_handle_t handle) {
  auto active_sound = dn_audio_resolve(handle);
  dn_audio_stop_ex(active_sound);
}

void dn_audio_stop_all() {
  dn_array_for(dn_audio.instances, active_sound) {
    dn_audio_stop_ex(active_sound);
  }
}

void dn_audio_set_volume(dn_audio_instance_handle_t handle, float volume) {
  auto active_sound = dn_audio_resolve(handle);
  if (!active_sound) return;

  std::unique_lock lock(dn_audio_mutex);

  active_sound->volume = dn_math_clamp(volume, 0.f, 1.f);
}

void dn_audio_set_filter_mode(dn_audio_instance_handle_t handle, dn_audio_filter_mode_t mode) {
  auto active_sound = dn_audio_resolve(handle);
  if (!active_sound) return;

  std::unique_lock lock(dn_audio_mutex);
  dn_low_pass_filter_set_mode(&active_sound->filter, mode);
}

void dn_audio_set_filter_cutoff(dn_audio_instance_handle_t handle, float cutoff) {
  auto active_sound = dn_audio_resolve(handle);
  if (!active_sound) return;

  std::unique_lock lock(dn_audio_mutex);
  dn_low_pass_filter_set_cutoff(&active_sound->filter, cutoff);
}

void dn_audio_set_filter_enabled(dn_audio_instance_handle_t handle, bool enabled) {
  auto active_sound = dn_audio_resolve(handle);
  if (!active_sound) return;

  std::unique_lock lock(dn_audio_mutex);
  active_sound->filter.enabled = enabled;
}

void dn_audio_set_compressor_threshold(float threshold) {
  std::unique_lock lock(dn_audio_mutex);
  dn_audio.compressor.threshold = threshold;
}

void dn_audio_set_compressor_ratio(float ratio) {
  std::unique_lock lock(dn_audio_mutex);
  dn_audio.compressor.ratio = ratio;
}

void dn_audio_set_compressor_attack(float attack) {
  std::unique_lock lock(dn_audio_mutex);
  dn_audio.compressor.attack_time = attack;
}

void dn_audio_set_compressor_release(float release) {
  std::unique_lock lock(dn_audio_mutex);
  dn_audio.compressor.release_time = release;
}

void dn_audio_set_sample_rate(float rate) {
  std::unique_lock lock(dn_audio_mutex);
  dn_audio.sample_frequency = rate;
}

void dn_audio_set_master_volume(float volume) {
  std::unique_lock lock(dn_audio_mutex);
  dn_audio.master_volume = volume;
}

void dn_audio_set_master_volume_mod(float volume_mod) {
  std::unique_lock lock(dn_audio_mutex);
  dn_audio.master_volume_mod = volume_mod;
}

void dn_audio_set_master_filter_cutoff(float frequency) {
  std::unique_lock lock(dn_audio_mutex);
  dn_low_pass_filter_set_cutoff(&dn_audio.filter, frequency);
}

void dn_audio_set_master_filter_cutoff_enabled(bool enabled) {
  std::unique_lock lock(dn_audio_mutex);
  dn_audio.filter.enabled = enabled;
}

void dn_audio_set_master_filter_mode(dn_audio_filter_mode_t mode) {
  std::unique_lock lock(dn_audio_mutex);
  dn_audio.filter.mode = mode;
}

float dn_audio_get_master_filter_cutoff() {
  return dn_audio.filter.cutoff_frequency;
}

float dn_audio_get_master_volume() {
  return dn_audio.master_volume;
}

float dn_audio_get_master_volume_mod() {
  return dn_audio.master_volume_mod;
}

bool dn_audio_is_playing(dn_audio_instance_handle_t handle) {
  return dn_audio_resolve(handle);
}

void dn_audio_pause(dn_audio_instance_handle_t handle) {
  auto sound = dn_audio_resolve(handle);
  if (!sound) return;
  
  sound->paused = true;
}

void dn_audio_resume(dn_audio_instance_handle_t handle) {
  auto sound = dn_audio_resolve(handle);
  if (!sound) return;
  
  sound->paused = false;
}

void dn_audio_queue(dn_audio_instance_handle_t current, dn_audio_instance_handle_t next) {
  auto next_sound = dn_audio_resolve(next);
  if (!dn_gen_arena_handle_valid(next)) return;
  
  auto current_sound = dn_audio_resolve(current);
  if (!current_sound) {
    next_sound->paused = false;
    return;
  };

  current_sound->next = next;
  dn_audio_pause(next);
}
#endif