#ifndef DN_GEN_ARENA_H
#define DN_GEN_ARENA_H
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

bool dn_gen_arena_handle_valid(dn_gen_arena_handle_t handle) {
	return handle.generation > 0;
}

template <typename T>
struct dn_gen_arena_t {	
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
		dn_gen_arena_t* arena;
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

	void init(u32 capacity);
	dn_gen_arena_handle_t insert(const T& value) ;
	dn_gen_arena_handle_t insert();
	void remove(dn_gen_arena_handle_t handle);
	bool contains(dn_gen_arena_handle_t handle);
	void clear();
	T* operator [](dn_gen_arena_handle_t handle);
	Iterator begin();
	Iterator end();
};
#endif

#ifdef DN_GEN_ARENA_IMPLEMENTATION
template<typename T>
void dn_gen_arena_t<T>::init(u32 capacity) {
	this->capacity = capacity;
	free_list = 0;
	
	values = dn::allocator::alloc<T>(&dn_allocators.standard, capacity);
	
	entries = dn::allocator::alloc<Entry>(&dn_allocators.standard, capacity); 
	for (int i = 0; i < capacity; i++) {
		entries[i].next_free = i + 1;
	}
	entries[capacity - 1].next_free = -1;
}

template<typename T>
dn_gen_arena_handle_t dn_gen_arena_t<T>::insert(const T& value) {
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

template<typename T>
dn_gen_arena_handle_t dn_gen_arena_t<T>::insert() {
	return insert(T());
}

template<typename T>
void dn_gen_arena_t<T>::remove(dn_gen_arena_handle_t handle) {
	if (handle.index >= capacity) return;
	
	auto entry = entries + handle.index;
	if (!entry->match(handle)) return;
	if (!entry->has_next_free()) return;

	entry->occupied = false;
	entry->generation++;
	entry->next_free = free_list;
	free_list = handle.index;
}


template<typename T>
bool dn_gen_arena_t<T>::contains(dn_gen_arena_handle_t handle) {
	if (handle.index >= capacity) return false;

	return entries[handle.index].match(handle);
}

template<typename T>
void dn_gen_arena_t<T>::clear() {
	for (int i = 0; i < capacity; i++) {
		entries[i].next_free = i + 1;
		entries[i].occupied = false;
		entries[i].generation++;
	}

	entries[capacity - 1].next_free = -1;

	free_list = 0;
}

template<typename T>
T* dn_gen_arena_t<T>::operator [](dn_gen_arena_handle_t handle) {
	if (!contains(handle)) return nullptr;
	return values + handle.index;
}

template<typename T>
dn_gen_arena_t<T>::Iterator dn_gen_arena_t<T>::begin() {
	Iterator it{ this, 0 };
	if (!entries[0].occupied) ++it;
	return it;
}

template<typename T>
dn_gen_arena_t<T>::Iterator dn_gen_arena_t<T>::end() {
	return Iterator{ this, capacity };
}
#endif