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

struct BinaryAssets {
    uint8_t* data;
    uint32_t size;
};
BinaryAssets binary_assets;

void init_assets();
