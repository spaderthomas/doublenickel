#ifndef DN_AUDIO_H
#define DN_AUDIO_H
typedef dn_gen_arena_handle_t dn_audio_info_handle_t;

struct dn_audio_info_t {
	static constexpr int name_len = 64;
	char name [name_len];
	hash_t hash;
	u32 num_channels;
	u32 sample_rate;
	drwav_uint64 num_frames;
	u32 num_samples;
	float* samples;
	u32 generation = 0;

	double file_mod_time;
};

typedef enum {
	DN_AUDIO_FILTER_MODE_FIRST_ORDER = 0,
	DN_AUDIO_FILTER_MODE_BUTTERWORTH = 1,
} dn_audio_filter_mode_t;

typedef struct {
	dn_audio_filter_mode_t mode;
	bool enabled;
	float cutoff_frequency;

	// First order
	float cutoff_alpha;

	// Second order Butterworth filter
	float a0, a1, a2, b1, b2;
	float input_history [2];
	float output_history [2];
} dn_low_pass_filter_t;


std::recursive_mutex audio_mutex;
Array<float> sample_buffer;
FileMonitor* audio_monitor;

float threshold = 0.5f;
float ratio = 2.0f;
float attack_time = 0.95f;
float release_time = 1.f;
float sample_frequency = 44100;
float master_volume = 1.0f;
float master_volume_mod = 1.0f;
dn_low_pass_filter_t low_pass;


typedef dn_gen_arena_handle_t dn_audio_instance_handle_t;

typedef struct {
	dn_audio_info_handle_t info;
	dn_audio_instance_handle_t next;
	u32 next_sample;
	bool loop;
	float32 volume;
	dn_low_pass_filter_t filter;
	bool paused;
	i32 sample_buffer_offset;
	i32 samples_from_next;
	
	bool occupied;
	u32 generation;
} dn_audio_instance_t;

FM_LUA_EXPORT void                       dn_audio_set_compressor_threshold(float t);
FM_LUA_EXPORT void                       dn_audio_set_compressor_ratio(float v);
FM_LUA_EXPORT void                       dn_audio_set_compressor_attack(float v);
FM_LUA_EXPORT void                       dn_audio_set_compressor_release(float v);
FM_LUA_EXPORT void                       dn_audio_set_sample_rate(float v);
FM_LUA_EXPORT float                      dn_audio_get_master_volume();
FM_LUA_EXPORT void                       dn_audio_set_master_volume(float v);
FM_LUA_EXPORT float                      dn_audio_get_master_volume_mod();
FM_LUA_EXPORT void                       dn_audio_set_master_volume_mod(float v);
FM_LUA_EXPORT float                      dn_audio_get_master_filter_cutoff();
FM_LUA_EXPORT void                       dn_audio_set_master_filter_cutoff(float v);
FM_LUA_EXPORT void                       dn_audio_set_master_filter_cutoff_enabled(bool enabled);
FM_LUA_EXPORT void                       dn_audio_set_master_filter_mode(dn_audio_filter_mode_t mode);
FM_LUA_EXPORT void                       dn_audio_set_volume(dn_audio_instance_handle_t handle, float volume);
FM_LUA_EXPORT void                       dn_audio_set_filter_cutoff(dn_audio_instance_handle_t handle, float cutoff);
FM_LUA_EXPORT void                       dn_audio_set_filter_enabled(dn_audio_instance_handle_t handle, bool enabled);
FM_LUA_EXPORT dn_audio_instance_handle_t dn_audio_play_sound(const char* name);
FM_LUA_EXPORT dn_audio_instance_handle_t dn_audio_play_looped(const char* name);
FM_LUA_EXPORT void                       dn_audio_queue(dn_audio_instance_handle_t current, dn_audio_instance_handle_t next);
FM_LUA_EXPORT void                       dn_audio_stop(dn_audio_instance_handle_t handle);
FM_LUA_EXPORT void                       dn_audio_stop_all();
FM_LUA_EXPORT void                       dn_audio_pause(dn_audio_instance_handle_t handle);
FM_LUA_EXPORT void                       dn_audio_resume(dn_audio_instance_handle_t handle);
FM_LUA_EXPORT bool                       dn_audio_is_playing(dn_audio_instance_handle_t handle);
FM_LUA_EXPORT bool                       dn_audio_is_any_playing();
FM_LUA_EXPORT void                       dn_audio_load(const char* file_path, const char* file_name);
FM_LUA_EXPORT void                       dn_low_pass_filter_set_cutoff(dn_low_pass_filter_t* filter, float cutoff);
FM_LUA_EXPORT float                      dn_low_pass_filter_apply(dn_low_pass_filter_t* filter, float input);

// @dn: You should be able to specify a directory when you initialize the audio
void dn_audio_init();
void dn_audio_update(float* buffer, int frames_requested, int num_channels);
void dn_audio_shutdown();

dn_audio_info_t* dn_audio_find(const char* name);
dn_audio_info_t* dn_audio_find_no_default(const char* name);
dn_audio_instance_t* dn_audio_resolve(dn_audio_instance_handle_t handle);
dn_audio_instance_handle_t dn_audio_reserve();
dn_audio_instance_handle_t dn_audio_play_sound_ex(dn_audio_info_t* sound, bool loop);
void dn_audio_stop_ex(dn_audio_instance_t* active_sound);
#endif



#ifdef DN_AUDIO_IMPLEMENTATION
using sound_iterator = std::function<void(const char*)>;

void dn_audio_init() {
	auto on_file_event = [](FileMonitor* monitor, FileChange* event, void* userdata) {
		std::unique_lock lock(audio_mutex);
		
		dn_audio_load(event->file_path, event->file_name);
	};

	auto events = FileChangeEvent::Added | FileChangeEvent::Modified;
	audio_monitor = arr_push(&file_monitors);
	audio_monitor->init(on_file_event, events, nullptr);
	
	// Load all sounds from WAV files
	sound_iterator check_directory = [&](const char* directory) {
		for (directory_iterator it(directory); it != directory_iterator(); it++) {
			if (it->is_directory()) {
				auto subdirectory = it->path().string();
				normalize_path(subdirectory);

				audio_monitor->add_directory(subdirectory.c_str());
				
				check_directory(subdirectory.c_str());
			}
			else {
				auto file_name = it->path().filename().string();
				auto file_path = it->path().string();
				normalize_path(file_path);

				dn_audio_load(file_path.c_str(), file_name.c_str());
			}
		}
	};

	auto audio_dir = dn_paths_resolve("audio");
	check_directory(audio_dir);

	// Initialize Sokol
	saudio_desc descriptor = { 0 };
	descriptor.num_channels = 2;
	descriptor.buffer_frames = 2048;
	descriptor.logger.func = slog_func;
	descriptor.stream_cb = dn_audio_update;
	
	saudio_setup(&descriptor);

	// Set up a sample buffer based on how much data Sokol expects
	auto max_samples_requested = saudio_expect() * saudio_channels() * 2;
	arr_init(&sample_buffer, max_samples_requested);
	sample_buffer.size = sample_buffer.capacity;

	// We use this array like a free list, so it is always full and we determine which elements are current
	// based on a flag in the element itself.
	active_sounds.size = active_sounds.capacity;
	arr_for(active_sounds, active_sound) {
		active_sound->occupied = false;
	}

	low_pass.enabled = true;
	dn_audio_set_master_filter_cutoff(low_pass.cutoff_frequency);
}

void dn_audio_add_samples(dn_audio_instance_t* active_sound, int samples_requested, int offset) {
	for (int32 i = 0; i < samples_requested; i++) {
		auto info = sound_infos[active_sound->info.index];
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

		*sample_buffer[i + offset] += sample;
	}
	
}
	

void dn_audio_update(float* buffer, int frames_requested, int num_channels) {
	if (!frames_requested) return;

	// Cap the number of samples so we don't overwrite the buffer
	int32 samples_requested = frames_requested * num_channels;
	if (samples_requested > sample_buffer.capacity) {
		tdns_log.write("requested too many audio samples: %d", samples_requested);
		samples_requested = sample_buffer.capacity;
	}

	// You must write zeros, or else whatever the last requested samples were will linger
	if (!dn_audio_is_any_playing()) {
		memset(buffer, 0, samples_requested * sizeof(float));
		return;
	}

	std::unique_lock lock(audio_mutex);

	arr_for(active_sounds, active_sound) {
		if (!active_sound->occupied) continue;
		if (active_sound->paused) continue;
		if (!dn_gen_arena_handle_valid(active_sound->info)) continue;
		
		auto info = sound_infos[active_sound->info.index];
		if (active_sound->info.generation != info->generation) {
			dn_audio_stop_ex(active_sound);
			continue;
		}

		
		dn_audio_add_samples(active_sound, samples_requested, 0);
	}

	bool chaining_sounds = true;
	while (chaining_sounds) {
		chaining_sounds = false;
		
		arr_for(active_sounds, active_sound) {
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
		auto sample = *sample_buffer[i];
		sample *= master_volume * master_volume_mod;

		auto abs_sample = std::abs(sample);
		envelope = fm_lerp(envelope, abs_sample, attack_time);

		if (envelope > threshold) {
			auto decibel = 10 * std::log10(envelope / threshold);
			auto target_decibel = decibel / ratio;
			auto target_bel = target_decibel / 10;
			auto target_envelope = std::pow(10, target_bel) * threshold;
			gain = target_envelope / envelope;
		}
		else {
			gain = fm_lerp(gain, 1.0f, release_time);
		}

		sample *= gain;
		sample = dn_low_pass_filter_apply(&low_pass, sample);
		sample = clamp(sample, -1.f, 1.f);
		
		*sample_buffer[i] = sample;
	}

	memcpy(buffer, sample_buffer.data, samples_requested * sizeof(float));
	arr_fill(&sample_buffer, 0.f);
}

void dn_audio_shutdown() {
	saudio_shutdown();
}

bool dn_audio_is_any_playing() {
	std::unique_lock lock(audio_mutex);
	
	arr_for(active_sounds, active_sound) {
		if (active_sound->occupied) return true;
	}

	return false;
}

dn_audio_info_t* dn_audio_find_no_default(const char* name) {
	std::unique_lock lock(audio_mutex);

	auto hash = hash_label(name);
	arr_for(sound_infos, info) {
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
	std::unique_lock lock(audio_mutex);
	
	for (u32 index = 0; index < active_sounds.size; index++) {
		auto active_sound = active_sounds[index];
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
	std::unique_lock lock(audio_mutex);

	if (!dn_gen_arena_handle_valid(handle)) return nullptr;
	
	auto active_sound = active_sounds[handle.index];
	if (handle.generation != active_sound->generation) return nullptr;

	return active_sound;
}

dn_audio_instance_handle_t dn_audio_play_sound_ex(dn_audio_info_t* sound, bool loop) { 
	std::unique_lock lock(audio_mutex);
	
	auto handle = dn_audio_reserve();
	if (!dn_gen_arena_handle_valid(handle)) return handle;

	auto active_sound = dn_audio_resolve(handle);
	active_sound->info = { arr_indexof(&sound_infos, sound), sound->generation };
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
	std::unique_lock lock(audio_mutex);	

	if (!active_sound) return;
	
	active_sound->occupied = false;
	active_sound->generation++;

	dn_audio_stop_ex(dn_audio_resolve(active_sound->next));
}

dn_audio_info_t* alloc_sound(const char* file_name) {
	auto sound = dn_audio_find_no_default(file_name);
	if (!sound) sound = arr_push(&sound_infos);
	sound->generation++;

	return sound;
}

void dn_audio_load(const char* file_path, const char* file_name) {
	auto sound = alloc_sound(file_name);
	strncpy(sound->name, file_name, dn_audio_info_t::name_len);
	sound->hash = hash_label(sound->name);
	
	sound->samples = drwav_open_file_and_read_pcm_frames_f32(file_path, &sound->num_channels, &sound->sample_rate, &sound->num_frames, NULL);
	if (!sound->samples) {
		tdns_log.write("failed to load sound file sound file: %s", file_path);
	}
	sound->num_samples = sound->num_frames * sound->num_channels;
};


//////////////////////
// LOW PASS FILTER  //
//////////////////////
void dn_low_pass_filter_set_cutoff(dn_low_pass_filter_t* filter, float cutoff) {
	// The low pass filters are unstable at frequencies higher than the Nyquist frequency, so clamp. Add a little wiggle room
	// to make sure we're not close to it, because it sounds a little bad that high anyway.
	float nyquist = sample_frequency / 2.1;
	filter->cutoff_frequency = std::min(cutoff, nyquist);

	// Butterworth filter
	float omega = 2.0f * 3.14159 * filter->cutoff_frequency / sample_frequency;
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
	filter->cutoff_alpha = 2.0f * 3.14159 * filter->cutoff_frequency / (sample_frequency + 2.0f * 3.14159 * filter->cutoff_frequency);

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
			TD_ASSERT(false);
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
	arr_for(active_sounds, active_sound) {
		dn_audio_stop_ex(active_sound);
	}
}

void dn_audio_set_volume(dn_audio_instance_handle_t handle, float volume) {
	auto active_sound = dn_audio_resolve(handle);
	if (!active_sound) return;

	std::unique_lock lock(audio_mutex);

	active_sound->volume = clamp(volume, 0.f, 1.f);
}

void dn_audio_set_filter_cutoff(dn_audio_instance_handle_t handle, float cutoff) {
	auto active_sound = dn_audio_resolve(handle);
	if (!active_sound) return;

	std::unique_lock lock(audio_mutex);
	dn_low_pass_filter_set_cutoff(&active_sound->filter, cutoff);
}

void dn_audio_set_filter_enabled(dn_audio_instance_handle_t handle, bool enabled) {
	auto active_sound = dn_audio_resolve(handle);
	if (!active_sound) return;

	std::unique_lock lock(audio_mutex);
	active_sound->filter.enabled = enabled;
}

void dn_audio_set_compressor_threshold(float threshold) {
	std::unique_lock lock(audio_mutex);
	threshold = threshold;
}

void dn_audio_set_compressor_ratio(float ratio) {
	std::unique_lock lock(audio_mutex);
	ratio = ratio;
}

void dn_audio_set_compressor_attack(float attack) {
	std::unique_lock lock(audio_mutex);
	attack_time = attack;
}

void dn_audio_set_compressor_release(float release) {
	std::unique_lock lock(audio_mutex);
	release_time = release;
}

void dn_audio_set_sample_rate(float rate) {
	std::unique_lock lock(audio_mutex);
	sample_frequency = rate;
}

void dn_audio_set_master_volume(float volume) {
	std::unique_lock lock(audio_mutex);
	master_volume = volume;
}

void dn_audio_set_master_volume_mod(float volume_mod) {
	std::unique_lock lock(audio_mutex);
	master_volume_mod = volume_mod;
}

void dn_audio_set_master_filter_cutoff(float frequency) {
	std::unique_lock lock(audio_mutex);
	dn_low_pass_filter_set_cutoff(&low_pass, frequency);
}

void dn_audio_set_master_filter_cutoff_enabled(bool enabled) {
	std::unique_lock lock(audio_mutex);
	low_pass.enabled = enabled;
}

void dn_audio_set_master_filter_mode(dn_audio_filter_mode_t mode) {
	std::unique_lock lock(audio_mutex);
	low_pass.mode = mode;
}


float dn_audio_get_master_filter_cutoff() {
	return low_pass.cutoff_frequency;
}

float dn_audio_get_master_volume() {
	return master_volume;
}

float dn_audio_get_master_volume_mod() {
	return master_volume_mod;
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