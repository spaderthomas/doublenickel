#ifndef DN_STRING_H
#define DN_STRING_H
typedef char* tstring;

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

void   dn_string_builder_grow(dn_string_builder_t* builder);
void   dn_string_builder_append_cstr(dn_string_builder_t* builder, const char* str);
dn_string_t dn_string_builder_write(dn_string_builder_t* builder);
char*  dn_string_builder_write_cstr(dn_string_builder_t* builder);

#define STRING_LITERAL(s) (dn_string_t) { .data = (u8*)(s), .len = sizeof(s) - 1}
#define AS_CSTR(s) (char*)((s).data)

char* copy_string(const char* str, u32 length, dn_allocator_t* allocator = nullptr);
char* copy_string(const char* str, dn_allocator_t* allocator = nullptr);
char* copy_string(const std::string& str, dn_allocator_t* allocator = nullptr);
char* copy_string_u8(const u8* str, u32 length, dn_allocator_t* allocator = nullptr);

DN_API void copy_string(const char* str, char* buffer, u32 buffer_length);
DN_API void copy_string_n(const char* str, u32 length, char* buffer, u32 buffer_length);

void copy_memory(const void* source, void* dest, u32 num_bytes);
bool is_memory_equal(void* a, void* b, size_t len);
void fill_memory(void* buffer, u32 buffer_size, void* fill, u32 fill_size);
void fill_memory_u8(void* buffer, u32 buffer_size, u8 fill);
void zero_memory(void* buffer, u32 buffer_size);
#endif

#ifdef DN_STRING_IMPLEMENTATION
char* copy_string(const char* str, u32 length, dn_allocator_t* allocator) {
  if (!allocator) allocator = &standard_allocator;

  auto buffer_length = length + 1;
  auto copy = allocator->alloc<char>(buffer_length);
  copy_string_n(str, length, copy, buffer_length);
  return copy;
}

char* copy_string(const char* str, dn_allocator_t* allocator) {
  return copy_string(str, strlen(str), allocator);
}

char* copy_string(const std::string& str, dn_allocator_t* allocator) {
  return copy_string(str.c_str(), str.length(), allocator);
}

char* copy_string_u8(const u8* str, u32 length, dn_allocator_t* allocator) {
  return copy_string(reinterpret_cast<const char*>(str), length, allocator);
}


void copy_string(const char* str, char* buffer, u32 buffer_length) {
  return copy_string_n(str, strlen(str), buffer, buffer_length);
}

void copy_string_n(const char* str, u32 length, char* buffer, u32 buffer_length) {
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

  builder->buffer.data = builder->allocator->realloc<u8>(builder->buffer.data, requested_capacity);
  builder->buffer.capacity = requested_capacity;
}

void dn_string_builder_append_cstr(dn_string_builder_t* builder, const char* str) {
  auto buffer = &builder->buffer;

  u32 len = strlen(str);
  dn_string_builder_grow(builder, buffer->count + len);
  copy_memory(str, builder->buffer.data + builder->buffer.count, len);
  builder->buffer.count += len;
}

dn_string_t dn_string_builder_write(dn_string_builder_t* builder) {
  dn_string_t string = {
    .data = builder->allocator->alloc<u8>(builder->buffer.count),
    .len = builder->buffer.count
  };

  copy_memory(builder->buffer.data, string.data, builder->buffer.count);
  return string;
}

char* dn_string_builder_write_cstr(dn_string_builder_t* builder) {
  return copy_string((char*)builder->buffer.data, builder->buffer.count, builder->allocator);
}

bool is_memory_equal(void* a, void* b, size_t len) {
    return 0 == memcmp(a, b, len);
}

void copy_memory(const void* source, void* dest, u32 num_bytes) {
    std::memcpy(dest, source, num_bytes);
}

void fill_memory(void* buffer, u32 buffer_size, void* fill, u32 fill_size) {
  u8* current_byte = (u8*)buffer;

  int i = 0;
  while (true) {
    if (i + fill_size > buffer_size) return;
    memcpy(current_byte + i, (u8*)fill, fill_size);
    i += fill_size;
  }
}

void fill_memory_u8(void* buffer, u32 buffer_size, u8 fill) {
  fill_memory(buffer, buffer_size, &fill, sizeof(u8));
}

void zero_memory(void* buffer, u32 buffer_size) {
  fill_memory_u8(buffer, buffer_size, 0);
}
#endif