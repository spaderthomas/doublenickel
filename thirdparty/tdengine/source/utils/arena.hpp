// Defined outside of the struct so we can easily declare it in Lua
struct ArenaHandle {
	u32 index = 0;
	u32 generation = 0;
};

template <typename T>
struct GenerationalArena {	
	struct Entry {
		i32 next_free;
		u32 generation = 1;
		bool occupied = false;

		bool has_next_free() {
			return next_free >= 0;
		}

		bool match(ArenaHandle handle) {
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
		
		values = standard_allocator.alloc<T>(capacity);
		
		entries = standard_allocator.alloc<Entry>(capacity); 
		for (int i = 0; i < capacity; i++) {
			entries[i].next_free = i + 1;
		}
		entries[capacity - 1].next_free = -1;
    }

	ArenaHandle insert(const T& value) {
		assert(!entries[free_list].occupied);
		
		auto entry = entries + free_list;
		entry->occupied = true;
		entry->generation++;

		values[free_list] = value;

		ArenaHandle handle;
		handle.index = free_list;
		handle.generation = entry->generation;
		
		free_list = entry->next_free;
		
		return handle;
	}

	ArenaHandle insert() {
		return insert(T());
	}

	void remove(ArenaHandle handle) {
		if (handle.index >= capacity) return;
		
		auto entry = entries + handle.index;
		if (!entry->match(handle)) return;
		if (!entry->has_next_free()) return;

		entry->occupied = false;
		entry->generation++;
		entry->next_free = free_list;
		free_list = handle.index;
	}


	bool contains(ArenaHandle handle) {
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

	T* operator [](ArenaHandle handle) {
		if (!contains(handle)) return nullptr;
		return values + handle.index;
	}

	ArenaHandle invalid_handle() {
		ArenaHandle handle;
		handle.generation = 0;
		
		return handle;
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
