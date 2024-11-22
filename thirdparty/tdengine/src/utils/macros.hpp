#define fox_max(a, b) (a) > (b) ? (a) : (b)
#define fox_min(a, b) (a) > (b) ? (b) : (a)
#define fox_for(iterName, iterCount) for (unsigned int iterName = 0; iterName < (iterCount); ++iterName)
#define fox_iter(iter_name, container) for (auto iter_name = (container).begin(); (iter_name) != (container).end(); (iter_name)++)
#define EXIT_IF_ERROR(return_code) if ((return_code)) { return -1; }
#define rand_float(max) (static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (max)))

// All Lua functions have to be declared as extern C! Otherwise, they'll get name mangled,
// and LuaJIT cannot find them when you declare them with ffi.cdef()
#define FM_LUA_EXPORT extern "C" __declspec(dllexport)

#define TD_ALIGN(n) __declspec(align(n))

// #define TD_ASSERT(condition) assert(condition)
#define TD_ASSERT(condition)
