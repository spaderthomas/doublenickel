
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


