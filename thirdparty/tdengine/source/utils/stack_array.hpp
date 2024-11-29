template<typename T, int N>
struct StackArray {
	int32 size = 0;
	T data [N] = {};

	void clear() {
		memset(data, 0, sizeof(T) * N);
		size = 0;
	}
	
	void push(T element) {
		if (size >= N) {
			DN_ASSERT(!"Overrunning stack array");
		}
		DN_ASSERT(size < N);
		data[size++] = element;
	}

	T* begin() { return &data[0]; }
	const T* begin() const { return &data[0]; }
	T* end() { return &data[size]; }
	const T* end() const { return &data[size]; }
};
