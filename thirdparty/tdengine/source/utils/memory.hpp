#ifndef DN_MEMORY_H
#define DN_MEMORY_H
typedef enum {
	DN_ALLOCATOR_MODE_ALLOC,
	DN_ALLOCATOR_MODE_FREE,
	DN_ALLOCATOR_MODE_RESIZE,
} dn_allocator_mode_t;

dn_typedef_fn(
	void*, 
	dn_alloc_fn_t, 
	dn_allocator_mode_t mode, u32 size, void* previous_allocation, void* userdata
);

struct dn_allocator_t {
	dn_alloc_fn_t on_alloc;
};

struct dn_bump_allocator_t : dn_allocator_t {
	u8* buffer;
	u32 capacity;
	u32 bytes_used;

	std::unordered_map<u32, u32> allocations;
};

struct dn_standard_allocator_t : dn_allocator_t {

};

struct dn_allocators_t {
	dn_standard_allocator_t standard;
	dn_bump_allocator_t bump;

	std::unordered_map<std::string, dn_allocator_t*> allocators;
};
dn_allocators_t dn_allocators;

DN_API void             dn_allocator_add(const char* name, dn_allocator_t* allocator);
DN_API dn_allocator_t*  dn_allocator_find(const char* name);
DN_API void*            dn_allocator_alloc(dn_allocator_t* allocator, u32 size);
DN_API void*            dn_allocator_realloc(dn_allocator_t* allocator, void* memory, u32 size);
DN_API void             dn_allocator_free(dn_allocator_t* allocator, void* buffer);
DN_IMP void             dn_bump_allocator_init(dn_bump_allocator_t* allocator, u32 capacity);
DN_IMP void             dn_bump_allocator_clear(dn_bump_allocator_t* allocator);
DN_IMP void*            dn_bump_allocator_on_alloc(dn_allocator_mode_t mode, u32 size, void* buffer, void* userdata);
DN_IMP void             dn_standard_allocator_init(dn_standard_allocator_t* allocator);
DN_IMP void*            dn_standard_allocator_on_alloc(dn_allocator_mode_t mode, u32 size, void* buffer, void* userdata);
DN_IMP void             dn_allocators_init();
DN_IMP void             dn_allocators_update();

namespace dn::allocator {
	template<typename T>
	T* alloc(dn_allocator_t* allocator, u32 size);

	template<typename T>
	T* realloc(dn_allocator_t* allocator, u32 count, T* buffer);

	template<typename T>
	void free(dn_allocator_t* allocator, T* buffer);

	template<typename T>
	Array<T> alloc_array(u32 size);
}
#endif



#ifdef DN_MEMORY_IMPLEMENTATION
namespace dn::allocator {
	template<typename T>
	T* alloc(dn_allocator_t* allocator, u32 count) {
		return (T*)dn_allocator_alloc(allocator, count * sizeof(T));
	}

	template<typename T>
	T* realloc(dn_allocator_t* allocator, T* buffer, u32 count) {
		return (T*)dn_allocator_realloc(allocator, (void*)buffer, count * sizeof(T));
	}

	template<typename T>
	void free(dn_allocator_t* allocator, T* buffer) {
		return dn_allocator_free(allocator, (void*)buffer);
	}

	template<typename T>
	Array<T> alloc_array(dn_allocator_t* allocator, u32 size) {
		Array<T> array;
		array.data = alloc<T>(allocator, size);
		array.size = 0;
		array.capacity = size;

		return array;
	}
}

////////////////////
// BUMP ALLOCATOR //
////////////////////
void dn_bump_allocator_init(dn_bump_allocator_t* allocator, u32 capacity) {
	allocator->buffer = dn::allocator::alloc<u8>(&dn_allocators.standard, capacity);
	allocator->capacity = capacity;
	allocator->on_alloc = &dn_bump_allocator_on_alloc;
}

void dn_bump_allocator_clear(dn_bump_allocator_t* allocator) {
	std::memset(allocator->buffer, 0, allocator->bytes_used);
	allocator->bytes_used = 0;
	allocator->allocations.clear();
}

void* dn_bump_allocator_on_alloc(dn_allocator_mode_t mode, u32 size, void* old_memory, void* userdata) {
	dn_bump_allocator_t* allocator = (dn_bump_allocator_t*)userdata;
	if (mode == DN_ALLOCATOR_MODE_ALLOC) {
		DN_ASSERT(allocator->bytes_used + size <= allocator->capacity);

		auto memory_block = allocator->buffer + allocator->bytes_used;
		allocator->allocations[allocator->bytes_used] = size;
		allocator->bytes_used += size;
	
		return memory_block;
	}
	else if (mode == DN_ALLOCATOR_MODE_FREE) {
		return nullptr;
	}
	else if (mode == DN_ALLOCATOR_MODE_RESIZE) {
		if (!old_memory) {
			return allocator->on_alloc(DN_ALLOCATOR_MODE_ALLOC, size, nullptr, allocator);
		}

		auto offset = (u32)((u8*)old_memory - (u8*)allocator->buffer);
		auto old_size = allocator->allocations[offset];
		if (old_size >= size) {
			return old_memory;
		} 

		auto memory_block = allocator->on_alloc(DN_ALLOCATOR_MODE_ALLOC, size, nullptr, allocator);
		dn_os_memory_copy(old_memory, memory_block, size);
		return memory_block;
	}

	assert(false);
	return nullptr;
}

void dn_standard_allocator_init(dn_standard_allocator_t* allocator) {
	allocator->on_alloc = &dn_standard_allocator_on_alloc;
}

void* dn_standard_allocator_on_alloc(dn_allocator_mode_t mode, u32 size, void* old_memory, void* userdata) {
	auto allocator = (dn_standard_allocator_t*)userdata;
	if (mode == DN_ALLOCATOR_MODE_ALLOC) {
		return calloc(size, 1);
	}
	else if (mode == DN_ALLOCATOR_MODE_FREE) {
		::free(old_memory);
		return nullptr;
	}
	else if (mode == DN_ALLOCATOR_MODE_RESIZE) {
		return ::realloc(old_memory, size);
	}

	DN_ASSERT(false);
	return nullptr;
}

void dn_allocator_add(const char* name, dn_allocator_t* allocator) {
	if (!allocator) return;

	dn_allocators.allocators[name] = allocator;
}

dn_allocator_t* dn_allocator_find(const char* name) {
	if (!name) return nullptr;

	if (!dn_allocators.allocators.contains(name)) {
		dn_log("Tried to find allocator, but name was not registered; name = %s", name);
		return nullptr;
	}

	return dn_allocators.allocators[name];
}

void* dn_allocator_alloc(dn_allocator_t* allocator, u32 size) {
	DN_ASSERT(allocator);
	return allocator->on_alloc(DN_ALLOCATOR_MODE_ALLOC, size, NULL, allocator);
}

void* dn_allocator_realloc(dn_allocator_t* allocator, void* memory, u32 size) {
	DN_ASSERT(allocator);
	return allocator->on_alloc(DN_ALLOCATOR_MODE_RESIZE, size, memory, allocator);
}

void dn_allocator_free(dn_allocator_t* allocator, void* buffer) {
	DN_ASSERT(allocator);
	DN_ASSERT(buffer);
	allocator->on_alloc(DN_ALLOCATOR_MODE_FREE, 0, buffer, allocator);
}

void dn_allocators_init() {
	dn_standard_allocator_init(&dn_allocators.standard);
	dn_bump_allocator_init(&dn_allocators.bump, 50 * 1024 * 1024);

	dn_allocator_add("bump", &dn_allocators.bump);
	dn_allocator_add("standard", &dn_allocators.standard);
}

void dn_allocators_update() {
	dn_bump_allocator_clear(&dn_allocators.bump);
}
#endif