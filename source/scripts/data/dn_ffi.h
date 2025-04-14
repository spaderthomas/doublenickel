  typedef enum gs_hash_table_entry_state
  {
      GS_HASH_TABLE_ENTRY_INACTIVE = 0x00,
      GS_HASH_TABLE_ENTRY_ACTIVE = 0x01
  } gs_hash_table_entry_state;
  
  
typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;
typedef u64 dn_hash_t;
typedef char dn_asset_name_t [64];
  
  typedef void* dn_win32_handle_t;
  typedef u32 dn_win32_dword_t;
  typedef void* dn_win32_find_data_t;
  typedef void* dn_win32_critical_section_t;
  
  
  
  
  typedef union {
    struct {
      float x;
      float y;
    };
    struct {
      float r;
      float g;
    };
  } dn_vector2_t;
  typedef union {
    struct {
      float x;
      float y;
      float z;
    };
    struct {
      float r;
      float g;
      float b;
    };
  } dn_vector3_t;
  typedef union {
    struct {
      float x;
      float y;
      float z;
      float w;
    };
    struct {
      float r;
      float g;
      float b;
      float a;
    };
  } dn_vector4_t;
  typedef struct { dn_vector4_t entries [4]; } dn_matrix4_t;
  typedef struct { dn_vector3_t entries [3]; } dn_matrix3_t;
  typedef struct { dn_vector2_t entries [2]; } dn_matrix2_t;
typedef struct {
  s32 x;
  s32 y;
} dn_vector2i_t;
 f32          dn_math_step(f32 edge, f32 x);
 f32          dn_math_lerp(f32 a, f32 b, f32 t);
 dn_vector4_t dn_math_lerp4(dn_vector4_t a, dn_vector4_t b, f32 t);
 dn_vector4_t dn_math_abs4(dn_vector4_t v);
 dn_vector4_t dn_math_fract4(dn_vector4_t v);
 f32          dn_math_clamp(f32 value, f32 lower, f32 upper);
 dn_vector4_t dn_math_clamp4(dn_vector4_t v, f32 vmin, f32 vmax);
 u32          dn_math_ceilf(f32 f);
 u32          dn_math_ceil_divide(u32 a, u32 b);
 f32          dn_random_f32_slow(f32 vmin, f32 vmax);
 s32          dn_random_i32(s32 vmin, s32 vmax);
typedef struct {
  struct {
    u32 perlin;
    u32 chaoric;
  } textures;
} dn_noise_t;
dn_noise_t dn_noise;
 f64 dn_noise_perlin(f64 x, f64 y);
 f64 dn_noise_perlin_scaled(f64 x, f64 y, f64 vmin, f64 vmax);
 f64 dn_noise_chaotic(f64 x, f64 y);
 f64 dn_noise_chaotic_scaled(f64 x, f64 y, f64 vmin, f64 vmax);
typedef dn_vector4_t dn_color_t;
typedef struct {
  dn_color_t indian_red;
  dn_color_t tyrian_purple;
  dn_color_t cardinal;
  dn_color_t celadon;
  dn_color_t spring_green;
  dn_color_t mindaro;
  dn_color_t light_green;
  dn_color_t zomp;
  dn_color_t midnight_green;
  dn_color_t prussian_blue;
  dn_color_t orange;
  dn_color_t sunglow;
  dn_color_t selective_yellow;
  dn_color_t gunmetal;
  dn_color_t paynes_gray;
  dn_color_t cadet_gray;
  dn_color_t charcoal;
  dn_color_t cool_gray;
  dn_color_t cream;
  dn_color_t misty_rose;
  dn_color_t taupe;
  dn_color_t dark_green;
  dn_color_t rich_black;
  dn_color_t white;
} dn_colors_t;
dn_color_t dn_color_rgb_to_hsv(dn_color_t color);
dn_color_t dn_color_hsv_to_rgb(dn_color_t color);
typedef enum {
  DN_ALLOCATOR_MODE_ALLOC,
  DN_ALLOCATOR_MODE_FREE,
  DN_ALLOCATOR_MODE_RESIZE,
} dn_allocator_mode_t;
struct dn_allocator_t;
typedef void*(*dn_alloc_fn_t)(struct dn_allocator_t* allocator, dn_allocator_mode_t mode, u32 size, void* previous_allocation);
typedef struct dn_allocator_t {
  dn_alloc_fn_t on_alloc;
} dn_allocator_t;
typedef struct {
  dn_allocator_t allocator;
  u8* buffer;
  u32 capacity;
  u32 bytes_used;
  struct { struct { u32 key; u32 val; gs_hash_table_entry_state state; }* data; u32 tmp_key; u32 tmp_val; size_t stride; size_t klpvl; size_t tmp_idx; }* allocations;
} dn_bump_allocator_t;
typedef struct {
  dn_allocator_t allocator;
} dn_standard_allocator_t;
typedef struct {
  dn_standard_allocator_t standard;
  dn_bump_allocator_t bump;
  struct { struct { dn_hash_t key; dn_allocator_t* val; gs_hash_table_entry_state state; }* data; dn_hash_t tmp_key; dn_allocator_t* tmp_val; size_t stride; size_t klpvl; size_t tmp_idx; }* allocators;
} dn_allocators_t;
dn_allocators_t dn_allocators;
 void             dn_allocator_add(const char* name, dn_allocator_t* allocator);
 dn_allocator_t*  dn_allocator_find(const char* name);
 void*            dn_allocator_alloc(dn_allocator_t* allocator, u32 size);
 void*            dn_allocator_realloc(dn_allocator_t* allocator, void* memory, u32 size);
 void             dn_allocator_free(dn_allocator_t* allocator, void* buffer);
typedef struct {
  u32 len;
  u8* data;
} dn_string_t;
typedef char* dn_tstring_t;
typedef char dn_path_t [256];
typedef struct {
  u8* data;
  u32 count;
  u32 capacity;
} dn_string_builder_buffer_t;
typedef struct {
  dn_string_builder_buffer_t buffer;
  dn_allocator_t* allocator;
} dn_string_builder_t;
 void        dn_string_builder_grow(dn_string_builder_t* builder, u32 requested_capacity);
 void        dn_string_builder_append(dn_string_builder_t* builder, dn_string_t str);
 void        dn_string_builder_append_cstr(dn_string_builder_t* builder, const char* str);
 void        dn_string_builder_append_fmt(dn_string_builder_t* builder, dn_string_t fmt, ...);
 dn_string_t dn_string_builder_write(dn_string_builder_t* builder);
 char*       dn_string_builder_write_cstr(dn_string_builder_t* builder);
 void        dn_cstr_copy_to(const char* str, char* buffer, u32 buffer_length);
 void        dn_cstr_copy_to_n(const char* str, u32 length, char* buffer, u32 buffer_length);
 bool        dn_cstr_equal(const char* a, const char* b);
 u32         dn_cstr_len(const char* str);
 char*       dn_string_to_cstr(dn_string_t str);
 char*       dn_string_to_double_null_terminated(dn_string_t str);
 char*       dn_string_to_cstr_ex(dn_string_t str, dn_allocator_t* allocator);
 dn_string_t dn_string_copy(dn_string_t str, dn_allocator_t* allocator);
 dn_string_t dn_string_copy_cstr_n(const char* str, u32 length, dn_allocator_t* allocator);
 dn_string_t dn_string_copy_cstr(const char* str, dn_allocator_t* allocator);
 void        dn_string_copy_to_str(dn_string_t str, dn_string_t* dest, u32 capacity);
 void        dn_string_copy_to(dn_string_t str, u8* buffer, u32 capacity);
 dn_string_t dn_string_alloc(u32 capacity, dn_allocator_t* allocator);
 bool        dn_string_equal(dn_string_t a, dn_string_t b);
 bool        dn_string_equal_cstr(dn_string_t a, const char* b);
 s32         dn_string_sort_kernel_alphabetical(const void* a, const void* b);
 s32         dn_string_compare_alphabetical(dn_string_t a, dn_string_t b);
 bool        dn_string_valid(dn_string_t str);
typedef enum {
  DN_OS_FILE_ATTR_NONE = 0,
  DN_OS_FILE_ATTR_REGULAR_FILE = 1,
  DN_OS_FILE_ATTR_DIRECTORY = 2,
} dn_os_file_attr_t;
typedef struct {
  s32 year;
  s32 month;
  s32 day;
  s32 hour;
  s32 minute;
  s32 second;
  s32 millisecond;
} dn_os_date_time_t;
typedef struct {
  dn_string_t file_path;
  dn_string_t file_name;
  dn_os_file_attr_t attributes;
} dn_os_directory_entry_t;
typedef struct {
  dn_os_directory_entry_t* data;
  u32 count;
} dn_os_directory_entry_list_t;
 bool                         dn_os_does_path_exist(dn_string_t path);
 bool                         dn_os_is_regular_file(dn_string_t path);
 bool                         dn_os_is_directory(dn_string_t path);
 void                         dn_os_create_directory(dn_string_t path);
 void                         dn_os_remove_directory(dn_string_t path);
 void                         dn_os_create_file(dn_string_t path);
 void                         dn_os_remove_file(dn_string_t path);
 dn_os_directory_entry_list_t dn_os_scan_directory(dn_string_t path);
 dn_os_directory_entry_list_t dn_os_scan_directory_recursive(dn_string_t path);
 dn_os_date_time_t            dn_os_get_date_time();
 f64                          dn_os_file_mod_time(dn_string_t path);
 void                         dn_os_normalize_path(dn_string_t path);
 dn_string_t                  dn_os_parent_path(dn_string_t path, dn_allocator_t* allocator);
 dn_string_t                  dn_os_path_extension(dn_string_t path);
 void                         dn_os_memory_copy(const void* source, void* dest, u32 num_bytes);
 bool                         dn_os_is_memory_equal(const void* a, const void* b, size_t len);
 void                         dn_os_fill_memory(void* buffer, u32 buffer_size, void* fill, u32 fill_size);
 void                         dn_os_fill_memory_u8(void* buffer, u32 buffer_size, u8 fill);
 void                         dn_os_zero_memory(void* buffer, u32 buffer_size);
 void                         dn_os_sleep_ms(f64 ms);
 u64                          dn_os_time_since_epoch();
typedef enum {
  DN_MUTEX_ATTR_NONE = 0,
  DN_MUTEX_ATTR_RECURSIVE = 1
} dn_mutex_attr_t;
typedef struct {
  dn_mutex_attr_t attrs;
  bool initialized;
  dn_win32_critical_section_t handle;
} dn_mutex_t;
void dn_mutex_init(dn_mutex_t* mutex);
void dn_mutex_lock(dn_mutex_t* mutex);
void dn_mutex_unlock(dn_mutex_t* mutex);
typedef void*(*dn_thread_fn_t)(void*);
typedef struct {
  dn_win32_handle_t handle;
  dn_win32_dword_t id;
  dn_thread_fn_t fn;
} dn_thread_t;
void dn_thread_init(dn_thread_t* thread, dn_thread_fn_t fn, void* argument);
void dn_thread_join(dn_thread_t* thread);
typedef struct {
  s32 dummy;  
} dn_condition_variable_t;
dn_hash_t dn_hash_siphash_bytes(const void *p, size_t len, size_t seed);
dn_hash_t dn_hash_bytes(const void *p, size_t len);
dn_hash_t dn_hash_bytes_ex(const void *p, size_t len, size_t seed);
dn_hash_t dn_combine_hashes(dn_hash_t a, dn_hash_t b);
dn_hash_t dn_hash_cstr_dumb(const char* str);
dn_hash_t dn_hash_str_dumb(dn_string_t str);
dn_hash_t dn_hash_string(dn_string_t str);
void      dn_hash_encode_hex(char* destination, const char* data, size_t len);
void      dn_hash_encode_base64(char* destination, const char* source, size_t len);
typedef struct {
  u8* data;
  u32 size;
  u32 capacity;
  u32 element_size;
  dn_allocator_t* allocator;
} dn_fixed_array_t;
 void dn_fixed_array_init(dn_fixed_array_t* fixed_array, u32 capacity, u32 element_size, dn_allocator_t* allocator);
 u8*  dn_fixed_array_push(dn_fixed_array_t* fixed_array, void* data, u32 count);
 u8*  dn_fixed_array_reserve(dn_fixed_array_t* fixed_array, u32 count);
 void dn_fixed_array_clear(dn_fixed_array_t* fixed_array);
 u32  dn_fixed_array_byte_size(dn_fixed_array_t* fixed_array);
 u8*  dn_fixed_array_at(dn_fixed_array_t* fixed_array, u32 index);
typedef struct {
  u8* data;
  u32 size;
  u32 capacity;
  u32 element_size;
  dn_allocator_t* allocator;
} dn_dynamic_array_t;
 void dn_dynamic_array_init(dn_dynamic_array_t* dynamic_array, u32 element_size, dn_allocator_t* allocator);
 u8*  dn_dynamic_array_push(dn_dynamic_array_t* dynamic_array, void* data);
 u8*  dn_dynamic_array_push_n(dn_dynamic_array_t* dynamic_array, void* data, u32 count);
 u8*  dn_dynamic_array_reserve(dn_dynamic_array_t* dynamic_array, u32 count);
 void dn_dynamic_array_clear(dn_dynamic_array_t* dynamic_array);
 u32  dn_dynamic_array_byte_size(dn_dynamic_array_t* dynamic_array);
 u8*  dn_dynamic_array_at(dn_dynamic_array_t* dynamic_array, u32 index);
 void dn_dynamic_array_grow(dn_dynamic_array_t* dynamic_array, u32 capacity);
typedef struct {
  u32 index;
  u32 generation;
} dn_pool_handle_t;
typedef struct {
  s32 next_free;
  u32 generation;
  bool occupied;
} dn_pool_slot_t;
typedef struct {
  u8* data;
  dn_pool_slot_t* slots;
  u32 element_size;
  u32 capacity;
  s32 free_list;
} dn_pool_t;
typedef struct {
  u32 index;
  dn_pool_t* pool;
} dn_pool_iterator_t;
 void               dn_pool_init(dn_pool_t* pool, u32 capacity, u32 element_size);
 dn_pool_handle_t   dn_pool_insert(dn_pool_t* pool, void* value);
 dn_pool_handle_t   dn_pool_reserve(dn_pool_t* pool);
 void               dn_pool_remove(dn_pool_t* pool, dn_pool_handle_t handle);
 bool               dn_pool_contains(dn_pool_t* pool, dn_pool_handle_t handle);
 void               dn_pool_clear(dn_pool_t* pool);
 dn_pool_handle_t   dn_pool_invalid_handle();
 bool               dn_pool_is_handle_valid(dn_pool_handle_t handle);
 bool               dn_pool_slot_has_next_free(dn_pool_slot_t* slot);
 bool               dn_pool_slot_is_match(dn_pool_slot_t* slot, dn_pool_handle_t handle);
 dn_pool_iterator_t dn_pool_iterator_init(dn_pool_t* pool);
 void               dn_pool_iterator_next(dn_pool_iterator_t* it);
 bool               dn_pool_iterator_done(dn_pool_iterator_t* it);
typedef struct {
  u8* data;
  u32 element_size;
  u32 head;
  u32 size;
  u32 capacity;
} dn_ring_buffer_t;
typedef struct {
  u32 index;
  bool reverse;
  dn_ring_buffer_t* buffer;
} dn_ring_buffer_iterator_t;
 void*                     dn_ring_buffer_at(dn_ring_buffer_t* buffer, u32 index);
 void                      dn_ring_buffer_init(dn_ring_buffer_t* buffer, u32 capacity, u32 element_size);
 void*                     dn_ring_buffer_back(dn_ring_buffer_t* buffer);
 void*                     dn_ring_buffer_push(dn_ring_buffer_t* buffer, void* data);
 void*                     dn_ring_buffer_push_zero(dn_ring_buffer_t* buffer);
 void*                     dn_ring_buffer_push_overwrite(dn_ring_buffer_t* buffer, void* data);
 void*                     dn_ring_buffer_push_overwrite_zero(dn_ring_buffer_t* buffer);
 void*                     dn_ring_buffer_pop(dn_ring_buffer_t* buffer);
 u32                       dn_ring_buffer_bytes(dn_ring_buffer_t* buffer);
 void                      dn_ring_buffer_clear(dn_ring_buffer_t* buffer);
 bool                      dn_ring_buffer_is_full(dn_ring_buffer_t* buffer);
 bool                      dn_ring_buffer_is_empty(dn_ring_buffer_t* buffer);
 void*                     dn_ring_buffer_iter_deref(dn_ring_buffer_iterator_t* it);
 void                      dn_ring_buffer_iter_next(dn_ring_buffer_iterator_t* it);
 void                      dn_ring_buffer_iter_prev(dn_ring_buffer_iterator_t* it);
 bool                      dn_ring_buffer_iter_done(dn_ring_buffer_iterator_t* it);
 dn_ring_buffer_iterator_t dn_ring_buffer_iter(dn_ring_buffer_t* buffer);
 dn_ring_buffer_iterator_t dn_ring_buffer_riter(dn_ring_buffer_t* buffer);
typedef struct {
  dn_ring_buffer_t queue;
  f64 time_begin;
} dn_time_metric_t;
typedef struct {
  struct { struct { dn_hash_t key; dn_time_metric_t val; gs_hash_table_entry_state state; }* data; dn_hash_t tmp_key; dn_time_metric_t tmp_val; size_t stride; size_t klpvl; size_t tmp_idx; }* metrics;
} dn_time_metrics_t;
dn_time_metrics_t dn_time_metrics;
 void              dn_time_metric_init(dn_time_metric_t* metric);
 void              dn_time_metric_begin_ex(dn_time_metric_t* metric);
 void              dn_time_metric_end_ex(dn_time_metric_t* metric);
 f64               dn_time_metric_average(dn_time_metric_t* metric);
 f64               dn_time_metric_last(dn_time_metric_t* metric);
 f64               dn_time_metric_largest(dn_time_metric_t* metric);
 f64               dn_time_metric_smallest(dn_time_metric_t* metric);
 void              dn_time_metric_busy_wait(dn_time_metric_t* metric, f64 target);
 void              dn_time_metric_sleep_wait(dn_time_metric_t* metric, f64 target);
 void              dn_time_metric_begin(dn_string_t name);
 void              dn_time_metric_end(dn_string_t name);
 dn_time_metric_t* dn_time_metrics_find(dn_string_t name);
 void              dn_time_metrics_add(dn_string_t name);
typedef enum {
  DN_LOG_FLAG_CONSOLE = 1,
  DN_LOG_FLAG_FILE = 2,
  DN_LOG_FLAG_DEFAULT = 3,
} dn_log_flags_t;
typedef struct {
  char message_buffer [4096];
  char preamble_buffer [512];
} dn_log_t;
dn_log_t dn_logger;
 void dn_log(const char* fmt, ...);
 void dn_log_flags(dn_log_flags_t flags, const char* fmt, ...);
 void dn_log_builder(dn_string_builder_t builder);
 void dn_log_str(dn_string_t message);
typedef struct {
  dn_string_t name;
  dn_string_t path;
} dn_named_path_t;
typedef struct {
  dn_named_path_t* data;
  u32 size;
} dn_named_path_result_t;
typedef struct {
  dn_string_t engine;
  dn_string_t install;
  dn_string_t write;
} dn_path_config_t;
typedef struct {
  struct { struct { dn_hash_t key; dn_string_t val; gs_hash_table_entry_state state; }* data; dn_hash_t tmp_key; dn_string_t tmp_val; size_t stride; size_t klpvl; size_t tmp_idx; }* entries;
} dn_paths_t;
dn_paths_t dn_paths;
 dn_named_path_result_t dn_paths_find_all();
 void                   dn_paths_add_install_subpath(dn_string_t name, dn_string_t relative_path);
 void                   dn_paths_add_write_subpath(dn_string_t name, dn_string_t relative_path);
 void                   dn_paths_add_engine_subpath(dn_string_t name, dn_string_t relative_path);
 void                   dn_paths_add_app_subpath(dn_string_t name, dn_string_t relative_path);
 void                   dn_paths_add_subpath(dn_string_t name, dn_string_t parent_name, dn_string_t relative_path);
 dn_string_t            dn_paths_resolve(dn_string_t name);
 dn_string_t            dn_paths_resolve_format(dn_string_t name, dn_string_t file_name);
 dn_string_t            dn_paths_resolve_cstr(const char* name);
 const char*            dn_paths_resolve_bootstrap(const char* name);
 dn_string_t            dn_paths_resolve_ex(dn_string_t name, dn_allocator_t* allocator);
 dn_string_t            dn_paths_resolve_format_cstr(const char* name, const char* file_name);
 dn_string_t            dn_paths_resolve_format_ex(dn_string_t name, dn_string_t file_name, dn_allocator_t* allocator);
 dn_string_t            dn_paths_strip(dn_string_t name, dn_string_t absolute_path);
 void                   dn_paths_add_ex(dn_string_t name, dn_string_t absolute_path);
typedef enum {
  DN_AUDIO_FILTER_MODE_FIRST_ORDER = 0,
  DN_AUDIO_FILTER_MODE_BUTTERWORTH = 1,
} dn_audio_filter_mode_t;
typedef struct {
  dn_audio_filter_mode_t mode;
  bool enabled;
  f32 cutoff_frequency;
  f32 cutoff_alpha;
  f32 a0, a1, a2, b1, b2;
  f32 input_history [2];
  f32 output_history [2];
} dn_low_pass_filter_t;
typedef struct {
  f32 threshold;
  f32 ratio;
  f32 attack_time;
  f32 release_time;
  bool enabled;
} dn_compressor_t;
typedef dn_pool_handle_t dn_audio_info_handle_t;
typedef struct {
  dn_asset_name_t name;
  dn_hash_t hash;
  u32 num_channels;
  u32 sample_rate;
  u64 num_frames;
  u32 num_samples;
  f32* samples;
  u32 generation;
  double file_mod_time;
} dn_audio_info_t;
typedef dn_pool_handle_t dn_audio_instance_handle_t;
typedef struct {
  dn_audio_info_handle_t info;
  dn_audio_instance_handle_t next;
  u32 next_sample;
  bool loop;
  f32 volume;
  dn_low_pass_filter_t filter;
  bool paused;
  s32 sample_buffer_offset;
  s32 samples_from_next;
  
  bool occupied;
  u32 generation;
} dn_audio_instance_t;
typedef struct {
  dn_path_t* dirs;
  u32 num_dirs;
  dn_compressor_t compressor;
  dn_low_pass_filter_t filter;
  f32 sample_frequency;
  f32 master_volume;
  f32 master_volume_mod;
} dn_audio_config_t;
typedef struct {
  dn_mutex_t mutex;
  dn_compressor_t compressor;
  dn_low_pass_filter_t filter;
  f32 sample_frequency;
  f32 master_volume;
  f32 master_volume_mod;
  
  dn_fixed_array_t sample_buffer;
  dn_pool_t sounds;
  dn_pool_t instances;
} dn_audio_t;
dn_audio_t dn_audio;
 void                       dn_audio_set_compressor_threshold(f32 t);
 void                       dn_audio_set_compressor_ratio(f32 v);
 void                       dn_audio_set_compressor_attack(f32 v);
 void                       dn_audio_set_compressor_release(f32 v);
 void                       dn_audio_set_sample_rate(f32 v);
 f32                        dn_audio_get_master_volume();
 void                       dn_audio_set_master_volume(f32 v);
 f32                        dn_audio_get_master_volume_mod();
 void                       dn_audio_set_master_volume_mod(f32 v);
 f32                        dn_audio_get_master_filter_cutoff();
 void                       dn_audio_set_master_filter_cutoff(f32 v);
 void                       dn_audio_set_master_filter_cutoff_enabled(bool enabled);
 void                       dn_audio_set_master_filter_mode(dn_audio_filter_mode_t mode);
 void                       dn_audio_set_volume(dn_audio_instance_handle_t handle, f32 volume);
 void                       dn_audio_set_filter_cutoff(dn_audio_instance_handle_t handle, f32 cutoff);
 void                       dn_audio_set_filter_mode(dn_audio_instance_handle_t handle, dn_audio_filter_mode_t mode);
 void                       dn_audio_set_filter_enabled(dn_audio_instance_handle_t handle, bool enabled);
 dn_audio_instance_handle_t dn_audio_play_sound(const char* name);
 dn_audio_instance_handle_t dn_audio_play_looped(const char* name);
 void                       dn_audio_queue(dn_audio_instance_handle_t current, dn_audio_instance_handle_t next);
 void                       dn_audio_stop(dn_audio_instance_handle_t handle);
 void                       dn_audio_stop_all();
 void                       dn_audio_pause(dn_audio_instance_handle_t handle);
 void                       dn_audio_resume(dn_audio_instance_handle_t handle);
 bool                       dn_audio_is_playing(dn_audio_instance_handle_t handle);
 bool                       dn_audio_is_any_playing();
 void                       dn_audio_load(const char* file_path, const char* file_name);
 void                       dn_audio_load_dir(const char* path);
 void                       dn_low_pass_filter_set_mode(dn_low_pass_filter_t* filter, dn_audio_filter_mode_t mode);
 void                       dn_low_pass_filter_set_cutoff(dn_low_pass_filter_t* filter, f32 cutoff);
 f32                        dn_low_pass_filter_apply(dn_low_pass_filter_t* filter, f32 input);
 dn_audio_config_t          dn_audio_config_default();
 void                       dn_audio_init(dn_audio_config_t config);
typedef struct {
  dn_hash_t hash;
  u32 handle;
  u32 width;
  u32 height;
  u32 channels;
} dn_texture_t;
 void          dn_texture_init(s32 width, s32 height, s32 channels);
 void          dn_texture_load_to_gpu(u32* data);
 void          dn_texture_unload_from_gpu();
 dn_texture_t* dn_images_find_texture_by_name(dn_string_t name);
 dn_texture_t* dn_images_find_texture_by_hash(dn_hash_t hash);
typedef struct {
  dn_string_t file_path;
  dn_hash_t hash;
  dn_hash_t texture;
  dn_vector2_t* uv;
  dn_vector2i_t size;
} dn_sprite_t;
 dn_sprite_t* dn_images_find_sprite(const char* name);
 dn_sprite_t* dn_images_find_sprite_no_default(const char* name);
typedef struct {
  s32 id;
  dn_sprite_t* sprite;
  u32* data;
  s32 channels;
} dn_atlas_pack_item_t;
void dn_atlas_pack_item_init(dn_atlas_pack_item_t* item);
typedef struct {
  char name [64];
  double cfg_mod_time;
  double mod_time;
  dn_hash_t cfg_files_hash;
  dn_hash_t files_hash;
  bool high_priority;
  
  dn_texture_t* texture;
  
  
  
  
  
  
  bool need_async_build;
  bool need_async_load;
  bool need_gl_init;
  bool need_config_update;
  
} dn_atlas_t;
typedef struct {
  dn_dynamic_array_t files;
} dn_atlas_bucket_t;
typedef struct {
  dn_string_t* dirs;
  u32 num_dirs;
} dn_image_config_t;
typedef struct {
  dn_mutex_t mutex;
  dn_mutex_t config_mutex;
  dn_fixed_array_t uv_data;
  dn_fixed_array_t textures;
  dn_fixed_array_t sprites;
  dn_fixed_array_t atlases;
  dn_fixed_array_t buckets;
} dn_images_t;
dn_images_t dn_images;
void                  dn_atlases_init();
void                  dn_atlas_deinit();
void                  dn_atlas_set_name(const char* name);
void                  dn_atlas_build_sync();
void                  dn_atlas_load_to_memory();
bool                  dn_atlas_is_dirty();
void                  dn_atlas_calc_hash_and_mod_time();  
void                  dn_atlas_build_from_config();
void                  dn_atlas_build_from_source();
void                  dn_atlas_write_to_config();
void                  dn_atlas_write_to_png();
void                  dn_atlas_load_to_gpu();
void                  dn_atlas_unload_from_gpu();
void                  dn_atlas_watch_files();
void                  dn_atlas_on_file_add(const char* file_path);
void                  dn_atlas_on_file_change(const char* file_path);
dn_atlas_pack_item_t* dn_atlas_find_pack_item(s32 id);
void                  dn_image_init_atlases();
dn_sprite_t*          dn_image_alloc_sprite();
dn_texture_t*         dn_image_alloc_texture();
dn_vector2_t*         dn_image_alloc_uvs();
void                  dn_image_build_sprite_from_file(const char* id, const char* file_path);
void                  dn_image_build_sprite_from_data(const char* id, unsigned char* data, s32 width, s32 height, s32 channels);
void                  dn_image_build_sprite_ex(dn_sprite_t* sprite, const char* id, unsigned char* data, s32 width, s32 height, s32 channels);
 void           dn_screenshots_init();
 void           dn_screenshot_take();
 void           dn_screenshot_write(const char* file_name);
typedef struct {
  dn_texture_t* texture;
  u32* data;
} dn_background_loaded_tile_t;
typedef struct {
  char* name;
  char* tile_output_folder;
  char* tile_output_full_path;
  char* source_image;
  char* source_image_full_path;
  
  
  
  s32 width;
  s32 height;
  s32 channels;
  u32* data;
  bool high_priority;
  f64 mod_time;
  f64 filesystem_mod_time;
  bool need_async_build;
  bool need_async_load;
  bool need_config_update;
  
  bool gpu_ready;
  bool gpu_done;
  int gpu_load_index;
} dn_background_t;
typedef struct {
  dn_dynamic_array_t backgrounds;
} dn_backgrounds_t;
typedef struct {
  u32 current_tile;
  dn_mutex_t mutex;
} dn_background_tile_processor_shared_t;
typedef struct {
  u32* source_data;
  u32* tile_data;
  dn_thread_t thread;
} dn_background_tile_processor_t;
void dn_background_tile_processor_init(u32* source_data);
void dn_background_tile_processor_deinit();
void dn_background_tile_processor_process(dn_background_t* background);
void dn_background_init(dn_background_t* background);
void dn_background_deinit(dn_background_t* background);
void dn_background_load_paths(dn_background_t* background);
void dn_background_set_source_image_size(dn_background_t* background, s32 width, s32 height);
void dn_background_set_source_data(dn_background_t* background, u32* data);
bool dn_background_add_tile(dn_background_t* background);
void dn_background_add_tiles(dn_background_t* background);
bool dn_background_is_dirty(dn_background_t* background);
void dn_background_build_from_source(dn_background_t* background);
void dn_background_load_to_gpu(dn_background_t* background);
void dn_background_load_one_to_gpu(dn_background_t* background);
void dn_background_load_tiles(dn_background_t* background);
void dn_background_update_config(dn_background_t* background);
void dn_backgrounds_init();
typedef enum {
  DN_ASSET_KIND_BACKGROUND,
  DN_ASSET_KIND_ATLAS,
  DN_ASSET_KIND_USER,
} dn_asset_kind_t;
typedef enum {
  DN_ASSET_COMPLETION_STATUS_RUNNING,
  DN_ASSET_COMPLETION_STATUS_DONE,
} dn_asset_completion_status_t;
typedef void* dn_asset_data_t;
typedef struct {
  dn_asset_name_t name;
  dn_asset_data_t data;
} dn_asset_t;
typedef struct {
  s32 id;
  dn_asset_kind_t kind;
  union {
    dn_background_t* background;
    dn_atlas_t* atlas;
  };
} dn_asset_load_request_t;
typedef struct {
  dn_thread_t thread;
  dn_condition_variable_t condition;
  dn_mutex_t mutex;
  dn_ring_buffer_t load_requests;
  dn_ring_buffer_t completion_queue;
} dn_asset_loader_t;
typedef struct {
  void* user_data;
} dn_asset_import_request_t;
typedef void(*dn_asset_import_fn)(dn_asset_import_request_t* request);
typedef dn_asset_completion_status_t(*dn_asset_completion_fn)(dn_asset_import_request_t* request);
typedef struct {
  dn_asset_name_t id;
  dn_asset_import_fn on_import;
  dn_asset_completion_fn on_complete;
} dn_asset_importer_t;
typedef struct {
  struct {
    dn_asset_importer_t* data;
    u32 count;
  } importers;
} dn_asset_config_t;
typedef struct {
  struct { struct { dn_hash_t key; dn_asset_t val; gs_hash_table_entry_state state; }* data; dn_hash_t tmp_key; dn_asset_t tmp_val; size_t stride; size_t klpvl; size_t tmp_idx; }* assets;
  struct { struct { dn_hash_t key; dn_asset_importer_t val; gs_hash_table_entry_state state; }* data; dn_hash_t tmp_key; dn_asset_importer_t tmp_val; size_t stride; size_t klpvl; size_t tmp_idx; }* importers;
} dn_asset_registry_t;
 void            dn_asset_registry_init(dn_asset_config_t config);
 dn_asset_data_t dn_asset_registry_find(const char* name);
 void            dn_asset_registry_add(const char* name, dn_asset_data_t data);
typedef enum {
  DN_BACKGROUND_COMPLETION_STATUS_UPDATE_CONFIG,
  DN_BACKGROUND_COMPLETION_STATUS_LOAD_TILES,
  DN_BACKGROUND_COMPLETION_STATUS_DONE,
} dn_background_completion_status_t;
typedef struct {
  dn_background_completion_status_t status;
  dn_background_t* background;
} dn_background_import_request_t;
typedef enum {
  DN_TEST_CONTEXT_FLAG_DEFAULT = 0,
  DN_TEST_CONTEXT_FLAG_LOG_FAILURE = 1 << 0,
  DN_TEST_CONTEXT_FLAG_LOG_SUCCESS = 1 << 1,
} dn_test_context_flags_t;
typedef struct {
  dn_string_t description;
  bool success;
} dn_assertion_t;
typedef struct {
  dn_string_t name;
  bool success;
  u32 num_assertions;
  dn_dynamic_array_t assertions;
} dn_test_context_t;
typedef struct {
  dn_string_t name;
  dn_test_context_flags_t flags;
  dn_dynamic_array_t tests;
} dn_test_suite_t;
typedef struct {
  dn_test_suite_t* suite;
  dn_test_context_t* context;
} dn_tests_t;
dn_tests_t dn_tests;
void dn_test_init();
void dn_test_context_init(dn_test_context_t* context);
void dn_test_suite_init(dn_test_suite_t* suite);
void dn_test_begin_suite(dn_test_suite_t* suite);
void dn_test_begin(dn_string_t name);
void dn_test_end();
void dn_test_end_suite();
typedef enum {
  DN_WINDOW_FLAG_NONE,
  DN_WINDOW_FLAG_WINDOWED,
  DN_WINDOW_FLAG_BORDER,
  DN_WINDOW_FLAG_VSYNC,
} dn_window_flags_t;
typedef enum {
  DN_DISPLAY_MODE_AUTO,
  
  DN_DISPLAY_MODE_180,
  DN_DISPLAY_MODE_480,
  DN_DISPLAY_MODE_720,
  DN_DISPLAY_MODE_1080,
  DN_DISPLAY_MODE_1440,
  DN_DISPLAY_MODE_2160,
  
  DN_DISPLAY_MODE_1280_800,
  
  DN_DISPLAY_MODE_FULLSCREEN
} dn_display_mode_t;
typedef struct {
  dn_string_t title;
  dn_string_t icon;
  dn_display_mode_t display_mode;
  dn_vector2_t native_resolution;
  dn_window_flags_t flags;
  u32 target_fps;
} dn_window_config_t;
  typedef void* dn_window_handle_t;
typedef struct {
  dn_window_handle_t handle;
  dn_window_flags_t flags;
  dn_display_mode_t display_mode;
  dn_vector2i_t windowed_position;
  dn_vector2_t native_resolution;
  dn_vector2_t requested_area;
  dn_vector2_t content_area;
} dn_window_t;
 dn_window_config_t dn_window_config_default();
 void               dn_window_init(dn_window_config_t config);
 void               dn_window_set_native_resolution(float width, float height);
 dn_vector2_t       dn_window_get_content_area();
 dn_vector2_t       dn_window_get_native_resolution();
 void               dn_window_set_icon(dn_string_t path);
 void               dn_window_set_display_mode(dn_display_mode_t mode);
 dn_display_mode_t  dn_window_get_display_mode();
 void               dn_window_set_cursor_visible(bool visible);
 void               dn_window_set_size(int x, int y);
typedef struct {
  dn_vector4_t light;
  dn_vector4_t medium_light;
  dn_vector4_t low_light;
  dn_vector4_t neutral;
  dn_vector4_t medium_dark;
} dn_imgui_colors_t;
typedef struct {
  dn_string_t queued_layout;
  dn_imgui_colors_t colors;
} dn_imgui_t;
dn_imgui_t dn_imgui;
 void    dn_imgui_push_font(const char* font_name, u32 size);
 void    dn_imgui_image(const char* image, float sx, float sy);
 void    dn_imgui_file_browser_open();
 void    dn_imgui_file_browser_close();
 void    dn_imgui_file_browser_set_work_dir(const char* directory);
 bool    dn_imgui_file_browser_is_file_selected();
 dn_tstring_t dn_imgui_file_browser_get_selected_file();
 void    dn_imgui_load_layout(const char* file_name);
 void    dn_imgui_save_layout(const char* file_name);
 void    dn_imgui_load_colors(dn_imgui_colors_t colors);
  typedef void* dn_lua_interpreter_t;
typedef struct {
  dn_string_t scripts;
} dn_lua_config_t;
typedef struct {
  dn_string_t scripts;
  dn_lua_interpreter_t state;
  dn_fixed_array_t directories;
} dn_lua_t;
dn_lua_t dn_lua;
typedef enum {
  DN_FONT_FLAG_NONE = 0,
  DN_FONT_FLAG_IMGUI = 1,
  DN_FONT_FLAG_DEFAULT = 2,
  DN_FONT_FLAG_BOLD = 4,
  DN_FONT_FLAG_EXTRABOLD = 8,
  DN_FONT_FLAG_ITALIC = 16,
} dn_font_flags_t;
typedef struct {
  dn_vector2_t* verts;
  dn_vector2_t* uv;
  dn_vector2_t size;
  dn_vector2_t bearing;
  dn_vector2_t advance;
  f32 descender;
} dn_baked_glyph_t;
typedef struct {
  dn_asset_name_t name;
  dn_hash_t hash;
  dn_path_t path;
  dn_font_flags_t flags;
  
  u32 size;
  u32 texture;
  
  dn_vector2_t resolution;
   
  dn_baked_glyph_t* glyphs;
  dn_vector2_t max_advance;
  dn_vector2_t max_glyph;
  f32 line_spacing;
} dn_baked_font_t;
typedef struct {
  const char* id;
  const char* file_path;
  u32 sizes [16];
  dn_font_flags_t flags;
} dn_font_descriptor_t;
typedef struct {
  dn_font_descriptor_t* fonts;
  u32 num_fonts;
} dn_font_config_t;
typedef struct {
  dn_fixed_array_t baked_fonts;
  dn_fixed_array_t baked_glyphs;
  dn_fixed_array_t vertex_data;
  dn_fixed_array_t uv_data;
  dn_baked_font_t* default_font;
} dn_fonts_t;
dn_fonts_t dn_fonts;
 void             dn_font_init(dn_font_config_t config);
 void             dn_font_bake_n(dn_font_descriptor_t* descriptors, u32 num_descriptors);
 void             dn_font_bake(dn_font_descriptor_t descriptor);
 dn_font_config_t dn_font_config_default();
 dn_baked_font_t* dn_font_default();
 dn_baked_font_t* dn_font_find(const char* id, u32 size);
typedef enum {
  DN_GPU_COMMAND_OP_BIND_BUFFERS = 10,
  DN_GPU_COMMAND_OP_BEGIN_RENDER_PASS = 20,
  DN_GPU_COMMAND_OP_END_RENDER_PASS = 21,
  DN_GPU_COMMAND_OP_BIND_PIPELINE = 30,
  DN_GPU_COMMAND_OP_SET_CAMERA = 40,
  DN_GPU_COMMAND_OP_SET_LAYER = 41,
  DN_GPU_COMMAND_OP_SET_WORLD_SPACE = 42,
  DN_GPU_COMMAND_OP_SET_SCISSOR = 43,
  DN_GPU_COMMAND_OP_DRAW = 70,
} dn_gpu_command_op_t;
typedef enum {
  DN_GPU_PRIMITIVE_TRIANGLES = 0
} dn_gpu_draw_primitive_t;
typedef enum {
  DN_GPU_DRAW_MODE_ARRAYS = 0,
  DN_GPU_DRAW_MODE_INSTANCE = 1,
} dn_gpu_draw_mode_t;
typedef enum {
  DN_GPU_VERTEX_ATTRIBUTE_FLOAT = 0,
  DN_GPU_VERTEX_ATTRIBUTE_U32 = 1,
} dn_gpu_vertex_attribute_kind_t;
typedef enum {
  DN_GPU_UNIFORM_NONE = 0,
  DN_GPU_UNIFORM_MATRIX4 = 1,
  DN_GPU_UNIFORM_MATRIX3 = 2,
  DN_GPU_UNIFORM_MATRIX2 = 3,
  DN_GPU_UNIFORM_VECTOR4 = 4,
  DN_GPU_UNIFORM_VECTOR3 = 5,
  DN_GPU_UNIFORM_VECTOR2 = 6,
  DN_GPU_UNIFORM_I32 = 7,
  DN_GPU_UNIFORM_F32 = 8,
  DN_GPU_UNIFORM_TEXTURE = 9,
  DN_GPU_UNIFORM_ENUM = 10,
} dn_gpu_uniform_kind_t;
typedef enum {
  DN_GPU_BUFFER_KIND_STORAGE = 0,
  DN_GPU_BUFFER_KIND_ARRAY = 1,
  DN_GPU_BUFFER_KIND_UNIFORM = 2,
} dn_gpu_buffer_kind_t;
typedef enum {
  DN_GPU_BUFFER_USAGE_STATIC = 0,
  DN_GPU_BUFFER_USAGE_DYNAMIC = 1,
  DN_GPU_BUFFER_USAGE_STREAM = 2,
} dn_gpu_buffer_usage_t;
typedef enum {
  DN_GPU_BLEND_FUNC_NONE,
  DN_GPU_BLEND_FUNC_ADD,
  DN_GPU_BLEND_FUNC_SUBTRACT,
  DN_GPU_BLEND_FUNC_REVERSE_SUBTRACT,
  DN_GPU_BLEND_FUNC_MIN,
  DN_GPU_BLEND_FUNC_MAX
} dn_gpu_blend_func_t;
typedef enum {
  DN_GPU_BLEND_MODE_ZERO,
  DN_GPU_BLEND_MODE_ONE,
  DN_GPU_BLEND_MODE_SRC_COLOR,
  DN_GPU_BLEND_MODE_ONE_MINUS_SRC_COLOR,
  DN_GPU_BLEND_MODE_DST_COLOR,
  DN_GPU_BLEND_MODE_ONE_MINUS_DST_COLOR,
  DN_GPU_BLEND_MODE_SRC_ALPHA,
  DN_GPU_BLEND_MODE_ONE_MINUS_SRC_ALPHA,
  DN_GPU_BLEND_MODE_DST_ALPHA,
  DN_GPU_BLEND_MODE_ONE_MINUS_DST_ALPHA,
  DN_GPU_BLEND_MODE_CONSTANT_COLOR,
  DN_GPU_BLEND_MODE_ONE_MINUS_CONSTANT_COLOR,
  DN_GPU_BLEND_MODE_CONSTANT_ALPHA,
  DN_GPU_BLEND_MODE_ONE_MINUS_CONSTANT_ALPHA,
  DN_GPU_BLEND_MODE_SRC_ALPHA_SATURATE,
  DN_GPU_BLEND_MODE_SRC1_COLOR,
  DN_GPU_BLEND_MODE_ONE_MINUS_SRC1_COLOR,
  DN_GPU_BLEND_MODE_SRC1_ALPHA,
  DN_GPU_BLEND_MODE_ONE_MINUS_SRC1_ALPHA
} dn_gpu_blend_mode_t;
typedef enum {
  DN_GPU_LOAD_OP_NONE = 0,
  DN_GPU_LOAD_OP_CLEAR = 1
} dn_gpu_load_op_t;
typedef enum {
  DN_GPU_RESOURCE_FRAMEBUFFER = 0,
  DN_GPU_RESOURCE_SHADER = 1,
  DN_GPU_RESOURCE_PROGRAM = 2,
} dn_gpu_resource_id_t;
typedef enum {
  DN_GPU_MEMORY_BARRIER_STORAGE = 0,
  DN_GPU_MEMORY_BARRIER_BUFFER_UPDATE = 1,
} dn_gpu_memory_barrier_t;
typedef enum {
  DN_GPU_SHADER_GRAPHICS = 0,
  DN_GPU_SHADER_COMPUTE = 1,
} dn_gpu_shader_kind_t;
typedef struct {
  const char* name;
  const char* vertex_shader;
  const char* fragment_shader;
  const char* compute_shader;
  dn_gpu_shader_kind_t kind;
} dn_gpu_shader_descriptor_t;
typedef struct {
  dn_gpu_shader_kind_t kind;
  dn_asset_name_t name;
  u32 program;
  
  union {
    struct {
      char* vertex_path;
      char* fragment_path;
      u32 vertex_shader;
      u32 fragment_shader;
    } graphics;
    struct {
      char* path;
      u32 shader;
    } compute;
  };
} dn_gpu_shader_t;
typedef union {
  dn_matrix4_t mat4;
  dn_matrix3_t mat3;
  dn_matrix2_t mat2;
  dn_vector4_t vec4;
  dn_vector3_t vec3;
  dn_vector2_t vec2;
  float f32;
  s32 texture;
  s32 i32;
} dn_gpu_uniform_data_t;
typedef struct {
  dn_asset_name_t name;
  dn_gpu_uniform_kind_t kind;
} dn_gpu_uniform_descriptor_t;
typedef struct {
  dn_asset_name_t name;
  dn_gpu_uniform_kind_t kind;
} dn_gpu_uniform_t;
typedef struct {
  dn_asset_name_t name;
  dn_gpu_buffer_kind_t kind;
  dn_gpu_buffer_usage_t usage;
  u32 capacity;
  u32 element_size;
} dn_gpu_buffer_descriptor_t;
typedef struct {
  dn_asset_name_t name;
  dn_gpu_buffer_kind_t kind;
  dn_gpu_buffer_usage_t usage;
  u32 size;
  u32 handle;
} dn_gpu_buffer_t;
typedef struct {
  dn_fixed_array_t buffer;
  dn_gpu_buffer_t* gpu_buffer;
} dn_gpu_backed_buffer_t;
typedef struct {
  dn_asset_name_t name;
  dn_vector2_t size;
} dn_gpu_render_target_descriptor_t;
typedef struct {
  dn_asset_name_t name;
  u32 handle;
  u32 color_buffer;
  dn_vector2_t size;
} dn_gpu_render_target_t;
typedef struct {
  struct {
    dn_gpu_load_op_t load;
    dn_gpu_render_target_t* attachment;
  } color;
} dn_gpu_render_pass_t;
typedef struct {
  dn_gpu_buffer_t* buffer;
} dn_gpu_vertex_buffer_binding_t;
typedef struct {
  dn_gpu_vertex_buffer_binding_t bindings [8];
  u32 count;
} dn_gpu_vertex_buffer_binding_array_t;
typedef struct {
  dn_gpu_buffer_t* buffer;
  u32 base;
} dn_gpu_storage_buffer_binding_t;
typedef struct {
  dn_gpu_storage_buffer_binding_t bindings [8];
  u32 count;
} dn_gpu_storage_buffer_binding_array_t;
typedef struct {
  dn_gpu_uniform_data_t data;
  dn_gpu_uniform_t* uniform;
  u32 binding_index;
} dn_gpu_uniform_binding_t;
typedef struct {
  dn_gpu_uniform_binding_t bindings [8];
  u32 count;
} dn_gpu_uniform_binding_array_t;
typedef struct {
  dn_asset_name_t name;
  dn_gpu_buffer_t* buffer;
  u32 binding_index;
} dn_gpu_uniform_buffer_binding_t;
typedef struct {
  dn_gpu_uniform_buffer_binding_t bindings [8];
  u32 count;
} dn_gpu_uniform_buffer_binding_array_t;
typedef struct {
   dn_gpu_vertex_buffer_binding_array_t vertex;
   dn_gpu_uniform_binding_array_t uniforms;
   dn_gpu_storage_buffer_binding_array_t storage;
   dn_gpu_uniform_buffer_binding_array_t uniform_buffers;
} dn_gpu_buffer_binding_t;
typedef struct {
  dn_gpu_blend_func_t fn;
  dn_gpu_blend_mode_t source;
  dn_gpu_blend_mode_t destination;
} dn_gpu_blend_state_t;
typedef struct {
  dn_gpu_shader_t* shader;
  dn_gpu_draw_primitive_t primitive;
} dn_gpu_raster_state_t;
typedef struct {
  dn_vector2_t position;
  dn_vector2_t size;
  bool enabled;
} dn_gpu_scissor_state_t;
typedef struct {
  u32 layer;
  bool world_space;
  dn_vector2_t camera;
  dn_matrix4_t projection;
} dn_gpu_renderer_state_t;
typedef struct {
  dn_gpu_vertex_attribute_kind_t kind;
  u32 count;
  u32 divisor;
} dn_gpu_vertex_attribute_t;
typedef struct {
  dn_gpu_vertex_attribute_t vertex_attributes [8];
  u32 num_vertex_attributes;
} dn_gpu_buffer_layout_t;
typedef struct {
  u32 size;
  u32 value;
} dn_gpu_vertex_attr_info_t;
typedef struct {
  dn_gpu_blend_state_t blend;
  dn_gpu_raster_state_t raster;
  dn_gpu_buffer_layout_t buffer_layouts [8];
  u32 num_buffer_layouts;
} dn_gpu_pipeline_descriptor_t;
typedef struct {
  dn_gpu_blend_state_t blend;
  dn_gpu_raster_state_t raster;
  dn_gpu_buffer_layout_t buffer_layouts [8];
  u32 num_buffer_layouts;
} dn_gpu_pipeline_t;
typedef struct {
  dn_gpu_draw_mode_t mode;
  u32 vertex_offset;
  u32 num_vertices;
  u32 num_instances;
} dn_gpu_draw_call_t;
typedef struct {
  dn_gpu_command_op_t op;
  union {
    dn_gpu_pipeline_t*       pipeline;
    dn_gpu_buffer_binding_t  bindings;
    dn_gpu_render_pass_t     render_pass;
    dn_gpu_renderer_state_t  render;
    dn_gpu_scissor_state_t   scissor;
    dn_gpu_draw_call_t       draw;
  };
} dn_gpu_command_t;
typedef struct {
  u32 max_commands;
} dn_gpu_command_buffer_descriptor_t;
typedef struct {
  dn_gpu_pipeline_t* pipeline;
  dn_gpu_buffer_binding_t bindings;
  dn_gpu_render_pass_t render_pass;
  dn_gpu_renderer_state_t render;
  dn_gpu_scissor_state_t scissor;
  dn_fixed_array_t commands;
  u32 vao;
} dn_gpu_command_buffer_t;
typedef struct {
  dn_matrix4_t view;
  dn_matrix4_t projection;
  dn_vector2_t camera;
  dn_vector2_t native_resolution;
  dn_vector2_t output_resolution;
  f32 master_time;
} dn_gpu_uniforms_t;
typedef struct {
  const char* shader_path;
  dn_gpu_shader_descriptor_t* shaders;
  u32 num_shaders;
  const char** search_paths;
  u32 num_search_paths;
  dn_gpu_render_target_descriptor_t* render_targets;
  u32 num_render_targets;
} dn_gpu_config_t;
typedef struct {
  dn_fixed_array_t command_buffers;
  dn_fixed_array_t uniforms;
  dn_fixed_array_t pipelines;
  dn_fixed_array_t  gpu_buffers;
  dn_fixed_array_t  targets;
  dn_fixed_array_t shaders;
  
  dn_fixed_array_t search_paths;
  struct {
    dn_gpu_buffer_t* buffer;
    dn_gpu_uniform_buffer_binding_t binding;
    dn_gpu_uniforms_t data;
  } builtin_uniforms;
} dn_gpu_t;
dn_gpu_t dn_gpu;
 void                      dn_gpu_init(dn_gpu_config_t config);
 dn_gpu_command_buffer_t*  dn_gpu_command_buffer_create(dn_gpu_command_buffer_descriptor_t descriptor);
 void                      dn_gpu_command_buffer_draw(dn_gpu_command_buffer_t* command_buffer, dn_gpu_draw_call_t draw_call);
 void                      dn_gpu_command_buffer_submit(dn_gpu_command_buffer_t* command_buffer);
 void                      dn_gpu_bind_pipeline(dn_gpu_command_buffer_t* command_buffer, dn_gpu_pipeline_t* pipeline);
 void                      dn_gpu_begin_render_pass(dn_gpu_command_buffer_t* command_buffer, dn_gpu_render_pass_t render_pass);
 void                      dn_gpu_end_render_pass(dn_gpu_command_buffer_t* command_buffer);
 void                      dn_gpu_apply_bindings(dn_gpu_command_buffer_t* command_buffer, dn_gpu_buffer_binding_t bindings);
 void                      dn_gpu_bind_render_state(dn_gpu_command_buffer_t* command_buffer, dn_gpu_renderer_state_t render);
 void                      dn_gpu_set_layer(dn_gpu_command_buffer_t* command_buffer, u32 layer);
 void                      dn_gpu_set_world_space(dn_gpu_command_buffer_t* command_buffer, bool world_space);
 void                      dn_gpu_set_camera(dn_gpu_command_buffer_t* command_buffer, dn_vector2_t camera);
 dn_gpu_pipeline_t*        dn_gpu_pipeline_create(dn_gpu_pipeline_descriptor_t descriptor);
 dn_gpu_uniform_t*         dn_gpu_uniform_create(dn_gpu_uniform_descriptor_t descriptor);
 dn_gpu_buffer_t*          dn_gpu_buffer_create(dn_gpu_buffer_descriptor_t descriptor);
 void                      dn_gpu_buffer_bind(dn_gpu_buffer_t* buffer);
 void                      dn_gpu_buffer_bind_base(dn_gpu_buffer_t* buffer, u32 base);
 void                      dn_gpu_buffer_sync(dn_gpu_buffer_t* buffer, void* data, u32 size);
 void                      dn_gpu_buffer_sync_subdata(dn_gpu_buffer_t* buffer, void* data, u32 byte_size, u32 byte_offset);
 void                      dn_gpu_buffer_zero(dn_gpu_buffer_t* buffer, u32 size);
 dn_gpu_backed_buffer_t    dn_gpu_backed_buffer_create(dn_gpu_buffer_descriptor_t descriptor);
 u32                       dn_gpu_backed_buffer_size(dn_gpu_backed_buffer_t* buffer);
 void                      dn_gpu_backed_buffer_clear(dn_gpu_backed_buffer_t* buffer);
 u8*                       dn_gpu_backed_buffer_push(dn_gpu_backed_buffer_t* buffer, void* data, u32 num_elements);
 void                      dn_gpu_backed_buffer_sync(dn_gpu_backed_buffer_t* buffer);
 dn_gpu_shader_t*          dn_gpu_shader_create(dn_gpu_shader_descriptor_t descriptor);
 dn_gpu_shader_t*          dn_gpu_shader_find(const char* name);
 dn_gpu_render_target_t*   dn_gpu_render_target_create(dn_gpu_render_target_descriptor_t descriptor);
 dn_gpu_render_target_t*   dn_gpu_acquire_swapchain();
 dn_gpu_render_target_t*   dn_gpu_render_target_find(const char* name);
 void                      dn_gpu_render_target_bind(dn_gpu_render_target_t* target);
 void                      dn_gpu_render_target_clear(dn_gpu_render_target_t* target);
 void                      dn_gpu_render_target_blit(dn_gpu_render_target_t* source, dn_gpu_render_target_t* destination);
 void                      dn_gpu_memory_barrier(dn_gpu_memory_barrier_t barrier);
 void                      dn_gpu_dispatch_compute(dn_gpu_buffer_t* buffer, u32 size);
 void                      dn_gpu_swap_buffers();
 void                      dn_gpu_error_clear();
 dn_tstring_t              dn_gpu_error_read();
 void                      dn_gpu_error_log_one();
 void                      dn_gpu_error_log_all();
 void                      dn_gpu_set_resource_name(dn_gpu_resource_id_t id, u32 handle, u32 name_len, const char* name);
typedef enum {
  DN_SDF_SHAPE_CIRCLE = 0,
  DN_SDF_SHAPE_RING = 1,
  DN_SDF_SHAPE_BOX = 2,
  DN_SDF_SHAPE_ORIENTED_BOX = 3,
  DN_SDF_SHAPE_COMBINE = 100,
} dn_sdf_shape_t;
typedef enum {
  DN_SDF_COMBINE_OP_UNION = 0,
  DN_SDF_COMBINE_OP_INTERSECTION = 1,
  DN_SDF_COMBINE_OP_SUBTRACTION = 2,
} dn_sdf_combine_op_t;
typedef enum {
  DN_SDF_SMOOTH_KERNEL_NONE = 0,
  DN_SDF_SMOOTH_KERNEL_POLYNOMIAL_QUADRATIC = 1,
} dn_sdf_smoothing_kernel_t;
typedef enum {
  DN_SDF_RENDERER_STATE_NONE,
  DN_SDF_RENDERER_STATE_COMBINATION,
} dn_sdf_renderer_state_t;
typedef struct {
  dn_vector2_t position;
  dn_vector2_t uv;
} dn_sdf_vertex_t;
typedef struct {
  dn_sdf_shape_t shape;
  u32 buffer_index;
} dn_sdf_instance_t;
typedef struct {
  u32 num_sdfs;
} dn_sdf_combine_header_t;
typedef struct {
  u32 buffer_index;
  dn_sdf_shape_t shape;
  dn_sdf_combine_op_t op;
  dn_sdf_smoothing_kernel_t kernel;
} dn_sdf_combine_entry_t;
typedef struct {
  dn_vector3_t color;
  dn_vector2_t position;
  float rotation;
  float edge_thickness;
  dn_sdf_shape_t shape;
} dn_sdf_header_t;
typedef struct {
  dn_sdf_header_t header;
  float radius;
} dn_sdf_circle_t;
typedef struct {
  dn_sdf_header_t header;
  float inner_radius;
  float outer_radius;
} dn_sdf_ring_t;
typedef struct {
  dn_sdf_header_t header;
  dn_vector2_t size;
} dn_sdf_oriented_box_t;
typedef struct {
  dn_sdf_renderer_state_t state;
  dn_gpu_backed_buffer_t vertices;
  dn_gpu_backed_buffer_t instances;
  dn_gpu_backed_buffer_t combinations;
  dn_gpu_backed_buffer_t shape_data;
  dn_gpu_buffer_binding_t bindings;
  dn_gpu_pipeline_t* pipeline;
} dn_sdf_renderer_t;
 dn_sdf_renderer_t        dn_sdf_renderer_create(u32 buffer_size);
 void                     dn_sdf_renderer_draw(dn_sdf_renderer_t* renderer, dn_gpu_command_buffer_t* command_buffer);
 void                     dn_sdf_renderer_push_instance(dn_sdf_renderer_t* renderer, dn_sdf_shape_t shape);
 void                     dn_sdf_renderer_push_header(dn_sdf_renderer_t* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness);
 dn_sdf_combine_header_t* dn_sdf_combination_begin(dn_sdf_renderer_t* renderer);
 void                     dn_sdf_combination_append(dn_sdf_renderer_t* renderer, dn_sdf_combine_header_t* header, dn_sdf_shape_t shape, dn_sdf_combine_op_t op, dn_sdf_smoothing_kernel_t kernel);
 void                     dn_sdf_combination_commit(dn_sdf_renderer_t* renderer);
 void                     dn_sdf_circle_ex(dn_sdf_renderer_t* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float radius);
 void                     dn_sdf_ring_ex(dn_sdf_renderer_t* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float inner_radius, float outer_radius);
 void                     dn_sdf_oriented_box_ex(dn_sdf_renderer_t* renderer, float px, float py, float r, float g, float b, float rotation, float edge_thickness, float length, float thickness);
 void                     dn_sdf_grid(dn_sdf_renderer_t* renderer, u32 grid_width, u32 grid_size);
typedef enum {
  DN_STEAM_INIT_NONE,
  DN_STEAM_INIT_FAILED,
  DN_STEAM_INIT_OK,
} dn_steam_init_state_t;
typedef enum {
  DN_STEAM_INPUT_TEXT_UNINITIALIZED,
  DN_STEAM_INPUT_TEXT_OPEN,
  DN_STEAM_INPUT_TEXT_UNREAD,
  DN_STEAM_INPUT_TEXT_CLOSED,
} dn_steam_text_input_state_t;
typedef struct {
  
  u32 dummy;
} dn_steam_callbacks_t ;
dn_steam_callbacks_t dn_steam_callbacks;
typedef struct {
  u32 app_id;
} dn_steam_config_t;
typedef struct {
  u32 app_id;
  dn_steam_init_state_t state;
  struct {
    dn_steam_text_input_state_t state;
    dn_string_t contents;
  } text_input;
} dn_steam_t;
dn_steam_t dn_steam;
 void                        dn_steam_init(dn_steam_config_t config);
 bool                        dn_steam_initialized();
 void                        dn_steam_open_store_page();
 void                        dn_steam_open_store_page_utm(dn_string_t utm);
 void                        dn_steam_open_store_page_ex(dn_string_t url);
 void                        dn_steam_open_text_input();
 dn_steam_text_input_state_t dn_steam_text_input_state();
 dn_string_t                 dn_string_read_text_input();
 bool                        dn_steam_is_deck();
typedef enum {
  DN_MODE_CORE,
  DN_MODE_CORE_LUA,
  DN_MODE_APP,
} dn_mode_t;
typedef struct {
  dn_mode_t mode;
  dn_path_config_t path;
  dn_lua_config_t lua;
} dn_config_t;
typedef struct {
  dn_window_config_t window;
  dn_audio_config_t audio;
  dn_font_config_t font;
  dn_gpu_config_t gpu;
  dn_asset_config_t asset;
  dn_steam_config_t steam;
  dn_image_config_t image;
} dn_app_config_t;
typedef struct {
  const char* name;
  u32 size;
} dn_type_info_t;
typedef struct {
  dn_type_info_t* data;
  u32 count;
} dn_type_info_list_t;
typedef struct {
  dn_window_t window;
  dn_asset_loader_t loader;
  dn_asset_registry_t asset_registry;
  f32 target_fps;
  f32 dt;
  f32 elapsed_time;
  s32 frame;
   u32 monotonic_id;
  bool exit_game;
  bool steam;
} dn_app_t;
dn_app_t dn_app;
 void dn_app_configure(dn_app_config_t config);
 void                dn_app_set_exit_game();
 const char*         dn_app_get_game_hash();
 void                dn_app_set_target_fps(f32 fps);
 f32                 dn_app_get_target_fps();
 bool                dn_app_exceeded_frame_time();
 bool                dn_app_should_exit();
 dn_type_info_list_t dn_app_query_types();
 u32                 dn_app_monotonic_id();
