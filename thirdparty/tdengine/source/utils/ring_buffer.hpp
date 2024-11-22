template<typename T>
struct RingBuffer {
	int32 head      = 0;
	int32 size      = 0;
	int32 capacity  = 0;
	T* data         = nullptr;

	T* operator[](uint32 index) {
		return data + ((head + index) % capacity);
	}
};

template<typename T>
void rb_init(RingBuffer<T>* buffer, int32 capacity) {
	buffer->size = 0;
	buffer->head = 0;
	buffer->capacity = capacity;
	buffer->data = (T*)calloc(capacity, sizeof(T));
}

template<typename T>
void rb_init(RingBuffer<T>* buffer, int32 capacity, T fill) {
	rb_init(buffer, capacity);
	rb_fill(buffer, T());
}

template<typename T>
void rb_free(RingBuffer<T>* buffer) {
	free(buffer->data);
	buffer->data = nullptr;
	buffer->size = 0;
	buffer->head = 0;
	buffer->capacity = 0;
	return;
}


template<typename T>
T* rb_at(RingBuffer<T>* buffer, int32 index) {
	return (*buffer)[index];
}

template<typename T>
T* rb_back(RingBuffer<T>* buffer) {
	fm_assert(buffer->size);
	return (*buffer)[buffer->size - 1];
}

template<typename T>
int32 rb_index_of(RingBuffer<T>* buffer, T* element) {
	int32 index = element - (buffer->data + buffer->head);
	if (index < 0) return (buffer->size - buffer->head) - index;
	return index;
}

template<typename T>
T* rb_push(RingBuffer<T>* buffer, T data) {
	fm_assert(buffer->size < buffer->capacity &&
			  "Insufficient space remaining!");
	auto index = (buffer->head + buffer->size) % buffer->capacity;
	buffer->data[index] = data;
	buffer->size += 1;
	return rb_back(buffer);
}

template<typename T>
T* rb_push(RingBuffer<T>* buffer) {
	fm_assert(buffer->size < buffer->capacity &&
			  "Insufficient space remaining!");
	auto index = (buffer->head + buffer->size) % buffer->capacity;
	buffer->data[index] = T();
	buffer->size += 1;
	return rb_back(buffer);
}

template<typename T>
T* rb_push_overwrite(RingBuffer<T>* buffer) {
	if (buffer->size == buffer->capacity) rb_pop(buffer);
	return rb_push(buffer);
}

template<typename T>
T* rb_push_overwrite(RingBuffer<T>* buffer, T data) {
	if (buffer->size == buffer->capacity) rb_pop(buffer);
	return rb_push(buffer, data); // extra copy?
}

template<typename T>
T rb_pop(RingBuffer<T>* buffer) {
	fm_assert(buffer->size);
	auto element = buffer->data[buffer->head];
	buffer->head = (buffer->head + 1) % buffer->capacity;
	buffer->size--;
	return element;
}

template<typename T>
RingBuffer<T> rb_slice(RingBuffer<T>* buffer, int32 index, int32 size) {
	// No need to make any bounds checks if it's an empty slice
	if (size) {
		fm_assert(size  <= buffer->size);
		fm_assert(index <  buffer->size); 
		fm_assert(index >= 0);
	}
	
	RingBuffer<T> slice;
	slice.data = buffer->data;
	slice.capacity = buffer->capacity;
	slice.size = size;
	slice.head = (buffer->head + index) % buffer->capacity;

	return slice;
}

template<typename T>
RingBuffer<T> rb_slice(RingBuffer<T>* buffer, int32 size) {
	return rb_slice(buffer, 0, size);
}

template<typename T>
RingBuffer<T> rb_rslice(RingBuffer<T>* buffer, int32 size) {
	fm_assert(size  <= buffer->size);
	
	RingBuffer<T> slice;
	slice.data = buffer->data;
	slice.capacity = buffer->capacity;
	slice.size = size;

	slice.head = (buffer->head + buffer->size - size) % buffer->capacity;
		
	return slice;
}

template<typename T>
int32 rb_bytes(RingBuffer<T>* buffer) {
	return buffer->capacity * sizeof(T);
}

template<typename T>
void rb_clear(RingBuffer<T>* buffer) {
	memset(buffer->data, 0, rb_bytes(buffer));
	buffer->size = 0;
	buffer->head = 0;
}

template<typename T>
bool rb_full(RingBuffer<T>* buffer) {
	return buffer->capacity == buffer->size;
}

template<typename T>
bool rb_empty(RingBuffer<T>* buffer) {
	return buffer->size == 0;
}

// Iterator
template<typename T>
struct RingBufferIterator {
	uint32 index;
	bool reverse;
	RingBuffer<T>* buffer;

	T* operator*() {
		return (*buffer)[index];
	}
	
	RingBuffer<T>* operator->() {
		return buffer;
	}
	
	void operator++(int32) {
		fm_assert(index < buffer->size);
		index++;
	}

	void operator--(int32) {
		fm_assert(index >= 0);
		index--;
	}

	bool done() {
		if (reverse)  return index == -1;
		else          return index == buffer->size;
	};
};

template<typename T>
RingBufferIterator<T> rb_iter(RingBuffer<T>* buffer) {
	RingBufferIterator<T> iterator;
	iterator.index = 0;
	iterator.reverse = false;
	iterator.buffer = buffer;
	return iterator;
}

template<typename T>
RingBufferIterator<T> rb_riter(RingBuffer<T>* buffer) {
	RingBufferIterator<T> iterator;
	iterator.index = buffer->size - 1;
	iterator.reverse = true;
	iterator.buffer = buffer;
	return iterator;
}

// Macros
#define rb_for(rb, it)  for (auto (it) = rb_iter((&rb));  !it.done(); it++)
#define rb_rfor(rb, it) for (auto (it) = rb_riter((&rb)); !it.done(); it--)

template<typename T>
void rb_print(RingBuffer<T>* buffer) {
	printf("head = %d, size = %d, cap = %d\n", buffer->head, buffer->size, buffer->capacity);
	rb_for(*buffer, it) {
		printf("rb[%d] = arr[%d] = %d\n", it.index, ((buffer->head + it.index) % buffer->capacity), **it);
	}
	printf("\n");
}

template<typename T>
void rb_rprint(RingBuffer<T>* buffer) {
	printf("head = %d, size = %d, cap = %d\n", buffer->head, buffer->size, buffer->capacity);
	rb_rfor(*buffer, it) {
		printf("rb[%d] = arr[%d] = %d\n", it.index, ((buffer->head + it.index) % buffer->capacity), **it);
	}
	printf("\n");
}
