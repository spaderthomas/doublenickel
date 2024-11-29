template<typename T>
struct dn_ring_buffer_t {
	int32 head      = 0;
	int32 size      = 0;
	int32 capacity  = 0;
	T* data         = nullptr;

	T* operator[](uint32 index) {
		return data + ((head + index) % capacity);
	}
};

template<typename T>
void dn_ring_buffer_init(dn_ring_buffer_t<T>* buffer, int32 capacity) {
	buffer->size = 0;
	buffer->head = 0;
	buffer->capacity = capacity;
	buffer->data = (T*)calloc(capacity, sizeof(T));
}

template<typename T>
void dn_ring_buffer_init(dn_ring_buffer_t<T>* buffer, int32 capacity, T fill) {
	dn_ring_buffer_init(buffer, capacity);
	dn_ring_buffer_fill(buffer, T());
}

template<typename T>
void dn_ring_buffer_free(dn_ring_buffer_t<T>* buffer) {
	free(buffer->data);
	buffer->data = nullptr;
	buffer->size = 0;
	buffer->head = 0;
	buffer->capacity = 0;
	return;
}


template<typename T>
T* dn_ring_buffer_at(dn_ring_buffer_t<T>* buffer, int32 index) {
	return (*buffer)[index];
}

template<typename T>
T* dn_ring_buffer_back(dn_ring_buffer_t<T>* buffer) {
	DN_ASSERT(buffer->size);
	return (*buffer)[buffer->size - 1];
}

template<typename T>
int32 dn_ring_buffer_index_of(dn_ring_buffer_t<T>* buffer, T* element) {
	int32 index = element - (buffer->data + buffer->head);
	if (index < 0) return (buffer->size - buffer->head) - index;
	return index;
}

template<typename T>
T* dn_ring_buffer_push(dn_ring_buffer_t<T>* buffer, T data) {
	DN_ASSERT(buffer->size < buffer->capacity &&
			  "Insufficient space remaining!");
	auto index = (buffer->head + buffer->size) % buffer->capacity;
	buffer->data[index] = data;
	buffer->size += 1;
	return dn_ring_buffer_back(buffer);
}

template<typename T>
T* dn_ring_buffer_push(dn_ring_buffer_t<T>* buffer) {
	DN_ASSERT(buffer->size < buffer->capacity &&
			  "Insufficient space remaining!");
	auto index = (buffer->head + buffer->size) % buffer->capacity;
	buffer->data[index] = T();
	buffer->size += 1;
	return dn_ring_buffer_back(buffer);
}

template<typename T>
T* dn_ring_buffer_push_overwrite(dn_ring_buffer_t<T>* buffer) {
	if (buffer->size == buffer->capacity) dn_ring_buffer_pop(buffer);
	return dn_ring_buffer_push(buffer);
}

template<typename T>
T* dn_ring_buffer_push_overwrite(dn_ring_buffer_t<T>* buffer, T data) {
	if (buffer->size == buffer->capacity) dn_ring_buffer_pop(buffer);
	return dn_ring_buffer_push(buffer, data); // extra copy?
}

template<typename T>
T dn_ring_buffer_pop(dn_ring_buffer_t<T>* buffer) {
	DN_ASSERT(buffer->size);
	auto element = buffer->data[buffer->head];
	buffer->head = (buffer->head + 1) % buffer->capacity;
	buffer->size--;
	return element;
}

template<typename T>
dn_ring_buffer_t<T> dn_ring_buffer_slice(dn_ring_buffer_t<T>* buffer, int32 index, int32 size) {
	// No need to make any bounds checks if it's an empty slice
	if (size) {
		DN_ASSERT(size  <= buffer->size);
		DN_ASSERT(index <  buffer->size); 
		DN_ASSERT(index >= 0);
	}
	
	dn_ring_buffer_t<T> slice;
	slice.data = buffer->data;
	slice.capacity = buffer->capacity;
	slice.size = size;
	slice.head = (buffer->head + index) % buffer->capacity;

	return slice;
}

template<typename T>
dn_ring_buffer_t<T> dn_ring_buffer_slice(dn_ring_buffer_t<T>* buffer, int32 size) {
	return dn_ring_buffer_slice(buffer, 0, size);
}

template<typename T>
dn_ring_buffer_t<T> dn_ring_buffer_rslice(dn_ring_buffer_t<T>* buffer, int32 size) {
	DN_ASSERT(size  <= buffer->size);
	
	dn_ring_buffer_t<T> slice;
	slice.data = buffer->data;
	slice.capacity = buffer->capacity;
	slice.size = size;

	slice.head = (buffer->head + buffer->size - size) % buffer->capacity;
		
	return slice;
}

template<typename T>
int32 dn_ring_buffer_bytes(dn_ring_buffer_t<T>* buffer) {
	return buffer->capacity * sizeof(T);
}

template<typename T>
void dn_ring_buffer_clear(dn_ring_buffer_t<T>* buffer) {
	memset(buffer->data, 0, dn_ring_buffer_bytes(buffer));
	buffer->size = 0;
	buffer->head = 0;
}

template<typename T>
bool dn_ring_buffer_full(dn_ring_buffer_t<T>* buffer) {
	return buffer->capacity == buffer->size;
}

template<typename T>
bool dn_ring_buffer_empty(dn_ring_buffer_t<T>* buffer) {
	return buffer->size == 0;
}

// Iterator
template<typename T>
struct dn_ring_buffer_tIterator {
	uint32 index;
	bool reverse;
	dn_ring_buffer_t<T>* buffer;

	T* operator*() {
		return (*buffer)[index];
	}
	
	dn_ring_buffer_t<T>* operator->() {
		return buffer;
	}
	
	void operator++(int32) {
		DN_ASSERT(index < buffer->size);
		index++;
	}

	void operator--(int32) {
		DN_ASSERT(index >= 0);
		index--;
	}

	bool done() {
		if (reverse)  return index == -1;
		else          return index == buffer->size;
	};
};

template<typename T>
dn_ring_buffer_tIterator<T> dn_ring_buffer_iter(dn_ring_buffer_t<T>* buffer) {
	dn_ring_buffer_tIterator<T> iterator;
	iterator.index = 0;
	iterator.reverse = false;
	iterator.buffer = buffer;
	return iterator;
}

template<typename T>
dn_ring_buffer_tIterator<T> dn_ring_buffer_riter(dn_ring_buffer_t<T>* buffer) {
	dn_ring_buffer_tIterator<T> iterator;
	iterator.index = buffer->size - 1;
	iterator.reverse = true;
	iterator.buffer = buffer;
	return iterator;
}

// Macros
#define dn_ring_buffer_for(rb, it)  for (auto (it) = dn_ring_buffer_iter((&rb));  !it.done(); it++)
#define dn_ring_buffer_rfor(rb, it) for (auto (it) = dn_ring_buffer_riter((&rb)); !it.done(); it--)

template<typename T>
void dn_ring_buffer_print(dn_ring_buffer_t<T>* buffer) {
	printf("head = %d, size = %d, cap = %d\n", buffer->head, buffer->size, buffer->capacity);
	dn_ring_buffer_for(*buffer, it) {
		printf("rb[%d] = arr[%d] = %d\n", it.index, ((buffer->head + it.index) % buffer->capacity), **it);
	}
	printf("\n");
}

template<typename T>
void dn_ring_buffer_rprint(dn_ring_buffer_t<T>* buffer) {
	printf("head = %d, size = %d, cap = %d\n", buffer->head, buffer->size, buffer->capacity);
	dn_ring_buffer_rfor(*buffer, it) {
		printf("rb[%d] = arr[%d] = %d\n", it.index, ((buffer->head + it.index) % buffer->capacity), **it);
	}
	printf("\n");
}
