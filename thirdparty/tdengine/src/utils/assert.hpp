#define fm_assert(expr) do { \
	if (!(expr)) { \
		__debugbreak(); \
		printf("ASSERT: %s\n", __func__); \
	} \
} while (0)
