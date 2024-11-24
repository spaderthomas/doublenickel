#ifndef FIXED_ARRAY_H
#define FIXED_ARRAY_H

typedef struct {
	u8* data;
	u32 size;
	u32 capacity;

	u32 vertex_size;
} dn_fixed_array_t;

FM_LUA_EXPORT void dn_fixed_array_init(dn_fixed_array_t* vertex_buffer, u32 max_vertices, u32 vertex_size);
FM_LUA_EXPORT u8*  dn_fixed_array_push(dn_fixed_array_t* vertex_buffer, void* data, u32 count);
FM_LUA_EXPORT u8*  dn_fixed_array_reserve(dn_fixed_array_t* vertex_buffer, u32 count);
FM_LUA_EXPORT void dn_fixed_array_clear(dn_fixed_array_t* vertex_buffer);
FM_LUA_EXPORT u32  dn_fixed_array_byte_size(dn_fixed_array_t* vertex_buffer);
FM_LUA_EXPORT u8*  dn_fixed_array_at(dn_fixed_array_t* vertex_buffer, u32 index);
#endif


#ifdef FIXED_ARRAY_IMPLEMENTATION
void dn_fixed_array_init(dn_fixed_array_t* buffer, u32 max_vertices, u32 vertex_size) {
	TD_ASSERT(buffer);

	buffer->size = 0;
	buffer->capacity = max_vertices;
	buffer->vertex_size = vertex_size;
	buffer->data = (u8*)dn_allocator_alloc(&standard_allocator, max_vertices * vertex_size);
}

u8* dn_fixed_array_at(dn_fixed_array_t* buffer, u32 index) {
	TD_ASSERT(buffer);
	return buffer->data + (index * buffer->vertex_size);
}

u8* dn_fixed_array_push(dn_fixed_array_t* buffer, void* data, u32 count) {
	TD_ASSERT(buffer);
	TD_ASSERT(buffer->size < buffer->capacity);

	auto vertices = dn_fixed_array_reserve(buffer, count);
	if (data) copy_memory(data, vertices, buffer->vertex_size * count);
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

	return buffer->size * buffer->vertex_size;
}
#endif