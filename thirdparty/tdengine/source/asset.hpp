#ifndef DN_ASSET_H
#define DN_ASSET_H
enum class AssetKind {
  Background,
  TextureAtlas
};

struct AssetLoadRequest {
  int32 id;
  static int32 next_id;

  AssetKind kind;
  union {
    Background* background;
    TextureAtlas* atlas;
  };
};
int32 AssetLoadRequest::next_id = 0;

/*
I used to load all assets for the game synchronously, at startup. This was OK for a while, but eventually the game 
accumulated enough big PNGs that starting the game would take twenty or thirty seconds on a good desktop. The code
I wrote to solve this lets the engine:
    1. Load assets asynchronously
    2. Mark some assets as high priority, to be loaded synchronously before the first frame

But! This isn't a very general purpose system. You don't use it, or any centralized system, to *fetch* assets. That
means that if you want to return some stand-in asset for the case where the asset you want isn't loaded yet, you
have to add that logic in whatever get_thing() function you write, and then ensure that the stand-in asset is loaded
high-priority.

I also only use this to load image data. All the other assets in the game are small enough to not impact startup time,
so they're just loaded sync. If you need to add more asset types to this, it should be straightforward:
    1. Add an entry to AssetKind, and an entry to the union in AssetLoadRequest. 
    2. Wherever you currently load the assets, you can instead submit an AssetLoadRequest. Usually, I just need a pointer
       to the asset in question, but you can add arbitrary stuff to the union.
    3. Handle the new asset type in AssetLoader::process_requests(). This is the worker thread.
    4. Push the request to a completion queue; a lot of things need to happen on the main thread with assets (for example,
       loading data to the GPU or using the Lua state). You can do those things in AssetLoader::process_completion_queue().
*/
struct AssetLoader {
  std::thread thread;
  std::condition_variable condition;
  std::mutex mutex;

  RingBuffer<AssetLoadRequest> load_requests;
  RingBuffer<AssetLoadRequest> completion_queue;

  void process_requests();
  void process_completion_queue();
  void submit(AssetLoadRequest request);
};
AssetLoader asset_loader;

typedef struct {
  void* user_data;
} dn_asset_import_request_t;

typedef enum {
  DN_ASSET_COMPLETION_STATUS_RUNNING,
  DN_ASSET_COMPLETION_STATUS_DONE,
} dn_asset_completion_status_t;

dn_typedef_fn(void,                         dn_asset_import_fn,     dn_asset_import_request_t* request);
dn_typedef_fn(dn_asset_completion_status_t, dn_asset_completion_fn, dn_asset_import_request_t* request);

typedef struct {
  dn_asset_name_t id;
  dn_asset_import_fn on_import;
  dn_asset_completion_fn on_complete;
} dn_asset_importer_t;

typedef void* dn_asset_data_t;

typedef struct {
    dn_asset_name_t name;
    dn_asset_data_t data;
} dn_asset_t;

typedef struct {
  struct {
    dn_asset_importer_t* data;
    u32 count;
  } importers;
} dn_asset_config_t;

//////////////
// INTERNAL //
//////////////
typedef enum {
  DN_BACKGROUND_COMPLETION_STATUS_UPDATE_CONFIG,
  DN_BACKGROUND_COMPLETION_STATUS_LOAD_TILES,
  DN_BACKGROUND_COMPLETION_STATUS_DONE,
} dn_background_completion_status_t;

typedef struct {
  dn_background_completion_status_t status;
  Background* background;
} dn_background_import_request_t;

typedef struct {
  std::thread thread;
  std::condition_variable condition;
  std::mutex mutex;

  std::unordered_map<std::string, dn_asset_t> assets;
  std::unordered_map<std::string, dn_asset_importer_t> importers;

  RingBuffer<AssetLoadRequest> load_requests;
  RingBuffer<AssetLoadRequest> completion_queue;
} dn_assets_t;
dn_assets_t dn_assets;


DN_API void dn_assets_init(dn_asset_config_t config);
DN_API dn_asset_data_t dn_assets_find(const char* name);
DN_API void dn_assets_add(const char* name, dn_asset_data_t data);
DN_API void dn_asset_copy_name(const char* source, dn_asset_name_t dest);

DN_IMP void dn_assets_update();
DN_IMP void dn_background_import(dn_asset_import_request_t* request);
DN_IMP dn_asset_completion_status_t dn_background_complete(dn_asset_import_request_t* request);
#endif

#ifdef DN_ASSET_IMPLEMENTATION
void dn_background_import(dn_asset_import_request_t* request) {
    auto background_request = (dn_background_import_request_t*)request->user_data;
    auto background = background_request->background;
    if (background->need_async_build) {
        background->build_from_source();
    }
    if (background->need_async_load) {
        background->load_tiles();
    }
}

dn_asset_completion_status_t dn_background_complete(dn_asset_import_request_t* request) {
    auto background_request = (dn_background_import_request_t*)request->user_data;
    auto background = background_request->background;

    switch (background_request->status) {
        case DN_BACKGROUND_COMPLETION_STATUS_UPDATE_CONFIG: {
            if (background->is_dirty()) {
               background->update_config();
            }

            background_request->status = DN_BACKGROUND_COMPLETION_STATUS_LOAD_TILES;
            return DN_ASSET_COMPLETION_STATUS_RUNNING;
        } break;

        case DN_BACKGROUND_COMPLETION_STATUS_LOAD_TILES: {
            background->load_one_to_gpu();
            if (background->gpu_done) {
                background_request->status = DN_BACKGROUND_COMPLETION_STATUS_DONE;
                return DN_ASSET_COMPLETION_STATUS_DONE;
            }

            return DN_ASSET_COMPLETION_STATUS_RUNNING;
        } break;

        default: {
            DN_ASSERT(false);
            return DN_ASSET_COMPLETION_STATUS_DONE;
        }
    }
}

void dn_assets_init(dn_asset_config_t user_config) {
  dn_log("%s", __func__);

  dn_asset_importer_t default_importers [] = {
    {
      .id = dn_type_name(Background),
      .on_import = &dn_background_import,
      .on_complete = &dn_background_complete
    }
  };

  dn_asset_config_t configs [] = {
    {
      .importers = {
        .data = default_importers,
        .count = DN_ARR_LEN(default_importers)
      }
    },
    user_config
  };
   
  dn_for_each(configs, config) {
    dn_for_each_n(config->importers.data, importer, config->importers.count) {
      dn_log("%s: Registered importer; id = %s, on_import = %d, on_complete = %d", 
        __func__, 
        importer->id, 
        importer->on_import, importer->on_complete
      );

      dn_assets.importers[importer->id] = *importer;
    }
  }
  

  rb_init(&asset_loader.load_requests, 2048);
  rb_init(&asset_loader.completion_queue, 2048);
  
  // asset_loader.thread = std::thread(&AssetLoader::process_requests, &asset_loader);
  // asset_loader.thread.detach();

}

void dn_assets_add(const char* name, dn_asset_data_t data) {
  dn_log("%s: name = %s, data = %p", __func__, name, data);
  dn_asset_t asset;
  dn_asset_copy_name(name, asset.name);
  asset.data = data;
  dn_assets.assets[name] = asset;
}

dn_asset_data_t dn_assets_find(const char* name) {
    DN_ASSERT(name);

    if (!dn_assets.assets.contains(name)) {
    dn_log("Tried to find asset, but name was not registered; name = %s", name);
        return NULL;
    }

    return dn_assets.assets[name].data;
}

void dn_asset_copy_name(const char* source, dn_asset_name_t dest) {
  dn_string_copy(source, dest, DN_ASSET_NAME_LEN);
}


void dn_assets_update() {
  asset_loader.process_completion_queue();

  dn_array_for(backgrounds, background) {
    if (background->gpu_ready && !background->gpu_done) {
      background->load_one_to_gpu();

      if (background->gpu_done) {
        background->deinit();
      }

      if (dn_engine_exceeded_frame_time()) break;
    }
  }
}

void AssetLoader::process_requests() {
  while (true) {
    std::unique_lock lock(mutex);

    condition.wait(lock, [this] {
      return load_requests.size > 0;
      });

    auto request = rb_pop(&load_requests);
    lock.unlock();

    if (request.kind == AssetKind::TextureAtlas) {
      auto atlas = request.atlas;
      if (atlas->need_async_build) {
        atlas->build_from_source();
      }
      if (atlas->need_async_load) {
        atlas->load_to_memory();
      }

      lock.lock();
      rb_push(&completion_queue, request);
      lock.unlock();

    }
    else if (request.kind == AssetKind::Background) {
      auto background = request.background;
      if (background->need_async_build) {
        background->build_from_source();
      }
      if (background->need_async_load) {
        background->load_tiles();
      }

      lock.lock();
      rb_push(&completion_queue, request);
      lock.unlock();
    }
  }
}

void AssetLoader::process_completion_queue() {
  int num_assets_loaded = 0;
  auto begin = glfwGetTime();
  while (true) {
    std::unique_lock lock(mutex);
    if (!completion_queue.size) {
      break;
    }

    auto completion = rb_pop(&completion_queue);
    lock.unlock();

    num_assets_loaded++;

    if (completion.kind == AssetKind::Background) {
      auto background = completion.background;
      dn_log_flags(DN_LOG_FLAG_FILE, "%s: AssetKind = Background, AssetName = %s", __func__, background->name);
      
      if (background->is_dirty()) {
        background->update_config();
      }

      background->gpu_ready = true;
    }
    else if (completion.kind == AssetKind::TextureAtlas) {
      auto atlas = completion.atlas;
      dn_log_flags(DN_LOG_FLAG_FILE, "%s: atlas, %s", __func__, atlas->name);
      
      if (atlas->need_gl_init) {
        atlas->load_to_gpu();
        atlas->need_gl_init = false;
      }

      if (atlas->need_config_update) {
        atlas->write_to_config();
        atlas->need_config_update = false;
      }
      
    }

    if (dn_engine_exceeded_frame_time()) break;
  }

  if (num_assets_loaded) {
    std::unique_lock lock(mutex);
    auto now = glfwGetTime();
    dn_log_flags(DN_LOG_FLAG_FILE,
             "AssetLoader: frame = %d, assets_loaded =  %d, assets_remaining = %d, time_ms =  %f",
             engine.frame,
             num_assets_loaded, completion_queue.size,
             (now - begin) * 1000);
  }
}


void AssetLoader::submit(AssetLoadRequest request) {
  request.id = AssetLoadRequest::next_id++;

  mutex.lock();
  rb_push(&load_requests, request);
  mutex.unlock();

  condition.notify_all();
}

#endif