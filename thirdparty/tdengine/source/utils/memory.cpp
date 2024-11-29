////////////////////
// BASE ALLOCATOR //
////////////////////
template<typename T>
T* dn_allocator_t::alloc(u32 size) {
	return (T*)on_alloc(DN_ALLOCATOR_MODE_ALLOC, size * sizeof(T), nullptr);
}

template<typename T>
void dn_allocator_t::free(T* buffer) {
	on_alloc(DN_ALLOCATOR_MODE_FREE, 0, buffer);
}

template<typename T>
T* dn_allocator_t::realloc(T* buffer, u32 size) {
	return (T*)on_alloc(DN_ALLOCATOR_MODE_RESIZE, size, buffer);
}

template<typename T>
T* dn_allocator_t::alloc() {
	return alloc<T>(1);
}

template<typename T>
Array<T> dn_allocator_t::alloc_array(u32 size) {
	Array<T> array;
	array.data = alloc<T>(size);
	array.size = 0;
	array.capacity = size;

	return array;
}

template<typename T>
void dn_allocator_t::free_array(Array<T>* array) {
	this->free(array->data);
	array->data = nullptr;
	array->size = 0;
	array->capacity = 0;
}

char* dn_allocator_t::alloc_path() {
	return alloc<char>(DN_MAX_PATH_LEN);
}



////////////////////
// BUMP ALLOCATOR //
////////////////////
void dn_bump_allocator_t::init(u32 capacity) {
	this->buffer = standard_allocator.alloc<u8>(capacity);
	this->capacity = capacity;
	
	on_alloc = [this](dn_allocator_mode_t mode, u32 size, void* old_memory) -> void* {
		if (mode == DN_ALLOCATOR_MODE_ALLOC) {
			if (this->bytes_used + size > this->capacity) {
				assert(false);
			}

			auto memory_block = this->buffer + this->bytes_used;
			allocations[this->bytes_used] = size;
			this->bytes_used += size;
		
			return memory_block;
		}
		else if (mode == DN_ALLOCATOR_MODE_FREE) {
			return nullptr;
		}
		else if (mode == DN_ALLOCATOR_MODE_RESIZE) {
			if (!old_memory) {
				return on_alloc(DN_ALLOCATOR_MODE_ALLOC, size, nullptr);
			}
	
			auto offset = (u32)((u8*)old_memory - (u8*)this->buffer);
			auto old_size = allocations[offset];
			if (old_size >= size) {
				return old_memory;
			} 

			auto memory_block = on_alloc(DN_ALLOCATOR_MODE_ALLOC, size, nullptr);
			dn_os_memory_copy(old_memory, memory_block, size);
			return memory_block;
		}

		assert(false);
		return nullptr;
	};
}

void dn_bump_allocator_t::clear() {
	std::memset(buffer, 0, bytes_used);
	bytes_used = 0;
	allocations.clear();
}


///////////////////////
// DEFAULT ALLOCATOR //
///////////////////////
void dn_standard_allocator_t::init() {
	on_alloc = [&](dn_allocator_mode_t mode, u32 size, void* old_memory) -> void* {
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
	
		assert(false);
		return nullptr;
	};
}


// LUA API
void dn_allocator_add(const char* name, dn_allocator_t* allocator) {
	if (!allocator) return;

	allocators[name] = allocator;
}

dn_allocator_t* dn_allocator_find(const char* name) {
	if (!name) return nullptr;

	if (!allocators.contains(name)) {
		dn_log("Tried to find allocator, but name was not registered; name = %s", name);
		return nullptr;
	}

	return allocators[name];
}

void* dn_allocator_alloc(dn_allocator_t* allocator, u32 size) {
	if (!allocator) return nullptr;

	auto buffer = allocator->alloc<u8>(size);
	return reinterpret_cast<void*>(buffer);
}

void* dn_allocator_realloc(dn_allocator_t* allocator, void* memory, u32 size) {
	if (!allocator) return nullptr;

	return reinterpret_cast<void*>(allocator->realloc(memory, size));
}

void dn_allocator_free(dn_allocator_t* allocator, void* buffer) {
	if (!allocator) return;
	if (!buffer) return;

	allocator->free(buffer);
}



// ENGINE
void dn_allocators_init() {
	standard_allocator.init();
	bump_allocator.init(50 * 1024 * 1024);

	dn_allocator_add("bump", &bump_allocator);
	dn_allocator_add("standard", &standard_allocator);
}

void dn_allocators_update() {
	bump_allocator.clear();
}