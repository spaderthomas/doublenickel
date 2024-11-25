#ifndef FIXED_ARRAY_H
#define FIXED_ARRAY_H

typedef struct {
	u8* data;
	u32 size;
	u32 capacity;
	u32 element_size;
} dn_fixed_array_t;

template<typename T>
struct dn_fixed_array : dn_fixed_array_t {};

DN_API void dn_fixed_array_init(dn_fixed_array_t* vertex_buffer, u32 max_vertices, u32 element_size);
DN_API u8*  dn_fixed_array_push(dn_fixed_array_t* vertex_buffer, void* data, u32 count);
DN_API u8*  dn_fixed_array_reserve(dn_fixed_array_t* vertex_buffer, u32 count);
DN_API void dn_fixed_array_clear(dn_fixed_array_t* vertex_buffer);
DN_API u32  dn_fixed_array_byte_size(dn_fixed_array_t* vertex_buffer);
DN_API u8*  dn_fixed_array_at(dn_fixed_array_t* vertex_buffer, u32 index);

template<typename T>
void dn_fixed_array_init_t(dn_fixed_array<T>* vertex_buffer, u32 count) {
	return dn_fixed_array_init(vertex_buffer, count, sizeof(T));
}

template<typename T>
T* dn_fixed_array_push_t(dn_fixed_array<T>* vertex_buffer, T* data, u32 count) {
	return (T*)dn_fixed_array_push(vertex_buffer, data, count);
}

template<typename T>
T* dn_fixed_array_at_t(dn_fixed_array<T>* fixed_array, u32 index) {
	return (T*)dn_fixed_array_at((dn_fixed_array_t*)fixed_array, index);
}
#endif


#ifdef FIXED_ARRAY_IMPLEMENTATION

void dn_fixed_array_init(dn_fixed_array_t* buffer, u32 max_vertices, u32 element_size) {
	TD_ASSERT(buffer);

	buffer->size = 0;
	buffer->capacity = max_vertices;
	buffer->element_size = element_size;
	buffer->data = (u8*)dn_allocator_alloc(&standard_allocator, max_vertices * element_size);
}

u8* dn_fixed_array_at(dn_fixed_array_t* buffer, u32 index) {
	TD_ASSERT(buffer);
	return buffer->data + (index * buffer->element_size);
}

u8* dn_fixed_array_push(dn_fixed_array_t* buffer, void* data, u32 count) {
	TD_ASSERT(buffer);
	TD_ASSERT(buffer->size < buffer->capacity);

	auto vertices = dn_fixed_array_reserve(buffer, count);
	if (data) copy_memory(data, vertices, buffer->element_size * count);
	return vertices;
}

u8* dn_fixed_array_reserve(dn_fixed_array_t* buffer, u32 count) {
	TD_ASSERT(buffer);
	
	auto vertex = dn_fixed_array_at(buffer, buffer->size);
	buffer->size += count;
	return vertex;
}

void dn_fixed_array_clear(dn_fixed_array_t* buffer) {
	TD_ASSERT(buffer);

	buffer->size = 0;
}

u32 dn_fixed_array_byte_size(dn_fixed_array_t* buffer) {
	TD_ASSERT(buffer);

	return buffer->size * buffer->element_size;
}
#endif