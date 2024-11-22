///////////////////
// TEXTURE ATLAS //
///////////////////
void TextureAtlas::init() {
	arr_init(&directories, MAX_DIRECTORIES_PER_ATLAS);

	texture = alloc_texture();
	texture->init(TEXTURE_ATLAS_SIZE, TEXTURE_ATLAS_SIZE, 4);
	
	auto events = FileChangeEvent::Added | FileChangeEvent::Modified;
	auto on_file_event = [](FileMonitor* monitor, FileChange* event, void* userdata) {
		auto atlas = (TextureAtlas*)userdata;

		// If an image is removed, all of the still-existing images are still in the atlas
		if (enum_any(event->events & FileChangeEvent::Removed)) {
			return;
		}

		atlas->need_async_build = true;
		atlas->need_async_load = true;
		
		AssetLoadRequest request;
		request.kind = AssetKind::TextureAtlas;
		request.atlas = atlas;
		asset_loader.submit(request);
	};

	this->file_monitor = arr_push(&file_monitors);
	this->file_monitor->init(on_file_event, events, this);
}

void TextureAtlas::set_name(const char* name) {
	strncpy(this->name, name, 64);
	texture->hash = hash_label(name);
}

RectPackId* TextureAtlas::find_pack_item(i32 id) {
	arr_for(ids, item) {
		if (item->id == id) return item;
	}
	return nullptr;
}

bool TextureAtlas::is_dirty() {
	bool dirty = false;
	#ifdef FM_EDITOR
	dirty |= mod_time > cfg_mod_time;
	dirty |= files_hash != cfg_files_hash;

	// If, for some reason, the resulting PNG does not exist on disk, then we sure had better
	// rebuild the atlas! This is also an easy way to manually force the atlas to rebuild.
	auto output_path = resolve_format_path("atlas", name);
	dirty |= !std::filesystem::exists(output_path);
	#endif

	return dirty;
}

void TextureAtlas::load_to_memory() {
	// Load the atlas PNG into the GPU
	auto file_path = resolve_format_path("atlas", name);
	
	i32 width;
	i32 height;
	i32 channels;
	u32* data = (u32*)stbi_load(file_path, &width, &height, &channels, 0);
	memcpy(buffer.data, data, width * height * sizeof(u32));	

	stbi_image_free(data);

	need_gl_init = true;
}

void TextureAtlas::build_sync() {
}

void TextureAtlas::calc_hash_and_mod_time() {
	image_iterator check_directory = [&](const char* directory) {
		if (!std::filesystem::exists(directory)) return;
		
		for (directory_iterator it(directory); it != directory_iterator(); it++) {
			if (it->is_directory()) {
				auto next = it->path().string();
				normalize_path(next);
				check_directory(next.c_str());
			} else {
				auto file_name = it->path().filename().string();
				auto file_path = it->path().string();
				normalize_path(file_path);

				// 1: We hash all the names of the files, so new files will trigger regardless of their modtime.
				hash_t file_name_hash = hash_label(file_name.c_str());
				files_hash = files_hash ^ file_name_hash;

				// 2: We check modtime for files that exist
				auto this_file_mod_time = file_mod_time(file_path.c_str());
				if (this_file_mod_time > mod_time) {
					mod_time = this_file_mod_time;
				}
			}
		}
	};

	arr_for(directories, directory) {
		check_directory(*directory);
	}

	// Lua doesn't like the precision needed for a full 64 bits, so just take the bottom half.
	files_hash = files_hash >> 32;
}

void TextureAtlas::build_from_config() {
	this->buffer = Array<u32>();
	arr_init(&this->buffer, TEXTURE_ATLAS_SIZE * TEXTURE_ATLAS_SIZE);

	lua_State* l = get_lua().state;
	// Push global texture data onto the stack
	lua_getglobal(l, "tdengine");
	DEFER_POP(l);
	lua_pushstring(l, "texture");
	lua_gettable(l, -2);
	DEFER_POP(l);
	lua_pushstring(l, "data");
	lua_gettable(l, -2);
	DEFER_POP(l);
	lua_pushstring(l, "files");
	lua_gettable(l, -2);
	DEFER_POP(l);

	// Populate UVs and size for each sprite
	image_iterator add_directory = [&](const char* directory) {
		lua_State* l = get_lua().state;
		
		if (!std::filesystem::exists(directory)) return;

		for (directory_iterator it(directory); it != directory_iterator(); ++it) {
			auto path = it->path().string();
			normalize_path(path);
			
			if (it->is_directory()) {
				add_directory(path.c_str());
			} else {
				auto file_name = it->path().filename().string();

				auto sprite = alloc_sprite();
				sprite->texture = this->texture->hash;
				sprite->hash = hash_label(file_name.c_str());
				strncpy(sprite->file_path, file_name.c_str(), MAX_PATH_LEN);

				lua_pushstring(l, file_name.c_str());
				lua_gettable(l, -2);
				DEFER_POP(l);

				// Size
				lua_pushstring(l, "size");
				lua_gettable(l, -2);
				
				lua_pushstring(l, "x");
				lua_gettable(l, -2);
				sprite->size.x = lua_tonumber(l, -1);
				lua_pop(l, 1);
				
				lua_pushstring(l, "y");
				lua_gettable(l, -2);
				sprite->size.y = lua_tonumber(l, -1);
				lua_pop(l, 1);

				lua_pop(l, 1);

				// UV
				lua_pushstring(l, "uv");
				lua_gettable(l, -2);
				DEFER_POP(l);
					
				for (i32 i = 0; i < 6; i++) {
					Vector2& uv = sprite->uv[i];
						
					lua_pushnumber(l, i * 2 + 1);
					lua_gettable(l, -2);
					uv.x = lua_tonumber(l, -1);
					lua_pop(l, 1);
						
					lua_pushnumber(l, i * 2 + 2);
					lua_gettable(l, -2);
					uv.y = lua_tonumber(l, -1);
					lua_pop(l, 1);
				}
			}
		}
	};
			
	arr_for(directories, directory) {
		add_directory(*directory);
	}
}
	
void TextureAtlas::build_from_source() {
	tdns_log.write("%s: rebuilding %s", __func__, this->name);

	this->rects = Array<stbrp_rect>();
	arr_init(&this->rects, MAX_IMAGES_PER_ATLAS);
	this->ids = Array<RectPackId>();
	arr_init(&this->ids, MAX_IMAGES_PER_ATLAS);
	this->nodes = Array<stbrp_node>();
	arr_init(&this->nodes, TEXTURE_ATLAS_SIZE);
	this->buffer = Array<u32>();
	arr_init(&this->buffer, TEXTURE_ATLAS_SIZE * TEXTURE_ATLAS_SIZE);

	constexpr int fudge_px = 5;
	constexpr float fudge_uv = fudge_px / (float)TEXTURE_ATLAS_SIZE; 
	
	// Add all the image files under this atlas' directories
	stbi_set_flip_vertically_on_load(false);

	image_iterator add_directory = [&](const char* directory) {
		if (!std::filesystem::exists(directory)) return;

		for (directory_iterator it(directory); it != directory_iterator(); ++it) {
			auto path = it->path().string();
			normalize_path(path);
			
			if (it->is_directory()) {
				add_directory(path.c_str());
			} else {
				if (!is_png(path)) continue;
				auto file_name = it->path().filename().string();

				auto sprite = find_sprite_no_default(file_name.c_str());
				if (!sprite) sprite = alloc_sprite();
				sprite->texture = this->texture->hash;
				sprite->hash = hash_label(file_name.c_str());
				strncpy(sprite->file_path, file_name.c_str(), MAX_PATH_LEN);

				auto id = arr_push(&ids);
				id->sprite = sprite;

				// @hack: Sometimes on hotload the file is open when this is hit, it fails to open the file, bad things happen.
				// So you just spin. Who cares?
				id->data = nullptr;
				while (id->data == nullptr) {
					id->data = (u32*)stbi_load(path.c_str(), &sprite->size.x, &sprite->size.y, &id->channels, 0);
				}

				auto rect = arr_push(&rects);
				rect->id = id->id;
				rect->w = sprite->size.x + fudge_px;
				rect->h = sprite->size.y + fudge_px;
			}
		}
	};
	
	arr_for(directories, directory) {
		add_directory(*directory);
	}

	// Pack the sprites
	stbrp_context stbrp;
	stbrp_init_target(&stbrp, TEXTURE_ATLAS_SIZE, TEXTURE_ATLAS_SIZE, nodes.data, nodes.capacity);
	stbrp_pack_rects(&stbrp, rects.data, rects.size);

	// Copy the sprites into the texture atlas
	arr_for(rects, rect) {
		auto item = find_pack_item(rect->id);
		auto size = item->sprite->size;

		// Generate UV coordinates
		float32 top    = rect->y            / (float32)TEXTURE_ATLAS_SIZE;
		float32 bottom = (rect->y + size.y) / (float32)TEXTURE_ATLAS_SIZE;
		float32 left   = rect->x            / (float32)TEXTURE_ATLAS_SIZE;
		float32 right  = (rect->x + size.x) / (float32)TEXTURE_ATLAS_SIZE;
			
		Vector2 uv [6] = TD_MAKE_QUAD(top, bottom, left, right);
		memcpy(item->sprite->uv, uv, sizeof(Vector2) * 6);

		// Copy the sprite into the image buffer
		u32* image = item->data;
		u32* atlas = buffer.data + (TEXTURE_ATLAS_SIZE * rect->y) + (rect->x);

		fox_for(row, rect->h - fudge_px) {
			memcpy(atlas, image, size.x * sizeof(u32));
			atlas += TEXTURE_ATLAS_SIZE;
			image += size.x;
		}
	}

	// Save the PNG to disk
	write_to_png();

	// GL and Lua have to be done on the main thread, so signal for that to happen there
	need_gl_init = true;
	need_config_update = true;
}

void TextureAtlas::write_to_config() {
	// Because we load texture atlases asynchronously, we can be reading the config on the main thread while the
	// atlas loader thread also tries to serialize to the same config. 
	std::lock_guard lock(image_config_mutex);
	
	lua_State* l = get_lua().state;

	// Push global texture data onto the stack
	lua_getglobal(l, "tdengine");
	DEFER_POP(l);
	lua_pushstring(l, "texture");
	lua_gettable(l, -2);
	DEFER_POP(l);
	lua_pushstring(l, "data");
	lua_gettable(l, -2);
	DEFER_POP(l);

	// Get the table for this texture's data
	lua_pushstring(l, "atlases");
	lua_gettable(l, -2);

	lua_pushstring(l, this->name);
	lua_gettable(l, -2);
		
	// Update the modtime and hash with what we found in the sources
	lua_pushstring(l, "mod_time");
	lua_pushnumber(l, this->mod_time);
	lua_settable(l, -3);
		
	lua_pushstring(l, "hash");
	lua_pushnumber(l, this->files_hash);
	lua_settable(l, -3);

	lua_pop(l, 2);

	// Update the UVs for each file
	lua_pushstring(l, "files");
	lua_gettable(l, -2);
	DEFER_POP(l);

	image_iterator write_directory = [&](const char* directory) {
		if (!std::filesystem::exists(directory)) return; 

		// Go through each sprite, register it in the asset table, and collect its rect data
		for (directory_iterator it(directory); it != directory_iterator(); ++it) {
			auto path = it->path().string();
			normalize_path(path);
			
			if (it->is_directory()) {
				write_directory(path.c_str());
			} else {
				auto file_name = it->path().filename().string();

				// Create a new table for this file's data
				lua_newtable(l);

				lua_pushstring(l, "atlas");
				lua_pushstring(l, this->name);
				lua_settable(l, -3);

				auto sprite = find_sprite(file_name.c_str());
				
				// Size
				lua_newtable(l);
				lua_pushstring(l, "x");
				lua_pushnumber(l, sprite->size.x);
				lua_settable(l, -3);
				lua_pushstring(l, "y");
				lua_pushnumber(l, sprite->size.y);
				lua_settable(l, -3);

				// files -> file_data -> "size" -> size_data
				lua_pushstring(l, "size");
				lua_insert(l, -2);
				lua_settable(l, -3);

				// Build the UV table
				lua_newtable(l);
							   
				auto uvs = sprite->uv;
				for (int i = 0; i < 6; i++) {
					const auto& uv = uvs[i];
					lua_pushnumber(l, i * 2 + 1);
					lua_pushnumber(l, uv.x);
					lua_settable(l, -3);
					lua_pushnumber(l, i * 2 + 2);
					lua_pushnumber(l, uv.y);
					lua_settable(l, -3);
				}

				// files -> file_data -> "uv" -> uv_data
				lua_pushstring(l, "uv");
				lua_insert(l, -2);
				lua_settable(l, -3);

				// files -> file_name -> file_data
				lua_pushstring(l, file_name.c_str());
				lua_insert(l, -2);
				lua_settable(l, -3);
			}
		}
	};
			
	arr_for(directories, directory) {
		write_directory(*directory);
	}

	// Write the newly-updated config to disk
	lua_getglobal(l, "tdengine");
	lua_pushstring(l, "write_file_to_return_table");
	lua_gettable(l, -2);

	// First argument: config file path
	auto file_path = resolve_named_path("texture_info");
	lua_pushstring(l, file_path);

	// Second argument: config data
	lua_getglobal(l, "tdengine");
	DEFER_POP(l);
	lua_pushstring(l, "texture");
	lua_gettable(l, -2);
	lua_pushstring(l, "data");
	lua_gettable(l, -2);
	lua_insert(l, -3);
	lua_pop(l, 2);

	// Third argument: pretty
	lua_pushboolean(l, true);

	lua_pcall(l, 3, 0, 0);
}

void TextureAtlas::write_to_png() {
	auto file_path = resolve_format_path_ex("atlas", name, &standard_allocator);
	stbi_write_png(file_path, TEXTURE_ATLAS_SIZE, TEXTURE_ATLAS_SIZE, 4, buffer.data, 0);
	
	standard_allocator.free(file_path);
}

void TextureAtlas::load_to_gpu() {
	texture->load_to_gpu(buffer.data);
	arr_free(&ids);
	arr_free(&rects);
	arr_free(&nodes);
	arr_free(&buffer);
}


//
// TEXTURE
// 
void Texture::init(i32 width, i32 height, i32 channels) {
	this->width = width;
	this->height = height;
	this->channels = channels;
}

void Texture::load_to_gpu(u32* data) {
	// When we reload assets, instead of allocating a new Texture it's simpler to reuse the object
	// that already exists; that way, there are no lifetime issues whatsoever. But, to that end,
	// when a Texture is reloaded, it's best to ensure the GL resources are cleaned up properly.
	if (handle) unload_from_gpu();

	glGenTextures(1, &handle);
	glActiveTexture(GL_TEXTURE0); // Note: not all graphics drivers have default 0!
	glBindTexture(GL_TEXTURE_2D, handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0,
				 GL_RGBA,
				 width, height, 0,
				 GL_RGBA, GL_UNSIGNED_BYTE,
				 data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::unload_from_gpu() {
	glDeleteTextures(1, &handle);
	handle = 0;
}


///////////////
// LIFECYCLE //
///////////////
void init_texture_atlas() {
	lua_State* l = get_lua().state;

	// Push global texture data onto the stack
	lua_getglobal(l, "tdengine");
	DEFER_POP(l);
	lua_pushstring(l, "texture");
	lua_gettable(l, -2);
	DEFER_POP(l);
	lua_pushstring(l, "data");
	lua_gettable(l, -2);
	DEFER_POP(l);
	
	// Parse the list of atlases into a C struct	
	lua_pushstring(l, "atlases");
	lua_gettable(l, -2);
	
	lua_pushnil(l);
	while (lua_next(l, -2)) {
		auto atlas = arr_push(&atlas_infos);
		atlas->init();

		// Mod time
		lua_pushstring(l, "mod_time");
		lua_gettable(l, -2);
		atlas->cfg_mod_time = lua_tonumber(l, -1);
		lua_pop(l, 1);

		// Hash
		lua_pushstring(l, "hash");
		lua_gettable(l, -2);
		atlas->cfg_files_hash = lua_tonumber(l, -1);
		lua_pop(l, 1);

		// Name
		lua_pushstring(l, "name");
		lua_gettable(l, -2);
		const char* name = lua_tostring(l, -1);
		atlas->set_name(name);
		lua_pop(l, 1);

		// Priority
		lua_pushstring(l, "high_priority");
		lua_gettable(l, -2);
		atlas->high_priority = lua_toboolean(l, -1);
		lua_pop(l, 1);
		
		// Directories
		lua_pushstring(l, "directories");
		lua_gettable(l, -2);
		DEFER_POP(l);

		lua_pushnil(l);
		while (lua_next(l, -2)) {
			DEFER_POP(l);

			auto subdirectory = lua_tostring(l, -1);
			auto directory = resolve_format_path_ex("image", subdirectory, &standard_allocator);
			arr_push(&atlas->directories, directory);
			atlas->file_monitor->add_directory(directory);
		}
		lua_pop(l, 1);

		// Once we've loaded all the directories, we can iterate their contents to create the hash and modtime of the files
		// used by the atlas.
		atlas->calc_hash_and_mod_time();
	}

	lua_pop(l, 1); // pop data.atlases

	// Load texture atlases; most are loaded async, except for a few important early ones
	arr_for(atlas_infos, atlas) {
		// Step 1: Ensure that the atlas is up-to-date with its source images and load it into memory
		if (atlas->is_dirty()) {
			// Dirty atlases need to be rebuilt from source (i.e. load every constituent PNG, pack them into an atlas,
			// then blit the images onto the atlas image). This is very expensive, so except for things we need immediately
			// we do this asynchronously.
			if (atlas->high_priority) {
				atlas->build_from_source();
			}
			else {
				atlas->need_async_build = true;
			}
		} else {
			// Clean atlases just need to load the saved UVs and such from the Lua config file, and then load the atlas
			// PNG into the memory and then the GPU. Once again, loading the PNG is slow, so we also do that async for
			// everything that we don't need close to startup.
			//
			// Lua, however, is single-threaded, so we also have to load the Lua config here on the main thread.
			atlas->build_from_config();
			
			if (atlas->high_priority) {
				atlas->load_to_memory();
			}
			else {
				atlas->need_async_load = true;
			}
		}

		// Step 2: Load the atlas into the GPU and update the config if necessary
		if (atlas->high_priority) {
			if (atlas->need_gl_init) {
				atlas->load_to_gpu();
                atlas->need_gl_init = false;
            }

            if (atlas->need_config_update) {
                atlas->write_to_config();
                atlas->need_config_update = false;
            }
		}

		// Once again, steps 1 and 2 are expensive. Most assets are loaded async; here's where we send the request
		// to the asset loader to do that.
		if (atlas->need_async_build || atlas->need_async_load) {
			AssetLoadRequest request;
			request.kind = AssetKind::TextureAtlas;
			request.atlas = atlas;
			asset_loader.submit(request);
		}
	}
}


void create_sprite(const char* id, const char* file_path) {
	i32 width, height, channels;
	u8* data = (u8*)stbi_load(file_path, &width, &height, &channels, 0);
	defer{ free(data); };
	
	return create_sprite(id, data, width, height, channels);
}

void create_sprite(const char* id, u8* data, i32 width, i32 height, i32 channels) {
	auto sprite = alloc_sprite();
	if (!sprite) return;

	create_sprite_ex(sprite, id, data, width, height, channels);
}

void create_sprite_ex(Sprite* sprite, const char* id, u8* data, i32 width, i32 height, i32 channels) {
	auto texture = alloc_texture();
	if (!texture) return;
	texture->init(width, height, channels);
	texture->hash = hash_label(id);
	texture->load_to_gpu((u32*)data);
	
	sprite->hash = hash_label(id);
	sprite->texture = texture->hash;
	sprite->size = Vector2I(width, height);
	strncpy(sprite->file_path, id, MAX_PATH_LEN);

	Vector2 uv [6] = TD_MAKE_QUAD(0, 1, 0, 1);
	memcpy(sprite->uv, uv, sizeof(Vector2) * 6);
}

/////////////////
// SCREENSHOTS //
/////////////////

void init_screenshots() {
	auto screenshots = resolve_named_path("screenshots");
	if (!std::filesystem::exists(screenshots)) return;
	for (directory_iterator it(screenshots); it != directory_iterator(); ++it) {
		auto path = it->path().string();
		normalize_path(path);
		if (!is_png(path)) continue;

		// Load the data
		i32 width;
		i32 height;
		i32 channels;
		stbi_set_flip_vertically_on_load(false);
		unsigned char* data = (unsigned char*)stbi_load(path.c_str(), &width, &height, &channels, 0);
		defer { free(data); };

		// Create a Sprite using the file name as the ID
		auto file_name = it->path().filename().string();
		create_sprite(file_name.c_str(), data, width, height, channels);
	}
}

// Fill the screenshot buffer with the last frame.
void take_screenshot() {
	#if 0
	i32 width = window.native_resolution.x;
	i32 height = window.native_resolution.y;
	i32 bytes_per_pixel = 4;
	i32 bytes_per_row = bytes_per_pixel * width;
	
	auto data = bump_allocator.alloc<u8>(width * height * bytes_per_pixel);
	glBindFramebuffer(GL_FRAMEBUFFER, scene_target->color_buffer);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

	// OpenGL reads it in vertically flipped; we could keep it like this and reverse the UVs and
	// ask STB to flip on write, but it's simpler for me to know that the image is right side up
	// from the moment we load it in.
    for (int row = 0; row < height; row++) {
		auto data_ptr = data + (row * bytes_per_row);
		auto screenshot_ptr = render.screenshot + (height - row - 1) * bytes_per_row;
        memcpy(screenshot_ptr, data_ptr, bytes_per_row);
    }
	#endif
}

// Take the current contents of the screenshot buffer and dump them to the screenshot directory underf
// the given filename. Since this is just used for save file previews, this is *not* an arbitrary file.
// It's somewhere in the predetermined directory where we keep preview screenshots.
void write_screenshot_to_png(const char* file_name) {
	i32 width = window.native_resolution.x;
	i32 height = window.native_resolution.y;
	i32 bytes_per_pixel = 4;
	i32 bytes_per_row = bytes_per_pixel * width;

	auto file_path = resolve_format_path("screenshot", file_name);

	// We'd like to use the preview in the current game session, so we need to create a texture for it
	// that is identifiable by the file name (i.e. a Sprite)
	create_sprite(file_name, render.screenshot, width, height, bytes_per_pixel);

	// Then, save it out so we can use it next time
	stbi_flip_vertically_on_write(false);
	stbi_write_png(file_path, width, height, bytes_per_pixel, render.screenshot, bytes_per_row);
}


/////////////
// BUFFERS //
/////////////
Texture* find_texture(const char* name) {
	auto hash = hash_label(name);
	return find_texture(hash);
}

u32 find_texture_handle(const char* name) {
	auto hash = hash_label(name);
	auto texture = find_texture(hash);

	if (!texture) return 0;
	return texture->handle;
}

Texture* find_texture(hash_t hash) {
	std::lock_guard lock(image_mutex);
	
	arr_for(image_infos, image) {
		if (image->hash == hash) return image;
	}

	return nullptr;
}

Sprite* find_sprite_no_default(const char* name) {
	std::lock_guard lock(image_mutex);
	
	if (!name) return nullptr;

	auto hash = hash_label(name);
	arr_for(sprite_infos, sprite) {
		if (sprite->hash == hash) return sprite;
	}
	
	return nullptr;
}

Sprite* find_sprite(const char* name) {
	auto sprite = find_sprite_no_default(name);
	
	if (sprite) return sprite;
	return find_sprite_no_default("debug.png");
}

Vector2* alloc_uvs_no_lock() {
	return arr_push(&tc_data, Vector2(), 6);
}

Vector2* alloc_uvs() {
	std::lock_guard lock(image_mutex);
	return alloc_uvs_no_lock();
}

Texture* alloc_texture() {
	std::lock_guard lock(image_mutex);
	return arr_push(&image_infos);
}

Sprite* alloc_sprite() {
	// I'm kind of sloppy with lifetimes and stuff here. Most sprites are loaded on a utility thread, so there's
	// a window between when a sprite has been allocated and when it's "ready" for use (i.e. it points to the correct)
	// texture and UVs). I think there's a smarter way to signal that this has happened; maybe some thread safe
	// flag. 
	//
	// But, for now, the only thing where it's *bad* if it's not initialized is the UVs. That's just because it's a
	// pointer. No texture is OK; we just do a texture lookup on an empty texture ID and get back some standin asset.
	// But trying to dereference a nullptr to UVs that don't exist is bad. That's why for a Sprite, I always allocate
	// UVs right off.
	//
	// This whole thing feels kind of convoluted and hard to reason about. It feels like dangerous C territory, where
	// your lifetimes get all tangled up and you have no way to reason about when a thing is valid or not. But it's not
	// worth rewriting a bunch of code just for this simple case; just a note for the future. 
	std::lock_guard lock(image_mutex);
	auto sprite = arr_push(&sprite_infos);
	sprite->uv = alloc_uvs_no_lock();
	return sprite;
}
