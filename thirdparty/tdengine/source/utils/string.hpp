#ifndef DN_STRING_H
#define DN_STRING_H
typedef char* dn_tstring_t;

typedef struct {
  u8* data;
  u32 len;
} dn_string_t;

typedef struct {
  struct {
    u8* data;
    u32 count;
    u32 capacity;
  } buffer;

  dn_allocator_t* allocator;
} dn_string_builder_t;

#define dn_string_literal(s) { .data = (u8*)(s), .len = sizeof(s) - 1}

DN_API void        dn_string_builder_grow(dn_string_builder_t* builder);
DN_API void        dn_string_builder_append(dn_string_builder_t* builder, dn_string_t str);
DN_API void        dn_string_builder_append_cstr(dn_string_builder_t* builder, const char* str);
DN_API void        dn_string_builder_append_fmt(dn_string_builder_t* builder, dn_string_t fmt, ...);
DN_API dn_string_t dn_string_builder_write(dn_string_builder_t* builder);
DN_API char*       dn_string_builder_write_cstr(dn_string_builder_t* builder);
DN_IMP char*       dn_string_copy(const char* str, u32 length, dn_allocator_t* allocator = nullptr);
DN_IMP char*       dn_string_copy(const char* str, dn_allocator_t* allocator = nullptr);
DN_IMP char*       dn_string_copy(const std::string& str, dn_allocator_t* allocator = nullptr);
DN_IMP char*       dn_string_copy_u8(const u8* str, u32 length, dn_allocator_t* allocator = nullptr);
DN_API void        dn_string_copy(const char* str, char* buffer, u32 buffer_length);
DN_API void        dn_string_copy_n(const char* str, u32 length, char* buffer, u32 buffer_length);
DN_API char*       dn_string_to_cstr(dn_string_t str, dn_allocator_t* allocator);
#endif



#ifdef DN_STRING_IMPLEMENTATION
char* dn_string_copy(const char* str, u32 length, dn_allocator_t* allocator) {
  if (!allocator) allocator = &dn_allocators.standard;

  auto buffer_length = length + 1;
  auto copy = dn::allocator::alloc<char>(allocator, buffer_length);
  dn_string_copy_n(str, length, copy, buffer_length);
  return copy;
}

char* dn_string_copy(const char* str, dn_allocator_t* allocator) {
  return dn_string_copy(str, strlen(str), allocator);
}

char* dn_string_copy(const std::string& str, dn_allocator_t* allocator) {
  return dn_string_copy(str.c_str(), str.length(), allocator);
}

char* dn_string_copy_u8(const u8* str, u32 length, dn_allocator_t* allocator) {
  return dn_string_copy(reinterpret_cast<const char*>(str), length, allocator);
}


void dn_string_copy(const char* str, char* buffer, u32 buffer_length) {
  return dn_string_copy_n(str, strlen(str), buffer, buffer_length);
}

char* dn_string_to_cstr(dn_string_t str, dn_allocator_t* allocator) {
  DN_UNTESTED();
  return nullptr;
}

void dn_string_copy_n(const char* str, u32 length, char* buffer, u32 buffer_length) {
  if (!str) return;
  if (!buffer) return;
  if (!buffer_length) return;

  auto copy_length = std::min(length, buffer_length - 1);
  for (u32 i = 0; i < copy_length; i++) {
    buffer[i] = str[i];
  }
  buffer[copy_length] = '\0';
}

void dn_string_builder_grow(dn_string_builder_t* builder, u32 requested_capacity) {
  if (builder->buffer.capacity > requested_capacity) return;

  builder->buffer.data = dn::allocator::realloc<u8>(builder->allocator, builder->buffer.data, requested_capacity);
  builder->buffer.capacity = requested_capacity;
}

void dn_string_builder_append(dn_string_builder_t* builder, dn_string_t str) {
  DN_UNTESTED();
}

void dn_string_builder_append_cstr(dn_string_builder_t* builder, const char* str) {
  auto buffer = &builder->buffer;

  u32 len = strlen(str);
  dn_string_builder_grow(builder, buffer->count + len);
  dn_os_memory_copy(str, builder->buffer.data + builder->buffer.count, len);
  builder->buffer.count += len;
}

void dn_string_builder_append_fmt(dn_string_builder_t* builder, dn_string_t fmt, ...) {
  
}

dn_string_t dn_string_builder_write(dn_string_builder_t* builder) {
  dn_string_t string = {
    .data = dn::allocator::alloc<u8>(builder->allocator, builder->buffer.count),
    .len = builder->buffer.count
  };

  dn_os_memory_copy(builder->buffer.data, string.data, builder->buffer.count);
  return string;
}

char* dn_string_builder_write_cstr(dn_string_builder_t* builder) {
  return dn_string_copy((char*)builder->buffer.data, builder->buffer.count, builder->allocator);
}
#endif