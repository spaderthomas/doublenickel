typedef enum {
	DN_ALLOCATOR_MODE_ALLOC,
	DN_ALLOCATOR_MODE_FREE,
	DN_ALLOCATOR_MODE_RESIZE,
} dn_allocator_mode_t;

typedef std::function<void*(dn_allocator_mode_t, u32, void*)> dn_alloc_fn_t;


struct  dn_allocator_t {
	dn_alloc_fn_t on_alloc;

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

typedef struct {
	dn_bump_allocator_t bump;
	dn_standard_allocator_t standard;
} dn_allocators_t;
dn_allocators_t dn_allocators;

DN_API void             dn_allocators_init();
DN_API void             dn_allocator_add(const char* name, dn_allocator_t* allocator);
DN_API dn_allocator_t*  dn_allocator_find(const char* name);
DN_API void*            dn_allocator_alloc(dn_allocator_t* allocator, u32 size);
DN_API void*            dn_allocator_realloc(dn_allocator_t* allocator, void* memory, u32 size);
DN_API void             dn_allocator_free(dn_allocator_t* allocator, void* buffer);

struct dn_bump_allocator_t : dn_allocator_t {
	u8* buffer;
	u32 capacity;
	u32 bytes_used;

	std::unordered_map<u32, u32> allocations;

	void init(u32 size);
	void clear();
};
dn_bump_allocator_t bump_allocator;

struct dn_standard_allocator_t : dn_allocator_t {
	void init();
};

dn_standard_allocator_t standard_allocator;