typedef char* tstring;

enum class AllocatorMode : u32 {
	Allocate,
	Free,
	Resize,
};

typedef std::function<void*(AllocatorMode, u32, void*)> OnAllocate;


struct MemoryAllocator {
	OnAllocate on_alloc;

	template<typename T>
	T* alloc();

	template<typename T>
	void free(T* buffer);
	
	template<typename T>
	T* realloc(T* buffer, u32 size);

	template<typename T>
	T* alloc(u32 size);

	template<typename T>
	Array<T> alloc_array(u32 size);

	template<typename T>
	void free_array(Array<T>* array);

	
	char* alloc_path();
};
std::unordered_map<std::string, MemoryAllocator*> allocators;

FM_LUA_EXPORT void             ma_add(const char* name, MemoryAllocator* allocator);
FM_LUA_EXPORT MemoryAllocator* ma_find(const char* name);
FM_LUA_EXPORT void*            ma_alloc(MemoryAllocator* allocator, u32 size);
FM_LUA_EXPORT void*            ma_realloc(MemoryAllocator* allocator, void* memory, u32 size);
FM_LUA_EXPORT void             ma_free(MemoryAllocator* allocator, void* buffer);

struct BumpAllocator : MemoryAllocator {
	u8* buffer;
	u32 capacity;
	u32 bytes_used;

	std::unordered_map<u32, u32> allocations;

	void init(u32 size);
	void clear();
};
BumpAllocator bump_allocator;

struct DefaultAllocator : MemoryAllocator {
	void init();
};

DefaultAllocator standard_allocator;