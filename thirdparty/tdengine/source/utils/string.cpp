char* copy_string(const_string str, u32 length, MemoryAllocator* allocator) {
	if (!allocator) allocator = &standard_allocator;

	auto buffer_length = length + 1;
	auto copy = allocator->alloc<char>(buffer_length);
	copy_string_n(str, length, copy, buffer_length);
	return copy;
}

char* copy_string(const_string str, MemoryAllocator* allocator) {
	return copy_string(str, strlen(str), allocator);
}

char* copy_string(const std::string& str, MemoryAllocator* allocator) {
	return copy_string(str.c_str(), str.length(), allocator);
}

char* copy_string_u8(const u8* str, u32 length, MemoryAllocator* allocator) {
	return copy_string(reinterpret_cast<const char*>(str), length, allocator);
}


void copy_string(const_string str, string buffer, u32 buffer_length) {
	return copy_string_n(str, strlen(str), buffer, buffer_length);
}

void copy_string_n(const_string str, u32 length, string buffer, u32 buffer_length) {
	if (!str) return;
	if (!buffer) return;
	if (!buffer_length) return;

	auto copy_length = std::min(length, buffer_length - 1);
	for (u32 i = 0; i < copy_length; i++) {
		buffer[i] = str[i];
	}
	buffer[copy_length] = '\0';
}

void string_builder_grow(StringBuilder* builder, u32 requested_capacity) {
	if (builder->buffer.capacity > requested_capacity) return;

	builder->buffer.data = builder->allocator->realloc<u8>(builder->buffer.data, requested_capacity);
	builder->buffer.capacity = requested_capacity;
}

void string_builder_append_cstr(StringBuilder* builder, const char* str) {
	auto buffer = &builder->buffer;

	u32 len = strlen(str);
	string_builder_grow(builder, buffer->count + len);
	copy_memory(str, builder->buffer.data + builder->buffer.count, len);
	builder->buffer.count += len;
}

String string_builder_write(StringBuilder* builder) {
	String string = {
		.data = builder->allocator->alloc<u8>(builder->buffer.count),
		.len = builder->buffer.count
	};

	copy_memory(builder->buffer.data, string.data, builder->buffer.count);
	return string;
}

char* string_builder_write_cstr(StringBuilder* builder) {
	return copy_string((char*)builder->buffer.data, builder->buffer.count, builder->allocator);
}
