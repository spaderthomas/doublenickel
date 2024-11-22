struct SoundInfoHandle {
	int32 index;
	int32 generation;

	operator bool();
};

struct SoundInfo {
	static constexpr int name_len = 64;
	char name [name_len];
	hash_t hash;
	uint32 num_channels;
	uint32 sample_rate;
	drwav_uint64 num_frames;
	uint32 num_samples;
	float* samples;
	int generation = 0;

	double file_mod_time;
};


struct LowPassFilter {
	bool enabled = false;
	float cutoff_frequency = 22000.f;

	// First order
	float cutoff_alpha = 0.1f;

	// Second order Butterworth filter
	bool butterworth = true;
	float a0, a1, a2, b1, b2;
	float input_history [2] = {0};
	float output_history [2] = {0};

	void set_cutoff(float cutoff);
	float apply(float input);
};

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
LowPassFilter low_pass;


struct ActiveSoundHandle {
	int32 index;
	int32 generation;

	operator bool();
};

struct ActiveSound {
	SoundInfoHandle info;
	uint32 next_sample;
	bool loop;
	float32 volume;
	LowPassFilter filter;
	bool paused;
	ActiveSoundHandle next;
	int32 sample_buffer_offset;
	int32 samples_from_next;
	
	bool occupied;
	int32 generation;
};

FM_LUA_EXPORT void set_threshold(float t);
FM_LUA_EXPORT void set_ratio(float v);
FM_LUA_EXPORT void set_attack_time(float v);
FM_LUA_EXPORT void set_release_time(float v);
FM_LUA_EXPORT void set_sample_rate(float v);
FM_LUA_EXPORT void set_master_volume(float v);
FM_LUA_EXPORT void set_master_volume_mod(float v);
FM_LUA_EXPORT void set_master_cutoff(float v);
FM_LUA_EXPORT void set_butterworth(bool v);
FM_LUA_EXPORT void set_volume(ActiveSoundHandle handle, float volume);
FM_LUA_EXPORT void set_cutoff(ActiveSoundHandle handle, float cutoff);
FM_LUA_EXPORT float get_master_cutoff();
FM_LUA_EXPORT float get_master_volume();
FM_LUA_EXPORT float get_master_volume_mod();

FM_LUA_EXPORT ActiveSoundHandle play_sound(const char* name);
FM_LUA_EXPORT ActiveSoundHandle play_sound_loop(const char* name);
FM_LUA_EXPORT void play_sound_after(ActiveSoundHandle current, ActiveSoundHandle next);
FM_LUA_EXPORT void stop_sound(ActiveSoundHandle handle);
FM_LUA_EXPORT void stop_all_sounds();
FM_LUA_EXPORT void pause_sound(ActiveSoundHandle handle);
FM_LUA_EXPORT void unpause_sound(ActiveSoundHandle handle);
FM_LUA_EXPORT bool is_sound_playing(ActiveSoundHandle handle);

void init_audio();
void update_audio(float* buffer, int frames_requested, int num_channels);
void shutdown_audio();

SoundInfo* find_sound(const char* name);
SoundInfo* find_sound_no_default(const char* name);
ActiveSound* find_active_sound(ActiveSoundHandle handle);
ActiveSoundHandle find_free_active_sound();
ActiveSoundHandle play_sound_ex(SoundInfo* sound, bool loop);
void stop_sound_ex(ActiveSound* active_sound);
bool is_any_sound_active();
void load_audio_file(const char* file_path, const char* file_name);
