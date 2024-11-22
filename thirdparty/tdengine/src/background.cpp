void init_backgrounds() {
	auto& lua = get_lua();
	lua_State* l = get_lua().state;

	arr_init(&backgrounds, 64);
			
	lua_getglobal(l, "tdengine");
	DEFER_POP(l);
	lua_pushstring(l, "background");
	lua_gettable(l, -2);
	DEFER_POP(l);
	lua_pushstring(l, "data");
	lua_gettable(l, -2);
	DEFER_POP(l);
	
	lua_pushnil(l);
	while (lua_next(l, -2)) {
		DEFER_POP(l);

		auto background = arr_push(&backgrounds);
		background->init();
		background->load_paths();
		
		// Load the background's priority
		lua.parse_bool("high_priority", &background->high_priority);

		// Figure out whether the source has been modified since we tiled it
		lua.parse_float64("mod_time", &background->mod_time);
		background->filesystem_mod_time = file_mod_time(background->source_image_full_path);

		if (background->is_dirty()) {
			// The source image has been modified. We need to rebuild the background's tiles.
			if (background->high_priority) {
				background->build_from_source();
			}
			else {
				background->need_async_build = true;
			}
		}
		else {
			// Our current tiles are up to date. Pull the size of the background image so we can calculate
			// the position of each time.
			lua_pushstring(l, "size");
			lua_gettable(l, -2);
				
			lua_pushstring(l, "x");
			lua_gettable(l, -2);
			background->width = lua_tonumber(l, -1);
			lua_pop(l, 1);
				
			lua_pushstring(l, "y");
			lua_gettable(l, -2);
			background->height = lua_tonumber(l, -1);
			lua_pop(l, 1);

			lua_pop(l, 1);
			
			// Add the correct number of tile metadata.
			lua_pushstring(l, "tiles");
			lua_gettable(l, -2);
			DEFER_POP(l);

			lua_pushnil(l);
			while (lua_next(l, -2)) {
				DEFER_POP(l);
				background->add_tile();
			}
		}

		// Load the image data into the GPU (either on this thread or the asset thread)
		if (background->high_priority) {
			if (background->is_dirty()) {
                background->update_config();
            }

			background->load_tiles();
			background->load_to_gpu();
			background->deinit();
		}
		else {
			background->need_async_load = true;
		}

		// If any of the PNG loading or generation is marked to be done async, submit a request to the
		// asset thread.
		if (background->need_async_build || background->need_async_load) {		
			AssetLoadRequest request;
			request.kind = AssetKind::Background;
			request.background = background;

			asset_loader.submit(request);
		}
	}
}


void Background::init() {
	this->tiles = standard_allocator.alloc_array<char*>(64);
	this->tile_full_paths = standard_allocator.alloc_array<char*>(64);
	this->tile_positions = standard_allocator.alloc_array<Vector2I>(64);
	this->source_image = standard_allocator.alloc_path();
	this->tile_output_folder = standard_allocator.alloc_path();
}

void Background::deinit() {
	arr_for(tiles, tile_ptr) {
		char* tile = *tile_ptr;
		standard_allocator.free(tile);
	}

	arr_for(tile_full_paths, tile_ptr) {
		char* tile = *tile_ptr;
		standard_allocator.free(tile);
	}

	standard_allocator.free_array(&this->tiles);
	standard_allocator.free_array(&this->tile_full_paths);
	standard_allocator.free_array(&this->tile_positions);
	standard_allocator.free_array(&this->loaded_tiles);
	standard_allocator.free(this->source_image);
	standard_allocator.free(this->source_image_full_path);
	standard_allocator.free(this->tile_output_folder);
	standard_allocator.free(this->tile_output_full_path);
}

void Background::load_paths() {
	auto& lua = get_lua();
	auto l = get_lua().state;
		
	// Find the path to the source image, i.e. the full size background
	lua.parse_string("source", &source_image);
	this->source_image_full_path = resolve_format_path_ex("image", source_image, &standard_allocator);

	// Find the folder we're going to write tiles to
	lua.parse_string(-2, &tile_output_folder);
	this->tile_output_full_path = resolve_format_path_ex("atlas", tile_output_folder, &standard_allocator);
		
	// The name of the background is the folder we use for tiling
	name = tile_output_folder;
}

void Background::set_source_image_size(i32 width, i32 height) {
	this->width = width;
	this->height = height;
}

void Background::set_source_data(u32* data) {
	this->data = data;
}

bool Background::add_tile() {
	assert(width > 0);
	assert(height > 0);
		
	// Calculate where the next tile will be
	if (!tile_positions.size) {
		// If we just started, it's just (0, 0)
		arr_push(&tile_positions);
	} else {
		// Otherwise, advance the previous tile one column, and move to the next row if needed.
		Vector2I last_tile_position = *arr_back(&tile_positions);
		Vector2I tile_position = { 0, 0 };
		tile_position.x = last_tile_position.x + Background::TILE_SIZE;
		tile_position.y = last_tile_position.y;
		if (tile_position.x >= width) {
			tile_position.x = 0;
			tile_position.y += Background::TILE_SIZE;
		}

		// If it falls outside of the image, we're done
		if (tile_position.y >= height) {
			return false;
		} else {
			arr_push(&tile_positions, tile_position);
		}
	}
		
		
	char** tile = arr_push(&tiles);
	*tile = standard_allocator.alloc_path();
	snprintf(*tile, 256, "%s_%03lld.png", name, tiles.size);
		
	char** tile_full_path = arr_push(&tile_full_paths);
	*tile_full_path = standard_allocator.alloc_path();
	snprintf(*tile_full_path, MAX_PATH_LEN, "%s/%s", tile_output_full_path, *tile);

	return true;
}

void Background::add_tiles() {
	while(add_tile()) {}
}

void Background::build_from_source() {
	// Clean out old tiles 
	std::filesystem::remove_all(tile_output_full_path);
	std::filesystem::create_directories(tile_output_full_path);

	// Load the source image
	stbi_set_flip_vertically_on_load(false);
	u32* source_data = (u32*)stbi_load(source_image_full_path, &width, &height, &channels, 0);
	defer { free(source_data); };

	assert(channels == 4);

	set_source_image_size(width, height);
	add_tiles();

	constexpr u32 nthreads = 16;
	TileProcessor::current_tile = 0;
	Array<TileProcessor> tile_processors;
	arr_init(&tile_processors, nthreads, TileProcessor());

	// Use a thread per-tile, up to the maximum number of threads in the pool.
	auto use_threads = fox_min(tiles.size, nthreads);
	for (u32 i = 0; i < use_threads; i++) {
		auto tile_processor = tile_processors[i];
		tile_processor->init(source_data);
		tile_processor->thread = std::thread(&TileProcessor::process, tile_processor, this);
	}
	
	for (u32 i = 0; i < use_threads; i++) {
		auto tile_processor = tile_processors[i];
		tile_processor->thread.join();
		tile_processor->deinit();
	}
}

void Background::update_config() {
	lua_State* l = get_lua().state;

	lua_getglobal(l, "tdengine");
	DEFER_POP(l);
	lua_pushstring(l, "background");
	lua_gettable(l, -2);
	DEFER_POP(l);
	lua_pushstring(l, "data");
	lua_gettable(l, -2);
	DEFER_POP(l);
	lua_pushstring(l, name);
	lua_gettable(l, -2);
	DEFER_POP(l);

	// Write out the new tile data to disk
	lua_pushstring(l, "mod_time");
	lua_pushnumber(l, filesystem_mod_time);
	lua_settable(l, -3);

	// Clear the old tile names from the config
	lua_pushstring(l, "tiles");
	lua_newtable(l);
	lua_settable(l, -3);

	// Write each file's name to the config
	for (u32 i = 0; i < tiles.size; i++) {
		auto tile_file_name = *tiles[i];
		lua_pushstring(l, "tiles");
		lua_gettable(l, -2);
		DEFER_POP(l);
		lua_pushnumber(l, i + 1);
		lua_pushstring(l, tile_file_name);
		lua_settable(l, -3);
	}

	// Write size
	lua_newtable(l);
	lua_pushstring(l, "x");
	lua_pushnumber(l, width);
	lua_settable(l, -3);

	lua_pushstring(l, "y");
	lua_pushnumber(l, height);
	lua_settable(l, -3);

	lua_pushstring(l, "size");
	lua_insert(l, -2);
	lua_settable(l, -3);
	
	// Write the file to disk
	lua_getglobal(l, "tdengine");
	lua_pushstring(l, "write_file_to_return_table");
	lua_gettable(l, -2);

	auto background_info = resolve_named_path("background_info");
	lua_pushstring(l, background_info);

	lua_getglobal(l, "tdengine");
	DEFER_POP(l);
	lua_pushstring(l, "background");
	lua_gettable(l, -2);
	lua_pushstring(l, "data");
	lua_gettable(l, -2);
	lua_insert(l, -3);
	lua_pop(l, 2);

	lua_pushboolean(l, true);

	lua_pcall(l, 3, 0, 0);
}

void Background::load_tiles() {
	// At this point, the background is correctly tiled (i.e. the tile images exist on disk, and are more current
	// than the source image). Load the tiles into the GPU.
	//
	// This is definitely slower than it needs to be; when we regenerate tiles, we already have the source data.
	// But instead of loading it directly into the GPU, we write it to a PNG and then reload it here. Why? Well,
	// laziness. It's probably smarter to have LoadTileFromMemory(), and just call that in both the regenerate
	// case + after we load them from disk in the cache case. But to be honest, I really don't care right
	// now!

	// Make a completion queue that's the exact size we need. (If you had a vector, this would be a great time
	// to use a vector that allocates from temporary storage)

	loaded_tiles = standard_allocator.alloc_array<LoadedTile>(tiles.size);

	for (int tile_index = 0; tile_index < tiles.size; tile_index++) {
		// Pull tile data
		auto tile = *tiles[tile_index];
		auto tile_full_path = *tile_full_paths[tile_index];

		// Allocate any buffer resources
		auto texture = alloc_texture();
		texture->hash = hash_label(tile);
		stbi_set_flip_vertically_on_load(false);
		u32* data = (u32*)stbi_load(tile_full_path, &texture->width, &texture->height, &texture->channels, 0);
			
		// Create a sprite so the tile can be drawn with the draw_image() API
		auto sprite = alloc_sprite();
		strncpy(sprite->file_path, tile, MAX_PATH_LEN);
		sprite->texture = texture->hash;
		sprite->hash = texture->hash;
		sprite->size = { texture->width, texture->height };

		// Each tile spans the entire image, so use trivial UVs
		Vector2 uv [6] = TD_MAKE_QUAD(0, 1, 0, 1);
		for (u32 i = 0; i < 6; i++) sprite->uv[i] = uv[i];

		// Mark the data to be loaded to the GPU
		auto item = arr_push(&loaded_tiles);
		item->texture = texture;
		item->data = data;
	}
}

bool Background::is_dirty() {
	if (filesystem_mod_time > mod_time) return true;
	if (!std::filesystem::exists(tile_output_full_path)) return true;

	return false;
}


void Background::load_one_to_gpu() {
	auto tile = loaded_tiles[gpu_load_index++];
	tile->texture->load_to_gpu(tile->data);
	free(tile->data);

	if (gpu_load_index == loaded_tiles.size) {
		gpu_done = true;
	}
}

void Background::load_to_gpu() {
	gpu_ready = true;
	while (!gpu_done) {
		load_one_to_gpu();
	}
}


//
// TILE PROCESSOR
//
void TileProcessor::init(u32* source_data) {
	if (!tile_data) tile_data = standard_allocator.alloc<u32>(Background::TILE_SIZE * Background::TILE_SIZE);
	this->source_data = source_data;
}

void TileProcessor::deinit() {
	standard_allocator.free(tile_data);
	tile_data = nullptr;
	source_data = nullptr;
}

void TileProcessor::process(Background* background) {
	while (current_tile < background->tiles.size) {
		// Atomically get the next tile index, and pull that tile's data. I believe this could be an atomic
		// instead of needing a full mutex, but it doesn't matter.
		mutex.lock();
		u32 tile = current_tile++;
		mutex.unlock();

		Vector2I source_position = *background->tile_positions[tile];
		char*    tile_path       = *background->tile_full_paths[tile];
		memset(tile_data, 0, Background::TILE_SIZE * Background::TILE_SIZE * sizeof(u32));
			
		// Blit a single tile to its own texture
		u32 tile_offset = 0;
		u32 source_offset = (background->width * source_position.y) + source_position.x;
		u32 cols = fox_min(Background::TILE_SIZE, background->width - source_position.x);
		u32 rows = fox_min(Background::TILE_SIZE, background->height - source_position.y);
		for (u32 row = 0; row < rows; row++) {
			memcpy(tile_data + tile_offset, source_data + source_offset, cols * sizeof(u32));
			source_offset += background->width;
			tile_offset += Background::TILE_SIZE;
		}

		// Write the tile image out to a PNG file
		stbi_write_png(tile_path, Background::TILE_SIZE, Background::TILE_SIZE, 4, tile_data, 0);
	}
}
