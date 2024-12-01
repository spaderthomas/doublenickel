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
DN_IMP char*       dn_cstr_copy(const char* str, u32 length, dn_allocator_t* allocator = nullptr);
DN_IMP char*       dn_cstr_copy(const char* str, dn_allocator_t* allocator = nullptr);
DN_IMP char*       dn_cstr_copy(const std::string& str, dn_allocator_t* allocator = nullptr);
DN_IMP char*       dn_cstr_copy_u8(const u8* str, u32 length, dn_allocator_t* allocator = nullptr);
DN_API void        dn_cstr_copy(const char* str, char* buffer, u32 buffer_length);
DN_API void        dn_cstr_copy_n(const char* str, u32 length, char* buffer, u32 buffer_length);
DN_API char*       dn_string_to_cstr(dn_string_t str);
DN_API char*       dn_string_to_cstr_ex(dn_string_t str, dn_allocator_t* allocator);
DN_API bool        dn_string_equal(dn_string_t a, dn_string_t b);
DN_API bool        dn_string_equal_cstr(dn_string_t a, const char* b);
DN_API bool        dn_cstr_equal(const char* a, const char* b);
DN_API u32         dn_cstr_len(const char* str);
DN_API dn_string_t dn_string_copy_std(const std::string& str, dn_allocator_t* allocator);
DN_API dn_string_t dn_string_copy_cstr(const char* str, dn_allocator_t* allocator);
DN_API dn_string_t dn_string_copy(dn_string_t str, dn_allocator_t* allocator);
#endif



#ifdef DN_STRING_IMPLEMENTATION
char* dn_cstr_copy(const char* str, u32 length, dn_allocator_t* allocator) {
  if (!allocator) allocator = &dn_allocators.standard;

  auto buffer_length = length + 1;
  auto copy = dn::allocator::alloc<char>(allocator, buffer_length);
  dn_cstr_copy_n(str, length, copy, buffer_length);
  return copy;
}

char* dn_cstr_copy(const char* str, dn_allocator_t* allocator) {
  return dn_cstr_copy(str, dn_cstr_len(str), allocator);
}

char* dn_cstr_copy(const std::string& str, dn_allocator_t* allocator) {
  return dn_cstr_copy(str.c_str(), str.length(), allocator);
}

char* dn_cstr_copy_u8(const u8* str, u32 length, dn_allocator_t* allocator) {
  return dn_cstr_copy(reinterpret_cast<const char*>(str), length, allocator);
}


void dn_cstr_copy(const char* str, char* buffer, u32 buffer_length) {
  return dn_cstr_copy_n(str, dn_cstr_len(str), buffer, buffer_length);
}

char* dn_string_to_cstr(dn_string_t str) {
  return dn_string_to_cstr_ex(str, &dn_allocators.bump);
}

char* dn_string_to_cstr_ex(dn_string_t str, dn_allocator_t* allocator) {
  return dn_cstr_copy((char*)str.data, str.len, allocator);
}

bool dn_string_equal(dn_string_t a, dn_string_t b) {
  if (a.len != b.len) return false;

  return dn_os_is_memory_equal(a.data, b.data, a.len);
}

bool dn_string_equal_cstr(dn_string_t a, const char* b) {
  u32 len = dn_cstr_len(b);
  if (a.len != len) return false;

  return dn_os_is_memory_equal(a.data, b, len);
}

bool dn_cstr_equal(const char* a, const char* b) {
  u32 len_a = dn_cstr_len(a);
  u32 len_b = dn_cstr_len(b);
  if (len_a != len_b) return false;

  return dn_os_is_memory_equal(a, b, len_a);
}

u32 dn_cstr_len(const char* str) {
  u32 len = 0;
  if (str) {
    while (str[len]) len++;
  }

  return len;
}

dn_string_t dn_string_copy_std(const std::string& str, dn_allocator_t* allocator) {
  dn_string_t copy = {
    dn::allocator::alloc<u8>(allocator, str.size()),
    (u32)str.size()
  };

  dn_os_memory_copy(str.c_str(), copy.data, str.size());
  return copy;
}

dn_string_t dn_string_copy_cstr(const char* str, dn_allocator_t* allocator) {
  dn_string_t copy;
  copy.len = dn_cstr_len(str);
  copy.data = dn::allocator::alloc<u8>(allocator, copy.len);

  dn_os_memory_copy(str, copy.data, copy.len);
  return copy;
}

dn_string_t dn_string_copy(dn_string_t str, dn_allocator_t* allocator) {
  dn_string_t copy = {
    dn::allocator::alloc<u8>(allocator, str.len),
    str.len
  };

  dn_os_memory_copy(str.data, copy.data, str.len);
  return copy;
}

void dn_cstr_copy_n(const char* str, u32 length, char* buffer, u32 buffer_length) {
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
  dn_string_builder_grow(builder, builder->buffer.count + str.len);
  dn_os_memory_copy(str.data, builder->buffer.data + builder->buffer.count, str.len);
  builder->buffer.count += str.len;
}

void dn_string_builder_append_cstr(dn_string_builder_t* builder, const char* str) {
  u32 len = dn_cstr_len(str);
  dn_string_builder_grow(builder, builder->buffer.count + len);
  dn_os_memory_copy(str, builder->buffer.data + builder->buffer.count, len);
  builder->buffer.count += len;
}

void dn_string_builder_append_fmt(dn_string_builder_t* builder, dn_string_t fmt, ...) {
  va_list args, len_args;
  va_start(args, fmt);
  va_copy(len_args, args);

  const char* format = dn_string_to_cstr(fmt);
  u32 fmt_len = vsnprintf(NULL, 0, format, len_args);
  u32 vsnprintf_len = fmt_len + 1;
  dn_string_builder_grow(builder, builder->buffer.count + vsnprintf_len); // vsnprintf() null terminates, so ensure there's enough room (even though we don't care)
  vsnprintf((char*)builder->buffer.data + builder->buffer.count, vsnprintf_len, format, args);
  builder->buffer.count += fmt_len;

  va_end(args);
  va_end(args);
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
  return dn_cstr_copy((char*)builder->buffer.data, builder->buffer.count, builder->allocator);
}
#endif