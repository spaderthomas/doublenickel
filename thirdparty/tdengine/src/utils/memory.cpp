////////////////////
// BASE ALLOCATOR //
////////////////////
template<typename T>
T* MemoryAllocator::alloc(u32 size) {
	return (T*)on_alloc(AllocatorMode::Allocate, size * sizeof(T), nullptr);
}

template<typename T>
void MemoryAllocator::free(T* buffer) {
	on_alloc(AllocatorMode::Free, 0, buffer);
}

template<typename T>
T* MemoryAllocator::realloc(T* buffer, u32 size) {
	return (T*)on_alloc(AllocatorMode::Resize, size, buffer);
}

template<typename T>
T* MemoryAllocator::alloc() {
	return alloc<T>(1);
}

template<typename T>
Array<T> MemoryAllocator::alloc_array(u32 size) {
	Array<T> array;
	array.data = alloc<T>(size);
	array.size = 0;
	array.capacity = size;

	return array;
}

template<typename T>
void MemoryAllocator::free_array(Array<T>* array) {
	this->free(array->data);
	array->data = nullptr;
	array->size = 0;
	array->capacity = 0;
}

char* MemoryAllocator::alloc_path() {
	return alloc<char>(MAX_PATH_LEN);
}



////////////////////
// BUMP ALLOCATOR //
////////////////////
void BumpAllocator::init(u32 capacity) {
	this->buffer = standard_allocator.alloc<u8>(capacity);
	this->capacity = capacity;
	
	on_alloc = [this](AllocatorMode mode, u32 size, void* old_memory) -> void* {
		if (mode == AllocatorMode::Allocate) {
			if (this->bytes_used + size > this->capacity) {
				assert(false);
			}

			auto memory_block = this->buffer + this->bytes_used;
			allocations[this->bytes_used] = size;
			this->bytes_used += size;
		
			return memory_block;
		}
		else if (mode == AllocatorMode::Free) {
			return nullptr;
		}
		else if (mode == AllocatorMode::Resize) {
			if (!old_memory) {
				return on_alloc(AllocatorMode::Allocate, size, nullptr);
			}
	
			auto offset = (u32)((u8*)old_memory - (u8*)this->buffer);
			auto old_size = allocations[offset];
			if (old_size >= size) {
				return old_memory;
			} 

			auto memory_block = on_alloc(AllocatorMode::Allocate, size, nullptr);
			copy_memory(old_memory, memory_block, size);
			return memory_block;
		}

		assert(false);
		return nullptr;
	};
}

void BumpAllocator::clear() {
	std::memset(buffer, 0, bytes_used);
	bytes_used = 0;
	allocations.clear();
}


///////////////////////
// DEFAULT ALLOCATOR //
///////////////////////
void DefaultAllocator::init() {
	on_alloc = [&](AllocatorMode mode, u32 size, void* old_memory) -> void* {
		if (mode == AllocatorMode::Allocate) {
			return calloc(size, 1);
		}
		else if (mode == AllocatorMode::Free) {
			::free(old_memory);
			return nullptr;
		}
		else if (mode == AllocatorMode::Resize) {
			return ::realloc(old_memory, size);
		}
	
		assert(false);
		return nullptr;
	};
}


// LUA API
void ma_add(const char* name, MemoryAllocator* allocator) {
	if (!allocator) return;

	allocators[name] = allocator;
}

MemoryAllocator* ma_find(const char* name) {
	if (!name) return nullptr;

	if (!allocators.contains(name)) {
		tdns_log.write("Tried to find allocator, but name was not registered; name = %s", name);
		return nullptr;
	}

	return allocators[name];
}

void* ma_alloc(MemoryAllocator* allocator, u32 size) {
	if (!allocator) return nullptr;

	auto buffer = allocator->alloc<u8>(size);
	return reinterpret_cast<void*>(buffer);
}

void* ma_realloc(MemoryAllocator* allocator, void* memory, u32 size) {
	if (!allocator) return nullptr;

	return reinterpret_cast<void*>(allocator->realloc(memory, size));
}

void ma_free(MemoryAllocator* allocator, void* buffer) {
	if (!allocator) return;
	if (!buffer) return;

	allocator->free(buffer);
}



// ENGINE
void init_allocators() {
	standard_allocator.init();
	bump_allocator.init(50 * 1024 * 1024);

	ma_add("bump", &bump_allocator);
	ma_add("standard", &standard_allocator);
}

void update_allocators() {
	bump_allocator.clear();
}
