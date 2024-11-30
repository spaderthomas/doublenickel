struct Texture {
  dn_hash_t hash;
  u32 handle;
  i32 width;
  i32 height;
  i32 channels;

  void init(i32 width, i32 height, i32 channels);
  void load_to_gpu(u32* data);
  void unload_from_gpu();
};
Texture* find_texture(const char* name);
Texture* find_texture(dn_hash_t hash);
DN_API u32 find_texture_handle(const char* name);

struct Sprite {
  char file_path [DN_MAX_PATH_LEN];
  dn_hash_t hash;
  dn_hash_t texture;
  dn_vector2_t* uv;
  dn_vector2_tI size;
};
Sprite* find_sprite(const char* name);
Sprite* find_sprite_no_default(const char* name);


struct Path {
  char path [DN_MAX_PATH_LEN];
};

struct RectPackId {
  static i32 next_id;
  
  i32 id;
  Sprite* sprite;
  u32* data;
  i32 channels;

  RectPackId() { id = next_id++; }
};
i32 RectPackId::next_id = 0;

#define MAX_DIRECTORIES_PER_ATLAS 32
#define MAX_IMAGES_PER_ATLAS 256
constexpr i32 TEXTURE_ATLAS_SIZE = 2048;

struct TextureAtlas {
  char name [64];
  double cfg_mod_time;
  double mod_time;
  dn_hash_t cfg_files_hash;
  dn_hash_t files_hash;
  bool high_priority;
  dn_array_t<char*> directories;
  Texture* texture = nullptr;

  // Data used for generation. These are initialized to temporary storage, so
  // don't expect them to be valid in other cases.
  dn_array_t<RectPackId> ids;
  dn_array_t<stbrp_rect> rects;
  dn_array_t<stbrp_node> nodes;
  dn_array_t<u32> buffer;

  bool need_async_build;
  bool need_async_load;
  bool need_gl_init;
  bool need_config_update;

  FileMonitor* file_monitor;

  void init();
  void deinit();
  void set_name(const char* name);
  RectPackId* find_pack_item(i32 id);
  void build_sync();
  void load_to_memory();
  bool is_dirty();
  void calc_hash_and_mod_time();  
  void build_from_config();
  void build_from_source();
  void write_to_config();
  void write_to_png();
  void load_to_gpu();
  void unload_from_gpu();
  void watch_files();
  void on_file_add(const char* file_path);
  void on_file_change(const char* file_path);
};
using image_iterator = std::function<void(const char*)>;

void init_texture_atlas();
void init_screenshots();
void update_textures();
Sprite* alloc_sprite();
Texture* alloc_texture();
dn_vector2_t* alloc_uvs();
void create_sprite(const char* id, const char* file_path);
void create_sprite(const char* id, unsigned char* data, i32 width, i32 height, i32 channels);
void create_sprite_ex(Sprite* sprite, const char* id, unsigned char* data, i32 width, i32 height, i32 channels);

std::mutex image_mutex;
std::mutex image_config_mutex;

typedef struct {
  dn_array_t<dn_vector2_t, 65536> uv_data;
  dn_array_t<Texture, 256> textures;
  dn_array_t<Sprite, 1024> sprites;
  dn_array_t<TextureAtlas, 64> atlases;
} dn_images_t;
dn_images_t dn_images;

DN_IMP void dn_images_init();

DN_API void take_screenshot();
DN_API void write_screenshot_to_png(const char* file_name);
