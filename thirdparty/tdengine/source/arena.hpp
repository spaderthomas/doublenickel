// Defined outside of the struct so we can easily declare it in Lua
struct dn_gen_arena_handle_t {
	u32 index;
	u32 generation;
};
	dn_gen_arena_handle_t dn_gen_arena_invalid_handle() {
		return {
			.index = 0,
			.generation = 0
		};
	}


#define DN_GEN_ARENA_HANDLE(t) dn_gen_arena_handle_t

bool dn_gen_arena_handle_valid(dn_gen_arena_handle_t handle) {
	return handle.generation > 0;
}

template <typename T>
struct GenerationalArena {	
	struct Entry {
		i32 next_free;
		u32 generation = 1;
		bool occupied = false;

		bool has_next_free() {
			return next_free >= 0;
		}

		bool match(dn_gen_arena_handle_t handle) {
			return generation == handle.generation;
		}
	};

	struct Iterator {
		GenerationalArena* arena;
		u32 index;

		Iterator& operator++() {
			do {
				++index;
			} while (index < arena->capacity && !arena->entries[index].occupied);
			return *this;
		}

		T& operator*() {
			return arena->values[index];
		}

		bool operator!=(const Iterator& other) const {
			return index != other.index;
		}
	};

	T* values;
	Entry* entries;

	i32 free_list;
	u32 capacity;

    void init(u32 capacity) {
		this->capacity = capacity;
		free_list = 0;
		
		values = dn::allocator::alloc<T>(&dn_allocators.standard, capacity);
		
		entries = dn::allocator::alloc<Entry>(&dn_allocators.standard, capacity); 
		for (int i = 0; i < capacity; i++) {
			entries[i].next_free = i + 1;
		}
		entries[capacity - 1].next_free = -1;
    }

	dn_gen_arena_handle_t insert(const T& value) {
		assert(!entries[free_list].occupied);
		
		auto entry = entries + free_list;
		entry->occupied = true;
		entry->generation++;

		values[free_list] = value;

		dn_gen_arena_handle_t handle;
		handle.index = free_list;
		handle.generation = entry->generation;
		
		free_list = entry->next_free;
		
		return handle;
	}

	dn_gen_arena_handle_t insert() {
		return insert(T());
	}

	void remove(dn_gen_arena_handle_t handle) {
		if (handle.index >= capacity) return;
		
		auto entry = entries + handle.index;
		if (!entry->match(handle)) return;
		if (!entry->has_next_free()) return;

		entry->occupied = false;
		entry->generation++;
		entry->next_free = free_list;
		free_list = handle.index;
	}


	bool contains(dn_gen_arena_handle_t handle) {
		if (handle.index >= capacity) return false;

		return entries[handle.index].match(handle);
	}

	void clear() {
		for (int i = 0; i < capacity; i++) {
			entries[i].next_free = i + 1;
			entries[i].occupied = false;
			entries[i].generation++;
		}

		entries[capacity - 1].next_free = -1;

		free_list = 0;
	}

	T* operator [](dn_gen_arena_handle_t handle) {
		if (!contains(handle)) return nullptr;
		return values + handle.index;
	}

	Iterator begin() {
		Iterator it{ this, 0 };
		if (!entries[0].occupied) ++it;
		return it;
	}

	Iterator end() {
		return Iterator{ this, capacity };
	}


};
