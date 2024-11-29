#ifndef DN_ARRAY_H
#define DN_ARRAY_H

void memfill(void* dst, i32 size, void* pattern, i32 pattern_size);

template<typename T, u64 N = 0>
struct dn_array_t {
	u64 size      = 0;
	u64 capacity  = 0;
	T* data         = nullptr;

	T* operator[](u64 index) { DN_ASSERT(index < size); return data + index; }
	operator bool() { return data; }
};

template<typename T, u64 N>
void dn_array_init(dn_array_t<T, N>* array, u64 capacity, dn_allocator_t* allocator);

template<typename T, u64 N>
void dn_array_init(dn_array_t<T, N>* array, u64 capacity);

template<typename T, u64 N>
void dn_array_init(dn_array_t<T, N>* array, u64 capacity, T fill);

template<typename T, u64 N>
void dn_array_init(dn_array_t<T, N>* array);

template<typename T, u64 N>
void dn_array_init(dn_array_t<T, N>* array, dn_allocator_t* allocator);

template<typename T, u64 N>
void dn_array_clear(dn_array_t<T, N>* array);

template<typename T, u64 N>
void dn_array_fastclear(dn_array_t<T, N>* array);

template<typename T, u64 N>
void dn_array_clear_n(dn_array_t<T, N>* array, i32 n);

template<typename T, u64 N>
void dn_array_fill(dn_array_t<T, N>* array, T element);

template<typename T, u64 N>
void dn_array_fill(dn_array_t<T, N>* array, T element, u64 count);

template<typename T, u64 N>
void dn_array_fill(dn_array_t<T, N>* array, u64 offset, u64 count, T element);

// Use case: You declare some array on the stack. It's empty, and you only want to modify its elements
// using an dn_array_t. Call this to wrap it in an empty dn_array_t of the correct capacity.
template<typename T, u64 N>
dn_array_t<T, N> dn_array_stack(T* data, u64 capacity);

template<typename T, u64 N>
dn_array_t<T, N> dn_array_stack(T (&c_array)[N]);

// Use case: You have some contiguous data filled out somewhere (maybe in another dn_array_t, maybe in a C
// array). You want to RW a subarray using dn_array_t functions. Call this to wrap the subarray. 
template<typename T, u64 N>
dn_array_t<T, N> dn_array_slice(dn_array_t<T, N>* array, u64 index, u64 size);

template<typename T, u64 N>
dn_array_t<T, N> dn_array_slice(T* data, u64 size);

template<typename T, u64 N>
dn_array_t<T, N> dn_array_slice(T (&data)[N]);

template<typename T, u64 N>
u32 dn_array_indexof(dn_array_t<T, N>* array, T* element);

template<typename T, u64 N>
T* dn_array_at(dn_array_t<T, N>* array, u64 index);

template<typename T, u64 N>
bool dn_array_full(dn_array_t<T, N>* array);

template<typename T, u64 N>
T* dn_array_push(dn_array_t<T, N>* array, const T* data, u64 count);

template<typename T, u64 N>
T* dn_array_push(dn_array_t<T, N>* array, T* data, u64 count);

template<typename T, u64 N>
T* dn_array_push(dn_array_t<T, N>* array, T element, u64 count);

template<typename T, u64 N>
T* dn_array_push(dn_array_t<T, N>* array, T* data);

template<typename T, u64 N>
T* dn_array_push(dn_array_t<T, N>* array, T data);

template<typename T, u64 N>
T* dn_array_push(dn_array_t<T, N>* array);

template<typename T, u64 N>
void dn_array_pop(dn_array_t<T, N>* array);

// "Preallocate" space in the array without the cost of zeroing memory or default constructing
// entities. For example, if you want to push 16 elements to an array and modify the elements
// in-place -- reserve the space with this function, then just use the memory blocks.
template<typename T, u64 N>
T* dn_array_reserve(dn_array_t<T, N>* array, u64 count);

template<typename T, u64 N>
T* dn_array_concat(dn_array_t<T, N>* dest, dn_array_t<T, N>* source);

template<typename T, u64 N>
T* dn_array_concat(dn_array_t<T, N>* dest, dn_array_t<T, N>* source, u64 count);

template<typename T, u64 N>
T* dn_array_back(dn_array_t<T, N>* array);

template<typename T, u64 N>
T* dn_array_next(dn_array_t<T, N>* array);

template<typename T, u64 N>
void dn_array_free(dn_array_t<T, N>* array);

template<typename T, u64 N>
i32 dn_array_bytes(dn_array_t<T, N>* array);

template<typename T, u64 N>
i32 dn_array_bytes_used(dn_array_t<T, N>* array);

#define dn_array_for(array, it) for (auto (it) = (array).data; (it) != ((array).data + (array).size); (it)++)
#define dn_array_for_ref(array, it, ref) for (auto (it) = (array).data; (it) != ((array).data + (array).size); (it)++)
#define dn_array_rfor(array, it) for (auto (it) = (array).data + array.size - 1; (it) >= ((array).data); (it)--)



template<typename T> 
struct dn_array_view_t {
	u64 size      = 0;
	u64 capacity  = 0;
	T* data         = nullptr;

	T* operator[](u64 index) { DN_ASSERT(index < size); return data + index; }
};

template<typename T>
dn_array_view_t<T> dn_array_view(T* data, u64 size);

template<typename T>
dn_array_view_t<T> dn_array_view(dn_array_t<T>* array);

template<typename T>
dn_array_view_t<T> dn_array_view(dn_array_t<T>* array, u64 index, u64 count);

template<typename T, u64 N>
dn_array_view_t<T> dn_array_view(T (&array)[N]);

template<typename T>
i32 dn_array_indexof(dn_array_view_t<T>* array, T* element);
#endif

#ifdef DN_ARRAY_IMPLEMENTATION
void memfill(void* dst, i32 size, void* pattern, i32 pattern_size) {
	char* cdst = (char*)dst;
	char* cptn = (char*)pattern;
	int i = 0;

	while (true) {
		if (i + pattern_size > size) return;
		memcpy(cdst + i, cptn, pattern_size);
		i += pattern_size;
	}
}

template<typename T, u64 N>
void dn_array_init(dn_array_t<T, N>* array, u64 capacity, dn_allocator_t* allocator) {
	array->size = 0;
	array->capacity = capacity;
	array->data = dn::allocator::alloc<T>(allocator, capacity);
}

template<typename T, u64 N>
void dn_array_init(dn_array_t<T, N>* array, u64 capacity) {
	dn_array_init(array, capacity, &dn_allocators.standard);
}

template<typename T, u64 N>
void dn_array_init(dn_array_t<T, N>* array, u64 capacity, T fill) {
	dn_array_init(array, capacity, &dn_allocators.standard);
	dn_array_fill(array, T());
}

template<typename T, u64 N>
void dn_array_init(dn_array_t<T, N>* array) {
	dn_array_init(array, &dn_allocators.standard);
}

template<typename T, u64 N>
void dn_array_init(dn_array_t<T, N>* array, dn_allocator_t* allocator) {
	static_assert(N > 0, "If you want to bake a fixed capacity into an dn_array_t, you need to put it as the second template parameter. Otherwise, specify the capacity when initializing.");
	array->size = 0;
	array->capacity = N;
	array->data = dn::allocator::alloc<T>(allocator, N);
}

template<typename T, u64 N>
void dn_array_clear(dn_array_t<T, N>* array) {
	memset(array->data, 0, array->size * sizeof(T));
	array->size = 0;
}

template<typename T, u64 N>
void dn_array_fastclear(dn_array_t<T, N>* array) {
	array->size = 0;
}

template<typename T, u64 N>
void dn_array_clear_n(dn_array_t<T, N>* array, i32 n) {
	memset(array->data, 0, n * sizeof(T));
	array->size = 0;
}

template<typename T, u64 N>
void dn_array_fill(dn_array_t<T, N>* array, T element) {
	memfill(array->data, dn_array_bytes(array), &element, sizeof(T));
	array->size = array->capacity; 
}

template<typename T, u64 N>
void dn_array_fill(dn_array_t<T, N>* array, T element, u64 count) {
	memfill(array->data, count * sizeof(T), &element, sizeof(T));
	array->size = count;
}

template<typename T, u64 N>
void dn_array_fill(dn_array_t<T, N>* array, u64 offset, u64 count, T element) {
	memfill(array->data + offset, count * sizeof(T), &element, sizeof(T));
	array->size = count;
	// You're on your own as far as the size here
}

template<typename T, u64 N>
dn_array_t<T, N> dn_array_stack(T* data, u64 capacity) {
	DN_ASSERT(data);

	dn_array_t<T, N> array;
	array.size = 0;
	array.capacity = capacity;
	array.data = data;
	return array;
}

template<typename T, u64 N>
dn_array_t<T, N> dn_array_stack(T (&c_array)[N]) {
	dn_array_t<T, N> array;
	array.size = 0;
	array.capacity = N;
	array.data = c_array;
	return array;
}

template<typename T, u64 N>
dn_array_t<T, N> dn_array_slice(dn_array_t<T, N>* array, u64 index, u64 size) {
	DN_ASSERT(index >= 0);
	DN_ASSERT(index + size <= array->capacity);
	
	dn_array_t<T, N> view;
	view.size = size;
	view.capacity = size;
	view.data = array->data + index;
	
	return view;
}

template<typename T, u64 N>
dn_array_t<T, N> dn_array_slice(T* data, u64 size) {
	dn_array_t<T, N> arr;
	arr.size = size;
	arr.capacity = size;
	arr.data = data;
	
	return arr; 
}

template<typename T, u64 N>
dn_array_t<T, N> dn_array_slice(T (&data)[N]) {
	dn_array_t<T, N> array;
	array.size = N;
	array.capacity = N;
	array.data = data;
	return array;
}

template<typename T, u64 N>
u32 dn_array_indexof(dn_array_t<T, N>* array, T* element) {
	u32 index = element - array->data;
	DN_ASSERT(index >= 0);
	DN_ASSERT(index < array->size);
	return index;
}

template<typename T, u64 N>
T* dn_array_at(dn_array_t<T, N>* array, u64 index) {
	return (*array)[index];
}

template<typename T, u64 N>
bool dn_array_full(dn_array_t<T, N>* array) {
	return array->size ==  array->capacity;
}

template<typename T, u64 N>
T* dn_array_push(dn_array_t<T, N>* array, const T* data, u64 count) {
	i32 remaining = array->capacity - array->size;
	assert(remaining >= count && "Insufficient space remaining!");

	if (remaining < count) return nullptr;
	
	memcpy(array->data + array->size, data, sizeof(T) * count);
	T* out = array->data + array->size;
	array->size += count;
	return out;
}

template<typename T, u64 N>
T* dn_array_push(dn_array_t<T, N>* array, T* data, u64 count) {
	i32 remaining = array->capacity - array->size;
	assert(remaining >= count && "Insufficient space remaining!");
	
	memcpy(array->data + array->size, data, sizeof(T) * count);
	T* out = array->data + array->size;
	array->size += count;
	return out;
}

template<typename T, u64 N>
T* dn_array_push(dn_array_t<T, N>* array, T element, u64 count) {
	auto back = array->data + array->size;
	memfill(back, count * sizeof(T), &element, sizeof(T));
	array->size += count;
	return back;
}

template<typename T, u64 N>
T* dn_array_push(dn_array_t<T, N>* array, T* data) {
	assert(array->size < array->capacity && "Insufficient space remaining!");
	array->data[array->size] = *data;
	T* out = array->data + array->size;
	array->size += 1;
	return out;
}

template<typename T, u64 N>
T* dn_array_push(dn_array_t<T, N>* array, T data) {
	assert(array->size < array->capacity && "Insufficient space remaining!");
	array->data[array->size] = data;
	T* out = array->data + array->size;
	array->size += 1;
	return out;
}

template<typename T, u64 N>
T* dn_array_push(dn_array_t<T, N>* array) {
	assert(array->size < array->capacity && "Insufficient space remaining!");
	array->data[array->size] = T();
	T* out = array->data + array->size;
	array->size += 1;
	return out;
}

template<typename T, u64 N>
void dn_array_pop(dn_array_t<T, N>* array) {
	DN_ASSERT(array->size && "cannot pop / what has not / been pushed");
	array->size -= 1;
	return;
}

template<typename T, u64 N>
T* dn_array_reserve(dn_array_t<T, N>* array, u64 count) {
	DN_ASSERT(array->size <= array->capacity + count && "Insufficient space remaining!");
	T* out = array->data + array->size;
	array->size += count;
	return out;
}


template<typename T, u64 N>
T* dn_array_concat(dn_array_t<T, N>* dest, dn_array_t<T, N>* source) {
	DN_ASSERT(dest->size + source->size < dest->capacity);
	memcpy(dest->data + dest->size, source->data, sizeof(T) * source->count);
	T* out = dest->data + dest->size;
	dest->size += source->size;
	return out;
}

template<typename T, u64 N>
T* dn_array_concat(dn_array_t<T, N>* dest, dn_array_t<T, N>* source, u64 count) {
	DN_ASSERT(dest->size + count < dest->capacity);
	memcpy(dest->data + dest->size, source->data, sizeof(T) * count);
	T* out = dest->data + dest->size;
	dest->size += count;
	return out;
}


template<typename T, u64 N>
T* dn_array_back(dn_array_t<T, N>* array) {
	if (!array->size) return array->data;
	return array->data + (array->size - 1);
}

template<typename T, u64 N>
T* dn_array_next(dn_array_t<T, N>* array) {
	DN_ASSERT(array->size != array->capacity);
	return array->data + (array->size);
}

template<typename T, u64 N>
void dn_array_free(dn_array_t<T, N>* array) {
	if (!array->data) return;
	
	free(array->data);
	
	array->data = nullptr;
	array->size = 0;
	array->capacity = 0;
	return;
}

template<typename T, u64 N>
i32 dn_array_bytes(dn_array_t<T, N>* array) {
	return array->capacity * sizeof(T);
}

template<typename T, u64 N>
i32 dn_array_bytes_used(dn_array_t<T, N>* array) {
	return array->size * sizeof(T);
}

#define dn_array_for(array, it) for (auto (it) = (array).data; (it) != ((array).data + (array).size); (it)++)
#define dn_array_rfor(array, it) for (auto (it) = (array).data + array.size - 1; (it) >= ((array).data); (it)--)


////////////////
// ARRAY VIEW //
////////////////
template<typename T>
dn_array_view_t<T> dn_array_view(T* data, u64 size) {
	dn_array_view_t<T> view;
	view.size = size;
	view.capacity = size;
	view.data = data;
	
	return view;
}

template<typename T>
dn_array_view_t<T> dn_array_view(dn_array_t<T>* array) {
	dn_array_view_t<T> view;
	view.size = array->size;
	view.capacity = array->size;
	view.data = array->data;
	
	return view;
}

template<typename T>
dn_array_view_t<T> dn_array_view(dn_array_t<T>* array, u64 index, u64 count) {
	DN_ASSERT(index >= 0);
	DN_ASSERT(index + count <= array->capacity);

	dn_array_view_t<T> view;
	view.size = count;
	view.capacity = count;
	view.data = array->data + index;
	
	return view;
}

template<typename T, u64 N>
dn_array_view_t<T> dn_array_view(T (&array)[N]) {
	dn_array_view_t<T> view;
	view.size = N;
	view.capacity = N;
	view.data = array;
	
	return view;
}


template<typename T>
i32 dn_array_indexof(dn_array_view_t<T>* array, T* element) {
	i32 index = element - array->data;
	DN_ASSERT(index >= 0);
	DN_ASSERT(index < array->size);
	return index;
}

#endif