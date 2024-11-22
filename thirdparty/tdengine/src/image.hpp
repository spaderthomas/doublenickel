struct Texture {
	hash_t hash;
	u32 handle;
	i32 width;
	i32 height;
	i32 channels;

	void init(i32 width, i32 height, i32 channels);
	void load_to_gpu(u32* data);
	void unload_from_gpu();
};
Texture* find_texture(const char* name);
Texture* find_texture(hash_t hash);
FM_LUA_EXPORT u32 find_texture_handle(const char* name);

struct Sprite {
	char file_path [MAX_PATH_LEN];
	hash_t hash;
	hash_t texture;
	Vector2* uv;
	Vector2I size;
};
Sprite* find_sprite(const char* name);
Sprite* find_sprite_no_default(const char* name);


struct Path {
	char path [MAX_PATH_LEN];
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
	hash_t cfg_files_hash;
	hash_t files_hash;
	bool high_priority;
	Array<string> directories;
	Texture* texture = nullptr;

	// Data used for generation. These are initialized to temporary storage, so
	// don't expect them to be valid in other cases.
	Array<RectPackId> ids;
	Array<stbrp_rect> rects;
	Array<stbrp_node> nodes;
	Array<u32> buffer;

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
Vector2* alloc_uvs();
void create_sprite(const char* id, const char* file_path);
void create_sprite(const char* id, unsigned char* data, i32 width, i32 height, i32 channels);
void create_sprite_ex(Sprite* sprite, const char* id, unsigned char* data, i32 width, i32 height, i32 channels);

std::mutex image_mutex;
std::mutex image_config_mutex;

FM_LUA_EXPORT void take_screenshot();
FM_LUA_EXPORT void write_screenshot_to_png(const char* file_name);
