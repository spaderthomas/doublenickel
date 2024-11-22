/*
  This is a simple, short-lived struct that we use to construct metadata for the tiles that make up
  a background. You can give it the size of the source image, and it will calculate the position
  and filename of each tile.

  The background tiling scheme is pretty simple. Start at tile 0, and advance by TILE_SIZE horizontally.
  When you reach the end of the image, move down by TILE_SIZE vertically and continue until you've
  tiled the whole images. At each tile, blit the corresponding subsection of the source image into
  a new image and save it in our atlas folder as "$BG_000.png" (&c. for subsequent tiles)
 */
struct Background {
	char* name;
	char* tile_output_folder;
	char* tile_output_full_path;
	char* source_image;
	char* source_image_full_path;
	Array<char*> tiles;
	Array<char*> tile_full_paths;
	Array<Vector2I> tile_positions;
	i32 width;
	i32 height;
	i32 channels;
	u32* data;
	bool high_priority = false;

	float64 mod_time;
	float64 filesystem_mod_time;

	bool need_async_build;
	bool need_async_load;
	bool need_config_update;

	struct LoadedTile {
		Texture* texture;
		u32* data;
	};
	Array<LoadedTile> loaded_tiles;
	bool gpu_ready = false;
	bool gpu_done = false;
	int gpu_load_index = 0;

	static constexpr i32 TILE_SIZE = 2048;

	void init();
	void deinit();
	void load_paths();
	void set_source_image_size(i32 width, i32 height);
	void set_source_data(u32* data);
	bool add_tile();
	void add_tiles();
	bool is_dirty();
	void build_from_source();
	void load_to_gpu();
	void load_one_to_gpu();
	void load_tiles();
	void update_config();
};


/*
  A small struct for parallelizing writing tiles out to PNG. 

  It uses synchronization very minimally; only to grab the next tile index for a given background. Each instance
  shares the source image data as read-only, and allocates a buffer from temporary storage once, on initialization,
  to store the tile data. It's important that each processor in the pool allocates once per-init instead of
  per-tile; Each tile is 4MB, so if we have even just fifty tiles, we exhaust temporary storage pretty fast.
 */
struct TileProcessor {
	u32* source_data;
	u32* tile_data;
	std::thread thread;
	
	static u32 current_tile;
	static std::mutex mutex;

	void init(u32* source_data);
	void deinit();
	void process(Background* background);
};
u32 TileProcessor::current_tile = 0;
std::mutex TileProcessor::mutex = std::mutex();


void init_backgrounds();
Array<Background> backgrounds;
