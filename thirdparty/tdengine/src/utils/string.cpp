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