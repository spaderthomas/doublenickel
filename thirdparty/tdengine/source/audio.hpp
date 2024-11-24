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

struct LowPassFilter {
	dn_audio_filter_mode_t mode;
	bool enabled = false;
	float cutoff_frequency = 22000.f;

	// First order
	float cutoff_alpha = 0.1f;

	// Second order Butterworth filter
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


typedef dn_gen_arena_handle_t dn_audio_instance_handle_t;

typedef struct {
	dn_audio_info_handle_t info;
	dn_audio_instance_handle_t next;
	u32 next_sample;
	bool loop;
	float32 volume;
	LowPassFilter filter;
	bool paused;
	i32 sample_buffer_offset;
	i32 samples_from_next;
	
	bool occupied;
	u32 generation;
} dn_audio_instance_t;

FM_LUA_EXPORT void  dn_audio_set_compressor_threshold(float t);
FM_LUA_EXPORT void  dn_audio_set_compressor_ratio(float v);
FM_LUA_EXPORT void  dn_audio_set_compressor_attack(float v);
FM_LUA_EXPORT void  dn_audio_set_compressor_release(float v);
FM_LUA_EXPORT void  dn_audio_set_sample_rate(float v);
FM_LUA_EXPORT float dn_audio_get_master_volume();
FM_LUA_EXPORT void  dn_audio_set_master_volume(float v);
FM_LUA_EXPORT float dn_audio_get_master_volume_mod();
FM_LUA_EXPORT void  dn_audio_set_master_volume_mod(float v);
FM_LUA_EXPORT float dn_audio_get_master_filter_cutoff();
FM_LUA_EXPORT void  dn_audio_set_master_filter_cutoff(float v);
FM_LUA_EXPORT void  dn_audio_set_master_filter_cutoff_enabled(bool enabled);
FM_LUA_EXPORT void  dn_audio_set_master_filter_mode(dn_audio_filter_mode_t mode);
FM_LUA_EXPORT void  dn_audio_set_volume(dn_audio_instance_handle_t handle, float volume);
FM_LUA_EXPORT void  dn_audio_set_cutoff(dn_audio_instance_handle_t handle, float cutoff);

FM_LUA_EXPORT dn_audio_instance_handle_t play_sound(const char* name);
FM_LUA_EXPORT dn_audio_instance_handle_t play_sound_loop(const char* name);
FM_LUA_EXPORT void dn_audio_play_after(dn_audio_instance_handle_t current, dn_audio_instance_handle_t next);
FM_LUA_EXPORT void stop_sound(dn_audio_instance_handle_t handle);
FM_LUA_EXPORT void stop_all_sounds();
FM_LUA_EXPORT void pause_sound(dn_audio_instance_handle_t handle);
FM_LUA_EXPORT void unpause_sound(dn_audio_instance_handle_t handle);
FM_LUA_EXPORT bool is_sound_playing(dn_audio_instance_handle_t handle);

void init_audio();
void update_audio(float* buffer, int frames_requested, int num_channels);
void shutdown_audio();

dn_audio_info_t* find_sound(const char* name);
dn_audio_info_t* find_sound_no_default(const char* name);
dn_audio_instance_t* find_active_sound(dn_audio_instance_handle_t handle);
dn_audio_instance_handle_t find_free_active_sound();
dn_audio_instance_handle_t play_sound_ex(dn_audio_info_t* sound, bool loop);
void stop_sound_ex(dn_audio_instance_t* active_sound);
bool is_any_sound_active();
void load_audio_file(const char* file_path, const char* file_name);
