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
    DN_ASSET_IMPORT_STATUS_RUNNING,
    DN_ASSET_IMPORT_STATUS_DONE,
} dn_asset_import_status_t;

typedef enum {
    DN_BACKGROUND_IMPORT_STATUS_UPDATE_CONFIG,
    DN_BACKGROUND_IMPORT_STATUS_LOAD_TILES,
    DN_BACKGROUND_IMPORT_STATUS_DONE,
} dn_background_import_status_t;
typedef struct {
    dn_background_import_status_t status;
    Background* background;
} dn_background_import_request_t;

typedef void(*dn_asset_import_fn)(dn_asset_import_request_t* request);
typedef dn_asset_import_status_t(*dn_asset_completion_fn)(dn_asset_import_request_t* request);

typedef struct {
    dn_asset_import_fn on_import;
    dn_asset_completion_fn on_complete;
} dn_asset_importer_t;

typedef void* dn_asset_data_t;

typedef struct {
    dn_asset_name_t name;
    dn_asset_data_t data;
} dn_asset_t;

typedef struct {
    std::thread thread;
    std::condition_variable condition;
    std::mutex mutex;

    std::unordered_map<std::string, dn_asset_t> assets;

    RingBuffer<AssetLoadRequest> load_requests;
    RingBuffer<AssetLoadRequest> completion_queue;
} dn_assets_t;
dn_assets_t dn_assets;


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

dn_asset_import_status_t dn_background_complete(dn_asset_import_request_t* request) {
    auto background_request = (dn_background_import_request_t*)request->user_data;
    auto background = background_request->background;

    switch (background_request->status) {
        case DN_BACKGROUND_IMPORT_STATUS_UPDATE_CONFIG: {
            if (background->is_dirty()) {
               background->update_config();
            }

            background_request->status = DN_BACKGROUND_IMPORT_STATUS_LOAD_TILES;
            return DN_ASSET_IMPORT_STATUS_RUNNING;
        } break;

        case DN_BACKGROUND_IMPORT_STATUS_LOAD_TILES: {
            background->load_one_to_gpu();
            if (background->gpu_done) {
                background_request->status = DN_BACKGROUND_IMPORT_STATUS_DONE;
                return DN_ASSET_IMPORT_STATUS_DONE;
            }

            return DN_ASSET_IMPORT_STATUS_RUNNING;
        } break;

        default: {
            DN_ASSERT(false);
            return DN_ASSET_IMPORT_STATUS_DONE;
        }
    }
}

void dn_assets_init() {
    dn_asset_importer_t background_importer = {
        .on_import = &dn_background_import,
        .on_complete = &dn_background_complete
    };
}

void dn_assets_add(const char* name, dn_asset_t asset) {
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

void init_assets();
