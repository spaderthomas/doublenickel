#ifndef DN_DYNAMIC_ARRAY_H
#define DN_DYNAMIC_ARRAY_H

typedef struct {
  u8* data;
  u32 size;
  u32 capacity;
  u32 element_size;
  dn_allocator_t* allocator;
} dn_dynamic_array_t;

template<typename T>
struct dn_dynamic_array : dn_dynamic_array_t {};

DN_API void dn_dynamic_array_init(dn_dynamic_array_t* dynamic_array, u32 element_size, dn_allocator_t* allocator);
DN_API u8*  dn_dynamic_array_push(dn_dynamic_array_t* dynamic_array, void* data, u32 count);
DN_API u8*  dn_dynamic_array_reserve(dn_dynamic_array_t* dynamic_array, u32 count);
DN_API void dn_dynamic_array_clear(dn_dynamic_array_t* dynamic_array);
DN_API u32  dn_dynamic_array_byte_size(dn_dynamic_array_t* dynamic_array);
DN_API u8*  dn_dynamic_array_at(dn_dynamic_array_t* dynamic_array, u32 index);
DN_API void dn_dynamic_array_grow(dn_dynamic_array_t* dynamic_array, u32 capacity);

namespace dn::dynamic_array {
  template<typename T>
  void init(dn_dynamic_array<T>* dynamic_array, dn_allocator_t* allocator);

  template<typename T>
  T* push(dn_dynamic_array<T>* dynamic_array, T* data, u32 count);

  template<typename T>
  T* push(dn_dynamic_array<T>* dynamic_array, T data);

  template<typename T>
  T* push(dn_dynamic_array<T>* dynamic_array);

  template<typename T>
  T* reserve(dn_dynamic_array<T>* dynamic_array, u32 count);

  template<typename T>
  T* at(dn_dynamic_array<T>* dynamic_array, u32 index);
}
#endif



#ifdef DN_DYNAMIC_ARRAY_IMPLEMENTATION
namespace dn::dynamic_array {
  template<typename T>
  void init(dn_dynamic_array<T>* dynamic_array, dn_allocator_t* allocator) {
    return dn_dynamic_array_init(dynamic_array, sizeof(T), allocator);
  }

  template<typename T>
  T* push(dn_dynamic_array<T>* dynamic_array, T* data, u32 count) {
    return (T*)dn_dynamic_array_push(dynamic_array, data, count);
  }

  template<typename T>
  T* push(dn_dynamic_array<T>* dynamic_array, T data) {
    return (T*)dn_dynamic_array_push(dynamic_array, &data, 1);
  }

  template<typename T>
  T* push(dn_dynamic_array<T>* dynamic_array) {
    T data = dn_zero_initialize();
    return (T*)dn_dynamic_array_push(dynamic_array, &data, 1);
  }

  template<typename T>
  T* reserve(dn_dynamic_array<T>* dynamic_array, u32 count) {
    return (T*)dn_dynamic_array_reserve(dynamic_array, count);
  }

  template<typename T>
  T* at(dn_dynamic_array<T>* dynamic_array, u32 index) {
    return (T*)dn_dynamic_array_at((dn_dynamic_array_t*)dynamic_array, index);
  }
}

void dn_dynamic_array_init(dn_dynamic_array_t* buffer, u32 element_size, dn_allocator_t* allocator) {
  DN_ASSERT(buffer);

  buffer->size = 0;
  buffer->capacity = 2;
  buffer->element_size = element_size;
  buffer->data = (u8*)dn_allocator_alloc(allocator, buffer->capacity * element_size);
  buffer->allocator = allocator;
}

u8* dn_dynamic_array_at(dn_dynamic_array_t* buffer, u32 index) {
  DN_ASSERT(buffer);
  return buffer->data + (index * buffer->element_size);
}

u8* dn_dynamic_array_push(dn_dynamic_array_t* buffer, void* data, u32 count) {
  DN_ASSERT(buffer);

  auto reserved = dn_dynamic_array_reserve(buffer, count);
  if (data) dn_os_memory_copy(data, reserved, buffer->element_size * count);
  return reserved;
}

u8* dn_dynamic_array_reserve(dn_dynamic_array_t* buffer, u32 count) {
  DN_ASSERT(buffer);

  dn_dynamic_array_grow(buffer, buffer->size + count);

  auto element = dn_dynamic_array_at(buffer, buffer->size);
  buffer->size += count;
  return element;
}

void dn_dynamic_array_clear(dn_dynamic_array_t* buffer) {
  DN_ASSERT(buffer);

  buffer->size = 0;
}

u32 dn_dynamic_array_byte_size(dn_dynamic_array_t* buffer) {
  DN_ASSERT(buffer);

  return buffer->size * buffer->element_size;
}

void dn_dynamic_array_grow(dn_dynamic_array_t* buffer, u32 capacity) {
  DN_ASSERT(buffer);

  if (buffer->capacity >= capacity) return;
  buffer->capacity = dn_max(buffer->capacity * 2, capacity);
  buffer->data = (u8*)dn_allocator_realloc(buffer->allocator, buffer->data, buffer->capacity * buffer->element_size);
}
#endif