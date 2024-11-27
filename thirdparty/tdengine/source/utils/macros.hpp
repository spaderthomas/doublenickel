#define DN_MAX(a, b) (a) > (b) ? (a) : (b)
#define DN_MIN(a, b) (a) > (b) ? (b) : (a)

#define dn_for(it, n) for (u32 it = 0; it < (n); it++)

#define DN_API extern "C" __declspec(dllexport)
#define DN_IMP

#define DN_ALIGN(n) __declspec(align(n))
#define DN_ASSERT(condition) assert(condition)

#define dn_zero_initialize() { 0 };

#define DN_ARR_LEN(carray) (sizeof((carray)) / sizeof((carray)[0]))