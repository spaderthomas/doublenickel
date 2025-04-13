#ifndef DN_IMAGE_H
#define DN_IMAGE_H

struct dn_texture_t {
  dn_asset_name_t name;
  u32 handle;
  u32 width;
  u32 height;
  u32 channels;
};


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

DN_API Texture* dn_images_find_texture_by_name(dn_string_t name);
DN_API Texture* dn_images_find_texture_by_hash(dn_hash_t hash);

struct Sprite {
  char file_path [DN_MAX_PATH_LEN];
  dn_hash_t hash;
  dn_hash_t texture;
  dn_vector2_t* uv;
  dn_vector2i_t size;
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
  dn_dynamic_array<dn_string_t> files;
} dn_atlas_bucket_t;

typedef struct {
  dn_string_t* dirs;
  u32 num_dirs;
} dn_image_config_t;

typedef struct {
  dn_array_t<dn_vector2_t, 65536> uv_data;
  dn_array_t<Texture, 256> textures;
  dn_array_t<Sprite, 1024> sprites;
  dn_array_t<TextureAtlas, 64> atlases;

  dn_fixed_array<dn_atlas_bucket_t, 16> buckets;
} dn_images_t;
dn_images_t dn_images;

DN_IMP void dn_images_init(dn_image_config_t config);
DN_IMP void dn_atlas_bucket_init(dn_atlas_bucket_t* bucket);
DN_IMP u32  dn_atlas_calc_bucket(dn_string_t path);

DN_API void take_screenshot();
DN_API void write_screenshot_to_png(const char* file_name);
#endif

#ifdef DN_IMAGE_IMPLEMENTATION
void dn_atlas_bucket_init(dn_atlas_bucket_t* bucket) {
  dn::dynamic_array::init(&bucket->files, &dn_allocators.bump);
}

u32 dn_atlas_calc_bucket(dn_string_t path) {
  dn_hash_t hash = dn_hash_string(path);
  return hash % dn_images.buckets.capacity;
}

void dn_images_init(dn_image_config_t config) {
  dn_array_init(&dn_images.uv_data);
  dn_array_init(&dn_images.textures);
  dn_array_init(&dn_images.sprites);
  
  dn_array_init(&dn_images.atlases);dn::fixed_array::init(&dn_images.buckets, &dn_allocators.bump);
  dn_for(i, dn_images.buckets.capacity) {
    dn_atlas_bucket_t* bucket = dn::fixed_array::reserve(&dn_images.buckets, 1);
    dn_atlas_bucket_init(bucket);
  }

  dn_for(path_index, config.num_dirs) {
    dn_string_t path = config.dirs[path_index];
    auto files = dn_os_scan_directory_recursive(path);
    dn_for(file_index, files.count) {
      dn_os_directory_entry_t file = files.data[file_index];
      dn_log("loading image; file = %s, bucket = %d", dn_string_to_cstr(file.file_path), dn_atlas_calc_bucket(file.file_path));
    }
  }
}

void dn_atlas_build(dn_string_t path) {
  dn_os_directory_entry_list_t entries = dn_os_scan_directory_recursive(path);
}

///////////////////
// TEXTURE ATLAS //
///////////////////
void TextureAtlas::init() {
  dn_array_init(&directories, MAX_DIRECTORIES_PER_ATLAS);

  texture = alloc_texture();
  texture->init(TEXTURE_ATLAS_SIZE, TEXTURE_ATLAS_SIZE, 4);
  
  auto events = DN_FILE_CHANGE_EVENT_ADDED | DN_FILE_CHANGE_EVENT_MODIFIED;
  auto on_file_event = [](FileMonitor* monitor, FileChange* event, void* userdata) {
    auto atlas = (TextureAtlas*)userdata;

    // If an image is removed, all of the still-existing images are still in the atlas
    if (event->events & DN_FILE_CHANGE_EVENT_REMOVED) {
      return;
    }

    atlas->need_async_build = true;
    atlas->need_async_load = true;
    
    AssetLoadRequest request;
    request.kind = AssetKind::TextureAtlas;
    request.atlas = atlas;
    asset_loader.submit(request);
  };

  this->file_monitor = dn_file_monitors_add();
  this->file_monitor->init(on_file_event, events, this);
}

void TextureAtlas::set_name(const char* name) {
  strncpy(this->name, name, 64);
  texture->hash = dn_hash_cstr_dumb(name);
}

RectPackId* TextureAtlas::find_pack_item(i32 id) {
  dn_array_for(ids, item) {
    if (item->id == id) return item;
  }
  return nullptr;
}

bool TextureAtlas::is_dirty() {
  bool dirty = false;
  #ifdef DN_EDITOR
  dirty |= mod_time > cfg_mod_time;
  dirty |= files_hash != cfg_files_hash;

  // If, for some reason, the resulting PNG does not exist on disk, then we sure had better
  // rebuild the atlas! This is also an easy way to manually force the atlas to rebuild.
  auto output_path = dn_paths_resolve_format("dn_atlas", name);
  dirty |= !std::filesystem::exists(output_path);
  #endif

  return dirty;
}

void TextureAtlas::load_to_memory() {
  // Load the atlas PNG into the GPU
  auto file_path = dn_paths_resolve_format("dn_atlas", name);
  
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
        dn_path_normalize_std(next);
        check_directory(next.c_str());
      } else {
        auto file_name = it->path().filename().string();
        auto file_path = it->path().string();
        dn_path_normalize_std(file_path);

        // 1: We hash all the names of the files, so new files will trigger regardless of their modtime.
        dn_hash_t file_name_hash = dn_hash_cstr_dumb(file_name.c_str());
        files_hash = files_hash ^ file_name_hash;

        // 2: We check modtime for files that exist
        // auto this_file_mod_time = dn_os_file_mod_time(file_path.c_str());
        auto this_file_mod_time = 0;
        if (this_file_mod_time > mod_time) {
          mod_time = this_file_mod_time;
        }
      }
    }
  };

  dn_array_for(directories, directory) {
    check_directory(*directory);
  }

  // Lua doesn't like the precision needed for a full 64 bits, so just take the bottom half.
  files_hash = files_hash >> 32;
}

void TextureAtlas::build_from_config() {
  this->buffer = dn_array_t<u32>();
  dn_array_init(&this->buffer, TEXTURE_ATLAS_SIZE * TEXTURE_ATLAS_SIZE);

  lua_State* l = dn_lua.state;
  // Push global texture data onto the stack
  lua_getglobal(l, "doublenickel");
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
    lua_State* l = dn_lua.state;
    
    if (!std::filesystem::exists(directory)) return;

    for (directory_iterator it(directory); it != directory_iterator(); ++it) {
      auto path = it->path().string();
      dn_path_normalize_std(path);
      
      if (it->is_directory()) {
        add_directory(path.c_str());
      } else {
        auto file_name = it->path().filename().string();

        auto sprite = alloc_sprite();
        sprite->texture = this->texture->hash;
        sprite->hash = dn_hash_cstr_dumb(file_name.c_str());
        strncpy(sprite->file_path, file_name.c_str(), DN_MAX_PATH_LEN);

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
          dn_vector2_t& uv = sprite->uv[i];
            
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
      
  dn_array_for(directories, directory) {
    add_directory(*directory);
  }
}
  
void TextureAtlas::build_from_source() {
  dn_log("%s: rebuilding %s", __func__, this->name);

  this->rects = dn_array_t<stbrp_rect>();
  dn_array_init(&this->rects, MAX_IMAGES_PER_ATLAS);
  this->ids = dn_array_t<RectPackId>();
  dn_array_init(&this->ids, MAX_IMAGES_PER_ATLAS);
  this->nodes = dn_array_t<stbrp_node>();
  dn_array_init(&this->nodes, TEXTURE_ATLAS_SIZE);
  this->buffer = dn_array_t<u32>();
  dn_array_init(&this->buffer, TEXTURE_ATLAS_SIZE * TEXTURE_ATLAS_SIZE);

  constexpr int fudge_px = 5;
  constexpr float fudge_uv = fudge_px / (float)TEXTURE_ATLAS_SIZE; 
  
  // Add all the image files under this atlas' directories
  stbi_set_flip_vertically_on_load(false);

  image_iterator add_directory = [&](const char* directory) {
    if (!std::filesystem::exists(directory)) return;

    for (directory_iterator it(directory); it != directory_iterator(); ++it) {
      auto path = it->path().string();
      dn_path_normalize_std(path);
      
      if (it->is_directory()) {
        add_directory(path.c_str());
      } else {
        if (!dn_path_is_extension(path, dn_string_literal(".png"))) return;
        auto file_name = it->path().filename().string();

        auto sprite = find_sprite_no_default(file_name.c_str());
        if (!sprite) sprite = alloc_sprite();
        sprite->texture = this->texture->hash;
        sprite->hash = dn_hash_cstr_dumb(file_name.c_str());
        strncpy(sprite->file_path, file_name.c_str(), DN_MAX_PATH_LEN);

        auto id = dn_array_push(&ids);
        id->sprite = sprite;

        // @hack: Sometimes on hotload the file is open when this is hit, it fails to open the file, bad things happen.
        // So you just spin. Who cares?
        id->data = nullptr;
        while (id->data == nullptr) {
          id->data = (u32*)stbi_load(path.c_str(), &sprite->size.x, &sprite->size.y, &id->channels, 0);
        }

        auto rect = dn_array_push(&rects);
        rect->id = id->id;
        rect->w = sprite->size.x + fudge_px;
        rect->h = sprite->size.y + fudge_px;
      }
    }
  };
  
  dn_array_for(directories, directory) {
    add_directory(*directory);
  }

  // Pack the sprites
  stbrp_context stbrp;
  stbrp_init_target(&stbrp, TEXTURE_ATLAS_SIZE, TEXTURE_ATLAS_SIZE, nodes.data, nodes.capacity);
  stbrp_pack_rects(&stbrp, rects.data, rects.size);

  // Copy the sprites into the texture atlas
  dn_array_for(rects, rect) {
    auto item = find_pack_item(rect->id);
    auto size = item->sprite->size;

    // Generate UV coordinates
    f32 top    = rect->y            / (f32)TEXTURE_ATLAS_SIZE;
    f32 bottom = (rect->y + size.y) / (f32)TEXTURE_ATLAS_SIZE;
    f32 left   = rect->x            / (f32)TEXTURE_ATLAS_SIZE;
    f32 right  = (rect->x + size.x) / (f32)TEXTURE_ATLAS_SIZE;
      
    dn_vector2_t uv [6] = dn_quad_literal(top, bottom, left, right);
    memcpy(item->sprite->uv, uv, sizeof(dn_vector2_t) * 6);

    // Copy the sprite into the image buffer
    u32* image = item->data;
    u32* atlas = buffer.data + (TEXTURE_ATLAS_SIZE * rect->y) + (rect->x);

    dn_for(row, rect->h - fudge_px) {
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
  
  lua_State* l = dn_lua.state;

  // Push global texture data onto the stack
  lua_getglobal(l, "doublenickel");
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
      dn_path_normalize_std(path);
      
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
      
  dn_array_for(directories, directory) {
    write_directory(*directory);
  }

  // Write the newly-updated config to disk
  lua_getglobal(l, "doublenickel");
  lua_pushstring(l, "write_file_to_return_table");
  lua_gettable(l, -2);

  // First argument: config file path
  auto file_path = dn_paths_resolve("texture_info");
  lua_pushstring(l, file_path);

  // Second argument: config data
  lua_getglobal(l, "doublenickel");
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
  auto file_path = dn_paths_resolve_format_ex("dn_atlas", name, &dn_allocators.bump);
  stbi_write_png(file_path, TEXTURE_ATLAS_SIZE, TEXTURE_ATLAS_SIZE, 4, buffer.data, 0);
}

void TextureAtlas::load_to_gpu() {
  texture->load_to_gpu(buffer.data);
  dn_array_free(&ids);
  dn_array_free(&rects);
  dn_array_free(&nodes);
  dn_array_free(&buffer);
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
  lua_State* l = dn_lua.state;

  // Push global texture data onto the stack
  lua_getglobal(l, "doublenickel");
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
    auto atlas = dn_array_push(&dn_images.atlases);
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
      auto directory = dn_paths_resolve_format_ex("dn_image", subdirectory, &dn_allocators.standard);
      dn_array_push(&atlas->directories, directory);
      atlas->file_monitor->add_directory(directory);
    }
    lua_pop(l, 1);

    // Once we've loaded all the directories, we can iterate their contents to create the hash and modtime of the files
    // used by the atlas.
    atlas->calc_hash_and_mod_time();
  }

  lua_pop(l, 1); // pop data.atlases
  return;
  // Load texture atlases; most are loaded async, except for a few important early ones
  dn_array_for(dn_images.atlases, atlas) {
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
  dn_defer{ free(data); };
  
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
  texture->width = width;
  texture->height = height;
  texture->channels = channels;
  texture->hash = dn_hash_cstr_dumb(id);
  texture->load_to_gpu((u32*)data);
  
  sprite->hash = dn_hash_cstr_dumb(id);
  sprite->texture = texture->hash;
  sprite->size = dn_vector2i_t(width, height);
  strncpy(sprite->file_path, id, DN_MAX_PATH_LEN);

  dn_vector2_t uv [6] = dn_quad_literal(0, 1, 0, 1);
  memcpy(sprite->uv, uv, sizeof(dn_vector2_t) * 6);
}

/////////////////
// SCREENSHOTS //
/////////////////

void init_screenshots() {
  DN_BROKEN();
  auto screenshots = dn_paths_resolve("screenshots");
  if (!std::filesystem::exists(screenshots)) return;
  for (directory_iterator it(screenshots); it != directory_iterator(); ++it) {
    auto path = it->path().string();
    dn_path_normalize_std(path);
    if (!dn_path_is_extension(path, dn_string_literal(".png"))) continue;

    // Load the data
    i32 width;
    i32 height;
    i32 channels;
    stbi_set_flip_vertically_on_load(false);
    unsigned char* data = (unsigned char*)stbi_load(path.c_str(), &width, &height, &channels, 0);
    dn_defer { free(data); };

    // Create a Sprite using the file name as the ID
    auto file_name = it->path().filename().string();
    create_sprite(file_name.c_str(), data, width, height, channels);
  }
}


/////////////
// BUFFERS //
/////////////
Texture* dn_images_find_texture_by_name(dn_string_t name) {
  auto hash = dn_hash_str_dumb(name);
  return dn_images_find_texture_by_hash(hash);
}

Texture* dn_images_find_texture_by_hash(dn_hash_t hash) {
  std::lock_guard lock(image_mutex);
  
  dn_array_for(dn_images.textures, image) {
    if (image->hash == hash) return image;
  }

  return nullptr;
}

Sprite* find_sprite_no_default(const char* name) {
  std::lock_guard lock(image_mutex);
  
  if (!name) return nullptr;

  auto hash = dn_hash_cstr_dumb(name);
  dn_array_for(dn_images.sprites, sprite) {
    if (sprite->hash == hash) return sprite;
  }
  
  return nullptr;
}

Sprite* find_sprite(const char* name) {
  auto sprite = find_sprite_no_default(name);
  
  if (sprite) return sprite;
  return find_sprite_no_default("debug.png");
}

dn_vector2_t* alloc_uvs_no_lock() {
  return dn_array_push(&dn_images.uv_data, dn_vector2_t(), 6);
}

dn_vector2_t* alloc_uvs() {
  std::lock_guard lock(image_mutex);
  return alloc_uvs_no_lock();
}

Texture* alloc_texture() {
  std::lock_guard lock(image_mutex);
  return dn_array_push(&dn_images.textures);
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
  auto sprite = dn_array_push(&dn_images.sprites);
  sprite->uv = alloc_uvs_no_lock();
  return sprite;
}

#endif
