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
