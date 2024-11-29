#ifndef DN_FIXED_ARRAY_H
#define DN_FIXED_ARRAY_H

typedef struct {
  u8* data;
  u32 size;
  u32 capacity;
  u32 element_size;
  dn_allocator_t* allocator;
} dn_fixed_array_t;

template<typename T, u32 N>
struct dn_fixed_array : dn_fixed_array_t {};

DN_API void dn_fixed_array_init(dn_fixed_array_t* vertex_buffer, u32 max_vertices, u32 element_size, dn_allocator_t* allocator);
DN_API u8*  dn_fixed_array_push(dn_fixed_array_t* vertex_buffer, void* data, u32 count);
DN_API u8*  dn_fixed_array_reserve(dn_fixed_array_t* vertex_buffer, u32 count);
DN_API void dn_fixed_array_clear(dn_fixed_array_t* vertex_buffer);
DN_API u32  dn_fixed_array_byte_size(dn_fixed_array_t* vertex_buffer);
DN_API u8*  dn_fixed_array_at(dn_fixed_array_t* vertex_buffer, u32 index);

template<typename T, u32 N>
void dn_fixed_array_init_t(dn_fixed_array<T, N>* vertex_buffer, dn_allocator_t* allocator);

template<typename T, u32 N>
T* dn_fixed_array_push_t(dn_fixed_array<T, N>* vertex_buffer, T* data, u32 count) {
  return (T*)dn_fixed_array_push(vertex_buffer, data, count);
}

template<typename T, u32 N>
T* dn_fixed_array_reserve_t(dn_fixed_array<T, N>* vertex_buffer, u32 count) {
  return (T*)dn_fixed_array_reserve(vertex_buffer, count);
}

template<typename T, u32 N>
T* dn_fixed_array_at_t(dn_fixed_array<T, N>* fixed_array, u32 index) {
  return (T*)dn_fixed_array_at((dn_fixed_array_t*)fixed_array, index);
}
#endif


#ifdef DN_FIXED_ARRAY_IMPLEMENTATION
template<typename T, u32 N>
void dn_fixed_array_init_t(dn_fixed_array<T, N>* vertex_buffer, dn_allocator_t* allocator) {
  return dn_fixed_array_init(vertex_buffer, N, sizeof(T), allocator);
}

void dn_fixed_array_init(dn_fixed_array_t* buffer, u32 max_vertices, u32 element_size, dn_allocator_t* allocator) {
  DN_ASSERT(buffer);

  buffer->size = 0;
  buffer->capacity = max_vertices;
  buffer->element_size = element_size;
  buffer->data = (u8*)dn_allocator_alloc(allocator, max_vertices * element_size);
  buffer->allocator = allocator;
}

u8* dn_fixed_array_at(dn_fixed_array_t* buffer, u32 index) {
  DN_ASSERT(buffer);
  return buffer->data + (index * buffer->element_size);
}

u8* dn_fixed_array_push(dn_fixed_array_t* buffer, void* data, u32 count) {
  DN_ASSERT(buffer);
  DN_ASSERT(buffer->size < buffer->capacity);

  auto vertices = dn_fixed_array_reserve(buffer, count);
  if (data) dn_os_memory_copy(data, vertices, buffer->element_size * count);
  return vertices;
}

u8* dn_fixed_array_reserve(dn_fixed_array_t* buffer, u32 count) {
  DN_ASSERT(buffer);
  
  auto vertex = dn_fixed_array_at(buffer, buffer->size);
  buffer->size += count;
  return vertex;
}

void dn_fixed_array_clear(dn_fixed_array_t* buffer) {
  DN_ASSERT(buffer);

  buffer->size = 0;
}

u32 dn_fixed_array_byte_size(dn_fixed_array_t* buffer) {
  DN_ASSERT(buffer);

  return buffer->size * buffer->element_size;
}
#endif