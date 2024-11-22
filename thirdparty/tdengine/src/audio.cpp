using sound_iterator = std::function<void(const char*)>;

void init_audio() {
	auto on_file_event = [](FileMonitor* monitor, FileChange* event, void* userdata) {
		std::unique_lock lock(audio_mutex);
		
		load_audio_file(event->file_path, event->file_name);
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

				load_audio_file(file_path.c_str(), file_name.c_str());
			}
		}
	};

	auto audio_dir = resolve_named_path("audio");
	check_directory(audio_dir);

	// Initialize Sokol
	saudio_desc descriptor = { 0 };
	descriptor.num_channels = 2;
	descriptor.buffer_frames = 2048;
	descriptor.logger.func = slog_func;
	descriptor.stream_cb = update_audio;
	
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
	set_master_cutoff(low_pass.cutoff_frequency);
}

void add_samples(ActiveSound* active_sound, int samples_requested, int offset) {
	for (int32 i = 0; i < samples_requested; i++) {
		auto info = sound_infos[active_sound->info.index];
		auto index = active_sound->next_sample++;

		if (index == info->num_samples) {
			if (active_sound->loop) {
				active_sound->next_sample = 0;
				index = 0;
			}
			else if (active_sound->next) {
				active_sound->sample_buffer_offset = i;
				active_sound->samples_from_next = samples_requested - i;
				break;
			}
			else {
				stop_sound_ex(active_sound);
				break;
			}
		}

		// Take the next sample from the sound and add it to the sample buffer
		auto sample = info->samples[index];
		sample = sample * active_sound->volume;
		sample = active_sound->filter.apply(sample);

		*sample_buffer[i + offset] += sample;
	}
	
}
	

void update_audio(float* buffer, int frames_requested, int num_channels) {
	if (!frames_requested) return;

	// Cap the number of samples so we don't overwrite the buffer
	int32 samples_requested = frames_requested * num_channels;
	if (samples_requested > sample_buffer.capacity) {
		tdns_log.write("requested too many audio samples: %d", samples_requested);
		samples_requested = sample_buffer.capacity;
	}

	// You must write zeros, or else whatever the last requested samples were will linger
	if (!is_any_sound_active()) {
		memset(buffer, 0, samples_requested * sizeof(float));
		return;
	}

	std::unique_lock lock(audio_mutex);

	arr_for(active_sounds, active_sound) {
		if (!active_sound->occupied) continue;
		if (active_sound->paused) continue;
		if (!active_sound->info) continue;
		
		auto info = sound_infos[active_sound->info.index];
		if (active_sound->info.generation != info->generation) {
			stop_sound_ex(active_sound);
			continue;
		}

		
		add_samples(active_sound, samples_requested, 0);
	}

	bool chaining_sounds = true;
	while (chaining_sounds) {
		chaining_sounds = false;
		
		arr_for(active_sounds, active_sound) {
			if (!active_sound->occupied) continue;
			if (active_sound->paused) continue;

			if (active_sound->samples_from_next) {
				chaining_sounds = true;
				
				auto next_sound = find_active_sound(active_sound->next);
				if (!next_sound) continue;
				
				unpause_sound(active_sound->next);
				add_samples(next_sound, active_sound->samples_from_next, active_sound->sample_buffer_offset);
				stop_sound_ex(active_sound);
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
		sample = low_pass.apply(sample);
		sample = clamp(sample, -1.f, 1.f);
		
		*sample_buffer[i] = sample;
	}

	memcpy(buffer, sample_buffer.data, samples_requested * sizeof(float));
	arr_fill(&sample_buffer, 0.f);
}

void shutdown_audio() {
	saudio_shutdown();
}


ActiveSoundHandle::operator bool() {
	return index >= 0;
}

SoundInfoHandle::operator bool() {
	return index >= 0;
}

bool is_any_sound_active() {
	std::unique_lock lock(audio_mutex);
	
	arr_for(active_sounds, active_sound) {
		if (active_sound->occupied) return true;
	}

	return false;
}

SoundInfo* find_sound_no_default(const char* name) {
	std::unique_lock lock(audio_mutex);

	auto hash = hash_label(name);
	arr_for(sound_infos, info) {
		if (info->hash == hash) return info;
	}

	return nullptr;
}

SoundInfo* find_sound(const char* name) {
	auto sound = find_sound_no_default(name);
	if (!sound) sound = find_sound_no_default("debug.wav");
	return sound;
}

ActiveSoundHandle find_free_active_sound() {
	std::unique_lock lock(audio_mutex);
	
	for (int index = 0; index < active_sounds.size; index++) {
		auto active_sound = active_sounds[index];
		if (!active_sound->occupied) {
			active_sound->occupied = true;
			return { index, active_sound->generation };
		}
	}

	return { -1, -1 };
}

ActiveSound* find_active_sound(ActiveSoundHandle handle) {
	std::unique_lock lock(audio_mutex);

	if (!handle) return nullptr;
	
	auto active_sound = active_sounds[handle.index];
	if (handle.generation != active_sound->generation) return nullptr;

	return active_sound;
}

ActiveSoundHandle play_sound_ex(SoundInfo* sound, bool loop) { 
	std::unique_lock lock(audio_mutex);
	
	auto handle = find_free_active_sound();
	if (!handle) return handle;

	auto active_sound = find_active_sound(handle);
	active_sound->info = { arr_indexof(&sound_infos, sound), sound->generation };
	active_sound->volume = 1.f;
	active_sound->next_sample = 0;
	active_sound->loop = loop;
	active_sound->paused = false;
	active_sound->next = { -1, -1 };
	active_sound->samples_from_next = 0;
	active_sound->sample_buffer_offset = 0;
	active_sound->filter.enabled = false;
	active_sound->filter.set_cutoff(get_master_cutoff());
	active_sound->filter.butterworth = true;

	return handle;
}

void stop_sound_ex(ActiveSound* active_sound) {
	std::unique_lock lock(audio_mutex);	

	if (!active_sound) return;
	
	active_sound->occupied = false;
	active_sound->generation++;

	stop_sound_ex(find_active_sound(active_sound->next));
}

SoundInfo* alloc_sound(const char* file_name) {
	auto sound = find_sound_no_default(file_name);
	if (!sound) sound = arr_push(&sound_infos);
	sound->generation++;

	return sound;
}

void load_audio_file(const char* file_path, const char* file_name) {
	auto sound = alloc_sound(file_name);
	strncpy(sound->name, file_name, SoundInfo::name_len);
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
void LowPassFilter::set_cutoff(float cutoff) {
	// The low pass filters are unstable at frequencies higher than the Nyquist frequency, so clamp. Add a little wiggle room
	// to make sure we're not close to it, because it sounds a little bad that high anyway.
	float nyquist = sample_frequency / 2.1;
	cutoff_frequency = std::min(cutoff, nyquist);

	// Butterworth filter
    float omega = 2.0f * 3.14159 * cutoff_frequency / sample_frequency;
    float cos_omega = cos(omega);
    float sin_omega = sin(omega);
    float alpha = sin_omega / sqrt(2.0f); // Butterworth filter (sqrt(2) damping factor)

    a0 = (1.0f - cos_omega) / 2.0f;
    a1 = 1.0f - cos_omega;
    a2 = (1.0f - cos_omega) / 2.0f;
    b1 = -2.0f * cos_omega;
    b2 = 1.0f - alpha;

    float a0_inv = 1.0f / (1.0f + alpha);
    a0 *= a0_inv;
    a1 *= a0_inv;
    a2 *= a0_inv;
    b1 *= a0_inv;
    b2 *= a0_inv;

	// Simple first order low pass filter
	cutoff_alpha = 2.0f * 3.14159 * cutoff_frequency / (sample_frequency + 2.0f * 3.14159 * cutoff_frequency);

}

float LowPassFilter::apply(float input) {
	if (!enabled) return input;
	
	if (butterworth) {
		float output = 0;
		output += (a0 * input) + (a1 * input_history[0]) + (a2 * input_history[1]);
		output -= (b1 * output_history[0]) + (b2 * output_history[1]);

		input_history[1] = input_history[0];
		input_history[0] = input;

		output_history[1] = output_history[0];
		output_history[0] = output;

		return output;
	}
	else {
		float output = 0;
		output += cutoff_alpha * input;
		output += (1.0f - cutoff_alpha) * output_history[0];
	
		output_history[0] = output;
	
		return output;
	}
}


/////////////
// LUA API //
/////////////
ActiveSoundHandle play_sound(const char* name) {
	auto info = find_sound(name);
	return play_sound_ex(info, false);
}

ActiveSoundHandle play_sound_loop(const char* name) {
	auto info = find_sound(name);
	return play_sound_ex(info, true);
}

void stop_sound(ActiveSoundHandle handle) {
	auto active_sound = find_active_sound(handle);
	stop_sound_ex(active_sound);
}

void stop_all_sounds() {
	arr_for(active_sounds, active_sound) {
		stop_sound_ex(active_sound);
	}
}

void set_volume(ActiveSoundHandle handle, float volume) {
	auto active_sound = find_active_sound(handle);
	if (!active_sound) return;

	std::unique_lock lock(audio_mutex);

	active_sound->volume = clamp(volume, 0.f, 1.f);
}

void set_cutoff(ActiveSoundHandle handle, float cutoff) {
	auto active_sound = find_active_sound(handle);
	if (!active_sound) return;

	std::unique_lock lock(audio_mutex);

	active_sound->filter.enabled = true;;
	active_sound->filter.set_cutoff(cutoff);
}

void set_threshold(float threshold) {
	std::unique_lock lock(audio_mutex);
	threshold = threshold;
}

void set_ratio(float ratio) {
	std::unique_lock lock(audio_mutex);
	ratio = ratio;
}

void set_attack_time(float attack) {
	std::unique_lock lock(audio_mutex);
	attack_time = attack;
}

void set_release_time(float release) {
	std::unique_lock lock(audio_mutex);
	release_time = release;
}

void set_sample_rate(float rate) {
	std::unique_lock lock(audio_mutex);
	sample_frequency = rate;
}

void set_master_volume(float volume) {
	std::unique_lock lock(audio_mutex);
	master_volume = volume;
}

void set_master_volume_mod(float volume_mod) {
	std::unique_lock lock(audio_mutex);
	master_volume_mod = volume_mod;
}

void set_master_cutoff(float frequency) {
	std::unique_lock lock(audio_mutex);
	low_pass.set_cutoff(frequency);
}

void set_butterworth(bool butterworth) {
	std::unique_lock lock(audio_mutex);
	low_pass.butterworth = butterworth;
}

float get_master_cutoff() {
	return low_pass.cutoff_frequency;
}

float get_master_volume() {
	return master_volume;
}

float get_master_volume_mod() {
	return master_volume_mod;
}

bool is_sound_playing(ActiveSoundHandle handle) {
	return find_active_sound(handle);
}

void pause_sound(ActiveSoundHandle handle) {
	auto sound = find_active_sound(handle);
	if (!sound) return;
	
	sound->paused = true;
}

void unpause_sound(ActiveSoundHandle handle) {
	auto sound = find_active_sound(handle);
	if (!sound) return;
	
	sound->paused = false;
}

void play_sound_after(ActiveSoundHandle current, ActiveSoundHandle next) {
	auto next_sound = find_active_sound(next);
	if (!next) return;
	
	auto current_sound = find_active_sound(current);
	if (!current_sound) {
		next_sound->paused = false;
		return;
	};

	current_sound->next = next;
	pause_sound(next);
}
