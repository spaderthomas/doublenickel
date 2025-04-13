#include "stdarg.h"
#include <string.h>

#ifndef gs_inline
    #define gs_inline static inline
#endif

#ifndef gs_local_persist
    #define gs_local_persist static
#endif

#ifndef gs_global
    #define gs_global static
#endif

 #if (defined _WIN32 || defined _WIN64)
    #define gs_force_inline gs_inline
#elif (defined __APPLE__ || defined _APPLE)
    #define gs_force_inline static __attribute__((always_inline))
#else
    #define gs_force_inline gs_inline
#endif

#define GS_INLINE           gs_force_inline
#define GS_GLOBAL           gs_global
#define GS_LOCAL_PERSIST    gs_local_persist
#ifdef GS_API_DLL_EXPORT
    #ifdef __cplusplus
        #define GS_API_EXTERN extern "C" __declspec(dllexport)
    #else
        #define GS_API_EXTERN extern __declspec(dllexport)
    #endif
#else
    #ifdef __cplusplus
        #define GS_API_EXTERN extern "C"
    #else
        #define GS_API_EXTERN extern
    #endif
#endif

#define GS_API_DECL     GS_API_EXTERN
#define GS_API_PRIVATE  GS_API_EXTERN 

/*============================================================
// C primitive types
============================================================*/

#ifndef __cplusplus
        #define false     0
        #define true      1
#endif

#ifdef __cplusplus
        typedef bool      b8;
#else
    #ifndef __bool_true_false_are_defined
        typedef _Bool     bool;
    #endif
        typedef bool      b8;
#endif

typedef size_t            usize;

typedef uint8_t           u8;
typedef int8_t            s8;
typedef uint16_t          u16;
typedef int16_t           s16;
typedef uint32_t          u32;
typedef int32_t           s32;
typedef s32               b32;
typedef uint64_t          u64;
typedef int64_t           s64;
typedef float             f32;
typedef double            f64;
typedef const char*       const_str;

typedef int32_t           bool32_t;
typedef float             float32_t;
typedef double            float64_t;

typedef bool32_t          bool32;

#define uint16_max        UINT16_MAX
#define uint32_max        UINT32_MAX
#define int32_max         INT32_MAX
#define float_max         FLT_MAX
#define float_min         FLT_MIN

/*============================================================
// gs utils
============================================================*/

/** @defgroup gs_util Common Utils
 *  Gunslinger Common Utils
 *  @{
 */

// Helper macro for compiling to nothing
#define gs_empty_instruction(...)

#define gs_array_size(__ARR) sizeof(__ARR) / sizeof(__ARR[0])

#ifndef gs_assert
    #define gs_assert assert
#endif
    
#if defined (__cplusplus)
    #define gs_default_val() {}
#else
    #define gs_default_val() {0}
#endif

// Helper macro for an in place for-range loop
#define gs_for_range_i(__COUNT)\
    for (uint32_t i = 0; i < __COUNT; ++i)

// Helper macro for an in place for-range loop
#define gs_for_range_j(__COUNT)\
    for (uint32_t j = 0; j < __COUNT; ++j)

#define gs_for_range(__COUNT)\
    for(uint32_t gs_macro_token_paste(__T, __LINE__) = 0;\
        gs_macro_token_paste(__T, __LINE__) < __COUNT;\
        ++(gs_macro_token_paste(__T, __LINE__)))

#define gs_max(A, B) ((A) > (B) ? (A) : (B))

#define gs_min(A, B) ((A) < (B) ? (A) : (B))

#define gs_clamp(V, MIN, MAX) ((V) > (MAX) ? (MAX) : (V) < (MIN) ? (MIN) : (V))

#define gs_is_nan(V) ((V) != (V))

// Helpful macro for casting one type to another
#define gs_cast(A, B) ((A*)(B))

#ifdef __cplusplus
    #define gs_ctor(TYPE, ...) (TYPE {__VA_ARGS__})
#else 
    #define gs_ctor(TYPE, ...) ((TYPE){__VA_ARGS__})
#endif

// Helpful marco for calculating offset (in bytes) of an element from a given structure type
#define gs_offset(TYPE, ELEMENT) ((size_t)(&(((TYPE*)(0))->ELEMENT)))

// macro for turning any given type into a const char* of itself
#define gs_to_str(TYPE) ((const char*)#TYPE)

#define gs_macro_token_paste(X, Y) X##Y
#define gs_macro_cat(X, Y) gs_macro_token_paste(X, Y)

#define gs_timed_action(INTERVAL, ...)\
    do {\
        static uint32_t gs_macro_cat(gs_macro_cat(__T, __LINE__), t) = 0;\
        if (gs_macro_cat(gs_macro_cat(__T, __LINE__), t)++ > INTERVAL) {\
            gs_macro_cat(gs_macro_cat(__T, __LINE__), t) = 0;\
            __VA_ARGS__\
        }\
    } while (0)

#define gs_int2voidp(I) (void*)(uintptr_t)(I)

#define gs_if(INIT, CHECK, ...)\
    do {\
        INIT;\
        if (CHECK)\
        {\
            __VA_ARGS__\
        }\
    } while (0) 

/*===================================
// String Utils
===================================*/

gs_force_inline uint32_t 
gs_string_length(const char* txt)
{
    uint32_t sz = 0;
    while (txt != NULL && txt[ sz ] != '\0') 
    {
        sz++;
    }
    return sz;
}

#define gs_strlen gs_string_length

// Expects null terminated strings
gs_force_inline b32 
gs_string_compare_equal
(
    const char*     txt, 
    const char*     cmp 
)
{
    // Grab sizes of both strings
    uint32_t a_sz = gs_string_length(txt);
    uint32_t b_sz = gs_string_length(cmp);

    // Return false if sizes do not match
    if (a_sz != b_sz) 
    {
        return false;
    }

    for(uint32_t i = 0; i < a_sz; ++i) 
    {
        if (*txt++ != *cmp++)
        {
            return false;
        }
    };

    return true;
}

gs_force_inline b32 
gs_string_compare_equal_n
(
    const char* txt, 
    const char* cmp, 
    uint32_t n 
)
{
    uint32_t a_sz = gs_string_length(txt);
    uint32_t b_sz = gs_string_length(cmp);

    // Not enough characters to do operation
    if (a_sz < n || b_sz < n)
    {
        return false;
    }

    for (uint32_t i = 0; i < n; ++i) 
    {
        if (*txt++ != *cmp++)
        {
            return false;
        }
    };

    return true;
}


//=== Logging ===//

#define gs_log_info(MESSAGE, ...) gs_println("LOG::%s::%s(%zu)::" MESSAGE, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define gs_log_success(MESSAGE, ...) gs_println("SUCCESS::%s::%s(%zu)::" MESSAGE, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define gs_log_warning(MESSAGE, ...) gs_println("WARNING::%s::%s(%zu)::" MESSAGE, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define gs_log_error(MESSAGE, ...) do {gs_println("ERROR::%s::%s(%zu)::" MESSAGE, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
                                        gs_assert(false);\
                                    } while (0)

#define gs_println(__FMT, ...)\
    do {\
        gs_printf(__FMT, ## __VA_ARGS__);\
        gs_printf("\n");\
    } while (0)

#ifndef gs_fprintf
    gs_force_inline
    void gs_fprintf
    (
        FILE* fp,
        const char* fmt,
        ...
    )
    {
        va_list args;
        va_start (args, fmt);
        vfprintf(fp, fmt, args);
        va_end(args);
    }
#endif

gs_force_inline 
void gs_fprintln
(
    FILE* fp, 
    const char* fmt, 
    ... 
)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    va_end(args);
    gs_fprintf(fp, "\n");
}

gs_force_inline
void gs_fprintln_t
(
	FILE* fp, 
	uint32_t tabs,
	const char* fmt, 
	...
)
{
    va_list args;
    va_start(args, fmt);
	for (uint32_t i = 0; i < tabs; ++i)
	{
		gs_fprintf(fp, "\t");
	}
    vfprintf(fp, fmt, args);
    va_end(args);
    gs_fprintf(fp, "\n");
}

#ifdef __MINGW32__
#define gs_snprintf(__NAME, __SZ, __FMT, ...) __mingw_snprintf(__NAME, __SZ, __FMT, ## __VA_ARGS__)
#else
gs_force_inline 
void gs_snprintf
(
    char* buffer, 
    size_t buffer_size, 
    const char* fmt, 
    ... 
)
{
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, buffer_size, fmt, args);
    va_end(args);
}
#endif

/*===================================
// Memory Allocation Utils
===================================*/

// Operating system function pointer
typedef struct gs_os_api_s
{ 
    void* (* malloc)(size_t sz);
    void  (* free)(void* ptr);
    void* (* realloc)(void* ptr, size_t sz);
    void* (* calloc)(size_t num, size_t sz);
    void* (* alloca)(size_t sz);
    void* (* malloc_init)(size_t sz);
} gs_os_api_t;

// TODO(john): Check if all defaults need to be set, in case gs context will not be used

GS_API_DECL 
void* _gs_malloc_init_impl(size_t sz);

// Default memory allocations
#ifndef GS_NO_OS_MEMORY_ALLOC_DEFAULT
    #define gs_malloc           malloc 
    #define gs_free             free 
    #define gs_realloc          realloc 
    #define gs_calloc           calloc 
    #define gs_alloca           malloc
    #define gs_malloc_init(__T) (__T*)_gs_malloc_init_impl(sizeof(__T))
#endif 

GS_API_DECL gs_os_api_t
gs_os_api_new_default(); 

#ifndef gs_os_api_new
    #define gs_os_api_new gs_os_api_new_default
#endif 

#ifndef gs_malloc
    #define gs_malloc(__SZ) (gs_ctx()->os.malloc(__SZ))
#endif

#ifndef gs_malloc_init
	#define gs_malloc_init(__T) ((__T*)gs_ctx()->os.malloc_init(sizeof(__T)))
#endif 

#ifndef gs_free
    #define gs_free(__MEM) (gs_ctx()->os.free(__MEM))
#endif

#ifndef gs_realloc
    #define gs_realloc(__MEM, __AZ) (gs_ctx()->os.realloc(__MEM, __AZ))
#endif

#ifndef gs_calloc
    #define gs_calloc(__NUM, __SZ) (gs_ctx()->os.calloc(__NUM, __SZ))
#endif

#ifndef gs_alloca
    #define gs_alloca(__SZ) (gs_ctx()->os.alloca(__SZ))
#endif 


// Modified from: https://stackoverflow.com/questions/11815894/how-to-read-write-arbitrary-bits-in-c-c
#define gs_bit_mask(INDEX, SIZE)\
    (((1u << (SIZE)) - 1u) << (INDEX))

#define gs_write_bits(DATA, INDEX, SIZE, VAL)\
    ((DATA) = (((DATA) & (~BIT_MASK((INDEX), (SIZE)))) | (((VAL) << (INDEX)) & (BIT_MASK((INDEX), (SIZE))))))

#define gs_read_bits(DATA, INDEX, SIZE)\
    (((DATA) & BIT_MASK((INDEX), (SIZE))) >> (INDEX))

/*============================================================
// Result
============================================================*/

typedef enum gs_result
{
    GS_RESULT_SUCCESS,
    GS_RESULT_IN_PROGRESS,
    GS_RESULT_INCOMPLETE,
    GS_RESULT_FAILURE
} gs_result;

#define gs_transient_buffer(__N, __SZ)\
    char __N[__SZ] = gs_default_val();\
    memset(__N, 0, __SZ);

#define gs_snprintfc(__NAME, __SZ, __FMT, ...)\
    char __NAME[__SZ] = gs_default_val();\
    gs_snprintf(__NAME, __SZ, __FMT, ## __VA_ARGS__); 

gs_force_inline
uint32_t gs_util_safe_truncate_u64(uint64_t value)
{
  gs_assert(value <= 0xFFFFFFFF); 
  uint32_t result = (uint32_t)value;
  return result;
}

gs_force_inline 
uint32_t gs_hash_uint32_t(uint32_t x)
{
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

#define gs_hash_u32_ip(__X, __OUT)\
    do {\
        __OUT = ((__X >> 16) ^ __X) * 0x45d9f3b;\
        __OUT = ((__OUT >> 16) ^ __OUT) * 0x45d9f3b;\
        __OUT = (__OUT >> 16) ^ __OUT;\
    } while (0) 

gs_force_inline 
uint32_t gs_hash_u64(uint64_t x)
{
    x = (x ^ (x >> 31) ^ (x >> 62)) * UINT64_C(0x319642b2d24d8ec3);
    x = (x ^ (x >> 27) ^ (x >> 54)) * UINT64_C(0x96de1b173f119089);
    x = x ^ (x >> 30) ^ (x >> 60);
    return (uint32_t)x; 
}

// Note(john): source: http://www.cse.yorku.ca/~oz/hash.html
// djb2 hash by dan bernstein
gs_force_inline 
uint32_t gs_hash_str(const char* str)
{
    uint32_t hash = 5381;
    s32 c;
    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

gs_force_inline 
uint64_t gs_hash_str64(const char* str)
{
    uint32_t hash1 = 5381;
    uint32_t hash2 = 52711;
    uint32_t i = gs_string_length(str);
    while(i--) 
    {
        char c = str[ i ];
        hash1 = (hash1 * 33) ^ c;
        hash2 = (hash2 * 33) ^ c;
    }

    return (hash1 >> 0) * 4096 + (hash2 >> 0);
}

gs_force_inline
bool gs_compare_bytes(void* b0, void* b1, size_t len)
{
    return 0 == memcmp(b0, b1, len);
}

// Hash generic bytes using (ripped directly from Sean Barret's stb_ds.h)
#define GS_SIZE_T_BITS  ((sizeof(size_t)) * 8)
#define GS_SIPHASH_C_ROUNDS 1
#define GS_SIPHASH_D_ROUNDS 1
#define gs_rotate_left(__V, __N)   (((__V) << (__N)) | ((__V) >> (GS_SIZE_T_BITS - (__N))))
#define gs_rotate_right(__V, __N)  (((__V) >> (__N)) | ((__V) << (GS_SIZE_T_BITS - (__N))))

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant, for do..while(0) and sizeof()==
#endif

gs_force_inline 
size_t gs_hash_siphash_bytes(void *p, size_t len, size_t seed)
{
  unsigned char *d = (unsigned char *) p;
  size_t i,j;
  size_t v0,v1,v2,v3, data;

  // hash that works on 32- or 64-bit registers without knowing which we have
  // (computes different results on 32-bit and 64-bit platform)
  // derived from siphash, but on 32-bit platforms very different as it uses 4 32-bit state not 4 64-bit
  v0 = ((((size_t) 0x736f6d65 << 16) << 16) + 0x70736575) ^  seed;
  v1 = ((((size_t) 0x646f7261 << 16) << 16) + 0x6e646f6d) ^ ~seed;
  v2 = ((((size_t) 0x6c796765 << 16) << 16) + 0x6e657261) ^  seed;
  v3 = ((((size_t) 0x74656462 << 16) << 16) + 0x79746573) ^ ~seed;

  #ifdef STBDS_TEST_SIPHASH_2_4
  // hardcoded with key material in the siphash test vectors
  v0 ^= 0x0706050403020100ull ^  seed;
  v1 ^= 0x0f0e0d0c0b0a0908ull ^ ~seed;
  v2 ^= 0x0706050403020100ull ^  seed;
  v3 ^= 0x0f0e0d0c0b0a0908ull ^ ~seed;
  #endif

  #define gs_sipround() \
    do {                   \
      v0 += v1; v1 = gs_rotate_left(v1, 13);  v1 ^= v0; v0 = gs_rotate_left(v0,GS_SIZE_T_BITS/2); \
      v2 += v3; v3 = gs_rotate_left(v3, 16);  v3 ^= v2;                                                 \
      v2 += v1; v1 = gs_rotate_left(v1, 17);  v1 ^= v2; v2 = gs_rotate_left(v2,GS_SIZE_T_BITS/2); \
      v0 += v3; v3 = gs_rotate_left(v3, 21);  v3 ^= v0;                                                 \
    } while (0)

  for (i=0; i+sizeof(size_t) <= len; i += sizeof(size_t), d += sizeof(size_t)) {
    data = d[0] | (d[1] << 8) | (d[2] << 16) | (d[3] << 24);
    data |= (size_t) (d[4] | (d[5] << 8) | (d[6] << 16) | (d[7] << 24)) << 16 << 16; // discarded if size_t == 4

    v3 ^= data;
    for (j=0; j < GS_SIPHASH_C_ROUNDS; ++j)
      gs_sipround();
    v0 ^= data;
  }
  data = len << (GS_SIZE_T_BITS-8);
  switch (len - i) {
    case 7: data |= ((size_t) d[6] << 24) << 24; // fall through
    case 6: data |= ((size_t) d[5] << 20) << 20; // fall through
    case 5: data |= ((size_t) d[4] << 16) << 16; // fall through
    case 4: data |= (d[3] << 24); // fall through
    case 3: data |= (d[2] << 16); // fall through
    case 2: data |= (d[1] << 8); // fall through
    case 1: data |= d[0]; // fall through
    case 0: break;
  }
  v3 ^= data;
  for (j=0; j < GS_SIPHASH_C_ROUNDS; ++j)
    gs_sipround();
  v0 ^= data;
  v2 ^= 0xff;
  for (j=0; j < GS_SIPHASH_D_ROUNDS; ++j)
    gs_sipround();

#if 0
  return v0^v1^v2^v3;
#else
  return v1^v2^v3; // slightly stronger since v0^v3 in above cancels out final round operation? I tweeted at the authors of SipHash about this but they didn't reply
#endif
}

gs_force_inline
size_t gs_hash_murmur3(void *p, size_t len, size_t seed)
{
    const uint8_t* data = (const uint8_t*)p;
    size_t h = seed ^ len;
    for (uint32_t i = 0; i < len / 4; ++i) {
        uint32_t k = *(uint32_t*)(data + i * 4);
        k *= 0xcc9e2d51;
        k = (k << 15) | (k >> (32 - 15));
        k *= 0x1b873593;
        h ^= k;
        h = (h << 13) | (h >> (32 - 13));
        h = h * 5 + 0xe6546b64;
    }

    // Handle remaining bytes
    const uint8_t* tail = data + (len & ~3);
    uint32_t k1 = 0;

    switch (len & 3) {
        case 3: k1 ^= tail[2] << 16;
        case 2: k1 ^= tail[1] << 8;
        case 1: k1 ^= tail[0];
                k1 *= 0xcc9e2d51;
                k1 = (k1 << 15) | (k1 >> (32 - 15));
                k1 *= 0x1b873593;
                h ^= k1;
    }
    
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h; 
}

gs_force_inline
size_t gs_hash_bytes(void *p, size_t len, size_t seed)
{
#if 1
  return gs_hash_siphash_bytes(p, len, seed);
#else 
  return gs_hash_murmur3(p, len, seed);
#endif
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif

/* Resource Loading Util */
GS_API_DECL bool32_t gs_util_load_texture_data_from_file(const char* file_path, int32_t* width, int32_t* height, uint32_t* num_comps, void** data, bool32_t flip_vertically_on_load);
GS_API_DECL bool32_t gs_util_load_texture_data_from_memory(const void* memory, size_t sz, int32_t* width, int32_t* height, uint32_t* num_comps, void** data, bool32_t flip_vertically_on_load);

/** @} */ // end of gs_util

/*========================
// GS_CONTAINERS
========================*/

/** @defgroup gs_containers Containers
 *  Gunslinger Containers
 *  @{
 */

/*========================
// Byte Buffer
========================*/

/** @defgroup gs_byte_buffer Byte Buffer
 *  @ingroup gs_containers
 *  Byte Buffer
 */

#define GS_BYTE_BUFFER_DEFAULT_CAPCITY  1024

/** @addtogroup gs_byte_buffer
 */
typedef struct gs_byte_buffer_t
{
    uint8_t* data;      // Buffer that actually holds all relevant byte data
    uint32_t size;      // Current size of the stored buffer data
    uint32_t position;  // Current read/write position in the buffer
    uint32_t capacity;  // Current max capacity for the buffer
} gs_byte_buffer_t;

// Generic "write" function for a byte buffer
#define gs_byte_buffer_write(__BB, __T, __VAL)\
do {\
    gs_byte_buffer_t* __BUFFER = __BB;\
    usize __SZ = sizeof(__T);\
    usize __TWS = __BUFFER->position + __SZ;\
    if (__TWS >= (usize)__BUFFER->capacity)\
    {\
        usize __CAP = __BUFFER->capacity * 2;\
        while(__CAP < __TWS)\
        {\
            __CAP *= 2;\
        }\
        gs_byte_buffer_resize(__BUFFER, __CAP);\
    }\
    *(__T*)(__BUFFER->data + __BUFFER->position) = __VAL;\
    __BUFFER->position += (uint32_t)__SZ;\
    __BUFFER->size += (uint32_t)__SZ;\
} while (0)

// Generic "read" function
#define gs_byte_buffer_read(__BUFFER, __T, __VAL_P)\
do {\
    __T* __V = (__T*)(__VAL_P);\
    gs_byte_buffer_t* __BB = (__BUFFER);\
    *(__V) = *(__T*)(__BB->data + __BB->position);\
    __BB->position += sizeof(__T);\
} while (0)

// Defines variable and sets value from buffer in place
// Use to construct a new variable
#define gs_byte_buffer_readc(__BUFFER, __T, __NAME)\
    __T __NAME = gs_default_val();\
    gs_byte_buffer_read((__BUFFER), __T, &__NAME);

#define gs_byte_buffer_read_bulkc(__BUFFER, __T, __NAME, __SZ)\
    __T __NAME = gs_default_val();\
    __T* gs_macro_cat(__NAME, __LINE__) = &(__NAME);\
    gs_byte_buffer_read_bulk(__BUFFER, (void**)&gs_macro_cat(__NAME, __LINE__), __SZ);

GS_API_DECL void gs_byte_buffer_init(gs_byte_buffer_t* buffer);
GS_API_DECL gs_byte_buffer_t gs_byte_buffer_new(); 
GS_API_DECL void gs_byte_buffer_free(gs_byte_buffer_t* buffer); 
GS_API_DECL void gs_byte_buffer_clear(gs_byte_buffer_t* buffer); 
GS_API_DECL bool gs_byte_buffer_empty(gs_byte_buffer_t* buffer);
GS_API_DECL size_t gs_byte_buffer_size(gs_byte_buffer_t* buffer);
GS_API_DECL void gs_byte_buffer_resize(gs_byte_buffer_t* buffer, size_t sz); 
GS_API_DECL void gs_byte_buffer_seek_to_beg(gs_byte_buffer_t* buffer); 
GS_API_DECL void gs_byte_buffer_seek_to_end(gs_byte_buffer_t* buffer); 
GS_API_DECL void gs_byte_buffer_advance_position(gs_byte_buffer_t* buffer, size_t sz); 
GS_API_DECL void gs_byte_buffer_write_str(gs_byte_buffer_t* buffer, const char* str);                   // Expects a null terminated string 
GS_API_DECL void gs_byte_buffer_read_str(gs_byte_buffer_t* buffer, char* str);                          // Expects an allocated string 
GS_API_DECL void gs_byte_buffer_write_bulk(gs_byte_buffer_t* buffer, void* src, size_t sz); 
GS_API_DECL void gs_byte_buffer_read_bulk(gs_byte_buffer_t* buffer, void** dst, size_t sz); 
GS_API_DECL void gs_byte_buffer_memset(gs_byte_buffer_t* buffer, uint8_t val);

/*===================================
// Dynamic Array
===================================*/

/** @defgroup gs_dyn_array Dynamic Array
 *  @ingroup gs_containers
 *  Dynamic Array
 */

/** @addtogroup gs_dyn_array
 */
typedef struct gs_dyn_array
{
    int32_t size;
    int32_t capacity;
} gs_dyn_array;

#define gs_dyn_array_head(__ARR)\
    ((gs_dyn_array*)((uint8_t*)(__ARR) - sizeof(gs_dyn_array)))

#define gs_dyn_array_size(__ARR)\
    (__ARR == NULL ? 0 : gs_dyn_array_head((__ARR))->size)

#define gs_dyn_array_capacity(__ARR)\
    (__ARR == NULL ? 0 : gs_dyn_array_head((__ARR))->capacity)

#define gs_dyn_array_full(__ARR)\
    ((gs_dyn_array_size((__ARR)) == gs_dyn_array_capacity((__ARR))))    

#define gs_dyn_array_byte_size(__ARR)\
    (gs_dyn_array_size((__ARR)) * sizeof(*__ARR))

GS_API_DECL void* 
gs_dyn_array_resize_impl(void* arr, size_t sz, size_t amount);

#define gs_dyn_array_need_grow(__ARR, __N)\
    ((__ARR) == 0 || gs_dyn_array_size(__ARR) + (__N) >= gs_dyn_array_capacity(__ARR))

#define gs_dyn_array_grow(__ARR)\
    gs_dyn_array_resize_impl((__ARR), sizeof(*(__ARR)), gs_dyn_array_capacity(__ARR) ? gs_dyn_array_capacity(__ARR) * 2 : 1)

#define gs_dyn_array_grow_size(__ARR, __SZ  )\
    gs_dyn_array_resize_impl((__ARR), (__SZ ), gs_dyn_array_capacity(__ARR) ? gs_dyn_array_capacity(__ARR) * 2 : 1)

GS_API_DECL void** 
gs_dyn_array_init(void** arr, size_t val_len);

GS_API_DECL void 
gs_dyn_array_push_data(void** arr, void* val, size_t val_len);

gs_force_inline
void gs_dyn_array_set_data_i(void** arr, void* val, size_t val_len, uint32_t offset)
{
    memcpy(((char*)(*arr)) + offset * val_len, val, val_len);
}

#define gs_dyn_array_push(__ARR, __ARRVAL)\
    do {\
        gs_dyn_array_init((void**)&(__ARR), sizeof(*(__ARR)));\
        if (!(__ARR) || ((__ARR) && gs_dyn_array_need_grow(__ARR, 1))) {\
            *((void **)&(__ARR)) = gs_dyn_array_grow(__ARR); \
        }\
        (__ARR)[gs_dyn_array_size(__ARR)] = (__ARRVAL);\
        gs_dyn_array_head(__ARR)->size++;\
    } while(0)

#define gs_dyn_array_reserve(__ARR, __AMOUNT)\
    do {\
        if ((!__ARR)) gs_dyn_array_init((void**)&(__ARR), sizeof(*(__ARR)));\
        if ((!__ARR) || (size_t)__AMOUNT > gs_dyn_array_capacity(__ARR)) {\
            *((void **)&(__ARR)) = gs_dyn_array_resize_impl(__ARR, sizeof(*__ARR), __AMOUNT);\
        }\
    } while(0)

#define gs_dyn_array_empty(__ARR)\
    (gs_dyn_array_init((void**)&(__ARR), sizeof(*(__ARR))), (gs_dyn_array_size(__ARR) == 0))

#define gs_dyn_array_pop(__ARR)\
    do {\
        if (__ARR && !gs_dyn_array_empty(__ARR)) {\
            gs_dyn_array_head(__ARR)->size -= 1;\
        }\
    } while (0)

#define gs_dyn_array_back(__ARR)\
    *(__ARR + (gs_dyn_array_size(__ARR) ? gs_dyn_array_size(__ARR) - 1 : 0))

#define gs_dyn_array_for(__ARR, __T, __IT_NAME)\
    for (__T* __IT_NAME = __ARR; __IT_NAME != gs_dyn_array_back(__ARR); ++__IT_NAME)

#define gs_dyn_array_new(__T)\
    ((__T*)gs_dyn_array_resize_impl(NULL, sizeof(__T), 0))

#define gs_dyn_array_clear(__ARR)\
    do {\
        if (__ARR) {\
            gs_dyn_array_head(__ARR)->size = 0;\
        }\
    } while (0)

#define gs_dyn_array(__T)   __T*

#define gs_dyn_array_free(__ARR)\
    do {\
        if (__ARR) {\
            gs_free(gs_dyn_array_head(__ARR));\
            (__ARR) = NULL;\
        }\
    } while (0)

/*===================================
// Static Array
===================================*/

/*===================================
// Hash Table
===================================*/

/* 
    If using struct for keys, requires struct to be word-aligned.
*/

#define GS_HASH_TABLE_HASH_SEED         0x31415296
#define GS_HASH_TABLE_INVALID_INDEX     UINT32_MAX

typedef enum gs_hash_table_entry_state
{
    GS_HASH_TABLE_ENTRY_INACTIVE = 0x00,
    GS_HASH_TABLE_ENTRY_ACTIVE = 0x01
} gs_hash_table_entry_state;

#define __gs_hash_table_entry(__HMK, __HMV)\
    struct\
    {\
        __HMK key;\
        __HMV val;\
        gs_hash_table_entry_state state;\
    }

#define gs_hash_table(__HMK, __HMV)\
    struct {\
        __gs_hash_table_entry(__HMK, __HMV)* data;\
        __HMK tmp_key;\
        __HMV tmp_val;\
        size_t stride;\
        size_t klpvl;\
        size_t tmp_idx;\
    }*

// Need a way to create a temporary key so I can take the address of it

#define gs_hash_table_new(__K, __V)\
    NULL

GS_API_DECL void 
__gs_hash_table_init_impl(void** ht, size_t sz);

#define gs_hash_table_init(__HT, __K, __V)\
    do {\
        size_t entry_sz = sizeof(*__HT->data);\
        size_t ht_sz = sizeof(*__HT);\
        __gs_hash_table_init_impl((void**)&(__HT), ht_sz);\
        memset((__HT), 0, ht_sz);\
        gs_dyn_array_reserve(__HT->data, 2);\
        __HT->data[0].state = GS_HASH_TABLE_ENTRY_INACTIVE;\
        __HT->data[1].state = GS_HASH_TABLE_ENTRY_INACTIVE;\
        uintptr_t d0 = (uintptr_t)&((__HT)->data[0]);\
        uintptr_t d1 = (uintptr_t)&((__HT)->data[1]);\
        ptrdiff_t diff = (d1 - d0);\
        ptrdiff_t klpvl = (uintptr_t)&(__HT->data[0].state) - (uintptr_t)(&__HT->data[0]);\
        (__HT)->stride = (size_t)(diff);\
        (__HT)->klpvl = (size_t)(klpvl);\
    } while (0)

#define gs_hash_table_reserve(_HT, _KT, _VT, _CT)\
    do {\
        if ((_HT) == NULL) {\
            gs_hash_table_init((_HT), _KT, _VT);\
        }\
        gs_dyn_array_reserve((_HT)->data, _CT);\
    } while (0)

    // ((__HT) != NULL ? (__HT)->size : 0) // gs_dyn_array_size((__HT)->data) : 0)
#define gs_hash_table_size(__HT)\
    ((__HT) != NULL ? gs_dyn_array_size((__HT)->data) : 0)

#define gs_hash_table_capacity(__HT)\
    ((__HT) != NULL ? gs_dyn_array_capacity((__HT)->data) : 0)

#define gs_hash_table_load_factor(__HT)\
    (gs_hash_table_capacity(__HT) ? (float)(gs_hash_table_size(__HT)) / (float)(gs_hash_table_capacity(__HT)) : 0.f)

#define gs_hash_table_grow(__HT, __C)\
    ((__HT)->data = gs_dyn_array_resize_impl((__HT)->data, sizeof(*((__HT)->data)), (__C)))

#define gs_hash_table_empty(__HT)\
    ((__HT) != NULL ? gs_dyn_array_size((__HT)->data) == 0 : true)

#define gs_hash_table_clear(__HT)\
    do {\
        if ((__HT) != NULL) {\
            uint32_t capacity = gs_dyn_array_capacity((__HT)->data);\
            for (uint32_t i = 0; i < capacity; ++i) {\
                (__HT)->data[i].state = GS_HASH_TABLE_ENTRY_INACTIVE;\
            }\
            /*memset((__HT)->data, 0, gs_dyn_array_capacity((__HT)->data) * );*/\
            gs_dyn_array_clear((__HT)->data);\
        }\
    } while (0)

#define gs_hash_table_free(__HT)\
    do {\
        if ((__HT) != NULL) {\
            gs_dyn_array_free((__HT)->data);\
            (__HT)->data = NULL;\
            gs_free(__HT);\
            (__HT) = NULL;\
        }\
    } while (0)

// Find available slot to insert k/v pair into
#define gs_hash_table_insert(__HT, __HMK, __HMV)\
    do {\
        /* Check for null hash table, init if necessary */\
        if ((__HT) == NULL) {\
            gs_hash_table_init((__HT), (__HMK), (__HMV));\
        }\
    \
        /* Grow table if necessary */\
        uint32_t __CAP = gs_hash_table_capacity(__HT);\
        float __LF = gs_hash_table_load_factor(__HT);\
        if (__LF >= 0.5f || !__CAP)\
        {\
            uint32_t NEW_CAP = __CAP ? __CAP * 2 : 2;\
            size_t ENTRY_SZ = sizeof((__HT)->tmp_key) + sizeof((__HT)->tmp_val) + sizeof(gs_hash_table_entry_state);\
            gs_dyn_array_reserve((__HT)->data, NEW_CAP);\
            /**((void **)&(__HT->data)) = gs_dyn_array_resize_impl(__HT->data, ENTRY_SZ, NEW_CAP);*/\
            /* Iterate through data and set state to null, from __CAP -> __CAP * 2 */\
            /* Memset here instead */\
            for (uint32_t __I = __CAP; __I < NEW_CAP; ++__I) {\
                (__HT)->data[__I].state = GS_HASH_TABLE_ENTRY_INACTIVE;\
            }\
            __CAP = gs_hash_table_capacity(__HT);\
        }\
    \
        /* Get hash of key */\
        (__HT)->tmp_key = (__HMK);\
        size_t __HSH = gs_hash_bytes((void*)&((__HT)->tmp_key), sizeof((__HT)->tmp_key), GS_HASH_TABLE_HASH_SEED);\
        size_t __HSH_IDX = __HSH % __CAP;\
        (__HT)->tmp_key = (__HT)->data[__HSH_IDX].key;\
        uint32_t c = 0;\
    \
        /* Find valid idx and place data */\
        while (\
            c < __CAP\
            && __HSH != gs_hash_bytes((void*)&(__HT)->tmp_key, sizeof((__HT)->tmp_key), GS_HASH_TABLE_HASH_SEED)\
            && (__HT)->data[__HSH_IDX].state == GS_HASH_TABLE_ENTRY_ACTIVE)\
        {\
            __HSH_IDX = ((__HSH_IDX + 1) % __CAP);\
            (__HT)->tmp_key = (__HT)->data[__HSH_IDX].key;\
            ++c;\
        }\
        (__HT)->data[__HSH_IDX].key = (__HMK);\
        (__HT)->data[__HSH_IDX].val = (__HMV);\
        (__HT)->data[__HSH_IDX].state = GS_HASH_TABLE_ENTRY_ACTIVE;\
        gs_dyn_array_head((__HT)->data)->size++;\
    } while (0)

// Need size difference between two entries
// Need size of key + val

gs_force_inline
uint32_t gs_hash_table_get_key_index_func(void** data, void* key, size_t key_len, size_t val_len, size_t stride, size_t klpvl)
{
    if (!data || !key) return GS_HASH_TABLE_INVALID_INDEX;

    // Need a better way to handle this. Can't do it like this anymore.
    // Need to fix this. Seriously messing me up.
    uint32_t capacity = gs_dyn_array_capacity(*data);
	uint32_t size = gs_dyn_array_size(*data);
	if (!capacity || !size) return (size_t)GS_HASH_TABLE_INVALID_INDEX;
    size_t idx = (size_t)GS_HASH_TABLE_INVALID_INDEX;
    size_t hash = (size_t)gs_hash_bytes(key, key_len, GS_HASH_TABLE_HASH_SEED);
    size_t hash_idx = (hash % capacity);

    // Iterate through data 
    for (size_t i = hash_idx, c = 0; c < capacity; ++c, i = ((i + 1) % capacity))
    {
        size_t offset = (i * stride);
        void* k = ((char*)(*data) + (offset));  
        size_t kh = gs_hash_bytes(k, key_len, GS_HASH_TABLE_HASH_SEED);
        bool comp = gs_compare_bytes(k, key, key_len);
        gs_hash_table_entry_state state = *(gs_hash_table_entry_state*)((char*)(*data) + offset + (klpvl)); 
        if (comp && hash == kh && state == GS_HASH_TABLE_ENTRY_ACTIVE) {
            idx = i;
            break;
        }
    }
    return (uint32_t)idx;
}

// Get key at index
#define gs_hash_table_getk(__HT, __I)\
    (((__HT))->data[(__I)].key)

// Get val at index
#define gs_hash_table_geti(__HT, __I)\
    ((__HT)->data[(__I)].val)

// Could search for the index in the macro instead now. Does this help me?
#define gs_hash_table_get(__HT, __HTK)\
    ((__HT)->tmp_key = (__HTK),\
        (gs_hash_table_geti((__HT),\
            gs_hash_table_get_key_index_func((void**)&(__HT)->data, (void*)&((__HT)->tmp_key),\
                sizeof((__HT)->tmp_key), sizeof((__HT)->tmp_val), (__HT)->stride, (__HT)->klpvl)))) 

#define gs_hash_table_getp(__HT, __HTK)\
    (\
        (__HT)->tmp_key = (__HTK),\
        ((__HT)->tmp_idx = (uint32_t)gs_hash_table_get_key_index_func((void**)&(__HT->data), (void*)&(__HT->tmp_key), sizeof(__HT->tmp_key),\
            sizeof(__HT->tmp_val), __HT->stride, __HT->klpvl)),\
        ((__HT)->tmp_idx != GS_HASH_TABLE_INVALID_INDEX ? &gs_hash_table_geti((__HT), (__HT)->tmp_idx) : NULL)\
    )

#define _gs_hash_table_key_exists_internal(__HT, __HTK)\
    ((__HT)->tmp_key = (__HTK),\
        (gs_hash_table_get_key_index_func((void**)&(__HT->data), (void*)&(__HT->tmp_key), sizeof(__HT->tmp_key),\
            sizeof(__HT->tmp_val), __HT->stride, __HT->klpvl) != GS_HASH_TABLE_INVALID_INDEX))

// uint32_t gs_hash_table_get_key_index_func(void** data, void* key, size_t key_len, size_t val_len, size_t stride, size_t klpvl)

#define gs_hash_table_exists(__HT, __HTK)\
        (__HT && _gs_hash_table_key_exists_internal((__HT), (__HTK)))

#define gs_hash_table_key_exists(__HT, __HTK)\
		(gs_hash_table_exists((__HT), (__HTK)))

#define gs_hash_table_erase(__HT, __HTK)\
    do {\
        if ((__HT))\
        {\
            /* Get idx for key */\
            (__HT)->tmp_key = (__HTK);\
            uint32_t __IDX = gs_hash_table_get_key_index_func((void**)&(__HT)->data, (void*)&((__HT)->tmp_key), sizeof((__HT)->tmp_key), sizeof((__HT)->tmp_val), (__HT)->stride, (__HT)->klpvl);\
            if (__IDX != GS_HASH_TABLE_INVALID_INDEX) {\
                (__HT)->data[__IDX].state = GS_HASH_TABLE_ENTRY_INACTIVE;\
                if (gs_dyn_array_head((__HT)->data)->size) gs_dyn_array_head((__HT)->data)->size--;\
            }\
        }\
    } while (0)

/*===== Hash Table Iterator ====*/

typedef uint32_t gs_hash_table_iter;
typedef gs_hash_table_iter gs_hash_table_iter_t;

gs_force_inline
uint32_t __gs_find_first_valid_iterator(void* data, size_t key_len, size_t val_len, uint32_t idx, size_t stride, size_t klpvl)
{
    uint32_t it = (uint32_t)idx;
    for (; it < (uint32_t)gs_dyn_array_capacity(data); ++it)
    {
        size_t offset = (it * stride);
        gs_hash_table_entry_state state = *(gs_hash_table_entry_state*)((uint8_t*)data + offset + (klpvl));
        if (state == GS_HASH_TABLE_ENTRY_ACTIVE)
        {
            break;
        }
    }
    return it;
}

/* Find first valid iterator idx */
#define gs_hash_table_iter_new(__HT)\
    (__HT ? __gs_find_first_valid_iterator((__HT)->data, sizeof((__HT)->tmp_key), sizeof((__HT)->tmp_val), 0, (__HT)->stride, (__HT)->klpvl) : 0)

#define gs_hash_table_iter_valid(__HT, __IT)\
    ((__IT) < gs_hash_table_capacity((__HT)))

// Have to be able to do this for hash table...
gs_force_inline
void __gs_hash_table_iter_advance_func(void** data, size_t key_len, size_t val_len, uint32_t* it, size_t stride, size_t klpvl)
{
    (*it)++;
    for (; *it < (uint32_t)gs_dyn_array_capacity(*data); ++*it)
    {
        size_t offset = (size_t)(*it * stride);
        gs_hash_table_entry_state state = *(gs_hash_table_entry_state*)((uint8_t*)*data + offset + (klpvl));
        if (state == GS_HASH_TABLE_ENTRY_ACTIVE)
        {
            break;
        }
    }
}

#define gs_hash_table_find_valid_iter(__HT, __IT)\
    ((__IT) = __gs_find_first_valid_iterator((void**)&(__HT)->data, sizeof((__HT)->tmp_key), sizeof((__HT)->tmp_val), (__IT), (__HT)->stride, (__HT)->klpvl))

#define gs_hash_table_iter_advance(__HT, __IT)\
    (__gs_hash_table_iter_advance_func((void**)&(__HT)->data, sizeof((__HT)->tmp_key), sizeof((__HT)->tmp_val), &(__IT), (__HT)->stride, (__HT)->klpvl))

#define gs_hash_table_iter_get(__HT, __IT)\
    gs_hash_table_geti(__HT, __IT)

#define gs_hash_table_iter_getp(__HT, __IT)\
    (&(gs_hash_table_geti(__HT, __IT)))

#define gs_hash_table_iter_getk(__HT, __IT)\
    (gs_hash_table_getk(__HT, __IT))

#define gs_hash_table_iter_getkp(__HT, __IT)\
    (&(gs_hash_table_getk(__HT, __IT)))

/*==================
//=== Hash Set ===//
==================*/

#define GS_HASH_SET_HASH_SEED         0x31415296
#define GS_HASH_SET_INVALID_INDEX     UINT32_MAX
#define GS_HASH_SET_MAX_PROBE         64
#define GS_HASH_SET_MAX_LOAD_FACTOR   0.75f

typedef enum gs_hash_set_entry_state
{
    GS_HASH_SET_ENTRY_INACTIVE = 0x00,
    GS_HASH_SET_ENTRY_ACTIVE = 0x01
} gs_hash_set_entry_state;

#define __gs_hash_set_entry(__T)\
    struct\
    {\
        __T val;\
        gs_hash_set_entry_state state;\
    }

#define gs_hash_set(__T)\
    struct {\
        __gs_hash_set_entry(__T)* data;\
        __T tmp_val;\
        size_t stride;\
        size_t klpvl;\
        size_t tmp_idx;\
    }*

#define gs_hash_set_new(T)\
    NULL

GS_API_DECL void
__gs_hash_set_init_impl( void** ht, size_t sz );

#define gs_hash_set_init(__S, __T)\
    do {\
        size_t entry_sz = sizeof(*__S->data);\
        size_t ht_sz = sizeof(*__S);\
        __gs_hash_set_init_impl((void**)&(__S), ht_sz);\
        memset((__S), 0, ht_sz);\
        gs_dyn_array_reserve(__S->data, 2);\
        __S->data[0].state = GS_HASH_SET_ENTRY_INACTIVE;\
        __S->data[1].state = GS_HASH_SET_ENTRY_INACTIVE;\
        uintptr_t d0 = (uintptr_t)&((__S)->data[0]);\
        uintptr_t d1 = (uintptr_t)&((__S)->data[1]);\
        ptrdiff_t diff = (d1 - d0);\
        ptrdiff_t klpvl = (uintptr_t)&(__S->data[0].state) - (uintptr_t)(&__S->data[0]);\
        (__S)->stride = (size_t)(diff);\
        (__S)->klpvl = (size_t)(klpvl);\
    } while (0)

#define gs_hash_set_reserve(__S, __T, __CT)\
    do {\
        if ((__S) == NULL) {\
            gs_hash_set_init((__S), __T);\
        }\
        gs_dyn_array_reserve((__S)->data, __CT);\
    } while (0)

#define gs_hash_set_size(__S)\
    ((__S) != NULL ? gs_dyn_array_size((__S)->data) : 0)

#define gs_hash_set_capacity(__S)\
    ((__S) != NULL ? gs_dyn_array_capacity((__S)->data) : 0)

#define gs_hash_set_load_factor(__S)\
    (gs_hash_set_capacity(__S) ? (float)(gs_hash_set_size(__S)) / (float)(gs_hash_set_capacity(__S)) : 0.f)

#define gs_hash_set_grow(__S, __C)\
    ((__S)->data = gs_dyn_array_resize_impl((__S)->data, sizeof(*((__S)->data)), (__C)))

#define gs_hash_set_empty(__S)\
    ((__S) != NULL ? gs_dyn_array_size((__S)->data) == 0 : true)

#define gs_hash_set_clear(__S)\
    do {\
        if ((__S) != NULL) {\
            uint32_t capacity = gs_dyn_array_capacity((__S)->data);\
            memset((__S)->data, 0, gs_dyn_array_capacity((__S)->data) * sizeof(*(__S)->data));\
            gs_dyn_array_clear((__S)->data);\
        }\
    } while (0)

#define gs_hash_set_free(__S)\
    do {\
        if ((__S) != NULL) {\
            gs_dyn_array_free((__S)->data);\
            (__S)->data = NULL;\
            gs_free(__S);\
            (__S) = NULL;\
        }\
    } while (0)

#define gs_hash_set_hash_idx(_I, _C, _CAP)\
    ((_I + _C) % _CAP)

gs_force_inline
uint32_t gs_hash_set_get_key_index_func(void** data, void* key, size_t key_len, size_t stride, size_t klpvl)
{
    if (!data || !key) return GS_HASH_SET_INVALID_INDEX;

    uint32_t capacity = gs_dyn_array_capacity(*data);
	uint32_t size = gs_dyn_array_size(*data);
	if (!capacity || !size) return (size_t)GS_HASH_SET_INVALID_INDEX;
    size_t idx = (size_t)GS_HASH_SET_INVALID_INDEX;
    size_t hash = (size_t)gs_hash_bytes(key, key_len, GS_HASH_SET_HASH_SEED); 
    size_t hash_idx = (hash % capacity);
    size_t c = 0;
    size_t max_probe = GS_HASH_SET_MAX_PROBE;
    // static size_t pc = 0;
    // static size_t pm = 0;

    // Iterate through data
    for (size_t i = hash_idx; c < max_probe; ++c, i = ((i + c) % capacity)) {
        size_t offset = (i * stride);
        void* k = ((char*)(*data) + (offset));  
        size_t kh = gs_hash_bytes(k, key_len, GS_HASH_SET_HASH_SEED);
        bool comp = gs_compare_bytes(k, key, key_len);
        gs_hash_set_entry_state state = *(gs_hash_set_entry_state*)((char*)(*data) + offset + (klpvl));
        if (comp && hash == kh && state == GS_HASH_SET_ENTRY_ACTIVE) {
            idx = i;
            break;
        }
    }
    // pc++;
    // pm += c;
    // float pf = (float)pm / (float)pc;
    // gs_println("hash: %zu, hash_idx: %zu, found: %zu, c: %d, pf: %.2f", hash, hash_idx, idx, (int32_t)c, pf);
    return (uint32_t)idx;
}

// TODO(john): Figure out why this is crashing.
gs_force_inline
void gs_hash_set_rehash(void** data, void** new_data, size_t new_cap, size_t key_len, size_t stride, size_t klpvl)
{
    if (!data | !new_data) return; 
    uint32_t capacity = gs_dyn_array_capacity(*data);
    if (new_cap <= capacity) return;

    for (uint32_t i = 0; i < capacity; ++i) {

        // Get original data
        size_t offset = (i * stride);

        // If this entry is inactive, then continue
        if (*((gs_hash_set_entry_state*)((char*)(*data) + offset + (klpvl))) == GS_HASH_SET_ENTRY_INACTIVE) {
            continue;
        }
        void* k = ((char*)(*data) + offset);  
        size_t kh = gs_hash_bytes(k, key_len, GS_HASH_SET_HASH_SEED);

        // Hash idx into new data with new capacity
        uint32_t c = 0;
        size_t hash_idx = (kh % new_cap);

        /* Find valid idx and place data */\
        while (
            c < new_cap
            && *((gs_hash_set_entry_state*)((char*)(*new_data) + (hash_idx * stride) + (klpvl))) == GS_HASH_SET_ENTRY_ACTIVE
        )
        {
            ++c;
            hash_idx = ((hash_idx + c)) % new_cap;
        }

        // Set new data in new array
        // This crashes
        // hash_idx = new_cap - 2;
        size_t noff = hash_idx * stride;
        uint32_t tmp = 20;
        memcpy((void*)(((uint8_t*)(*new_data)) + noff), k, key_len);
        // *(&(char*)(*new_data) + noff) = (char*)k;
        // memset((void*)((char*)(*new_data) + noff), 0x00, key_len);
        *((gs_hash_set_entry_state*)((char*)(*new_data) + (hash_idx * stride) + klpvl)) = GS_HASH_SET_ENTRY_ACTIVE;
    }
}

// Find available slot to insert k/v pair into
#define gs_hash_set_insert(__S, __T)\
    do {\
        /* Check for null hash table, init if necessary */\
        if ((__S) == NULL) {\
            gs_hash_set_init((__S), (__T));\
        }\
    \
        /* Grow table if necessary */\
        uint32_t __CAP = gs_hash_set_capacity(__S);\
        float __LF = gs_hash_set_load_factor(__S);\
        if (__LF >= GS_HASH_SET_MAX_LOAD_FACTOR || !__CAP)\
        {\
            uint32_t NEW_CAP = __CAP ? __CAP * 2 : 2;\
            size_t ENTRY_SZ = sizeof((__S)->tmp_val) + sizeof(gs_hash_set_entry_state);\
            void* new_data = gs_calloc(NEW_CAP * 2, ENTRY_SZ);\
            /*Rehash data, reserve, copy, free*/\
            gs_hash_set_rehash((void**)&(__S)->data, (void**)&new_data, NEW_CAP, sizeof((__S)->tmp_val), (__S)->stride, (__S)->klpvl);\
            gs_dyn_array_reserve((__S)->data, NEW_CAP);\
            memcpy((__S)->data, new_data, NEW_CAP * ENTRY_SZ);\
            /* Memset here instead */\
            /*for (uint32_t __I = __CAP; __I < NEW_CAP; ++__I) {*/\
                /*(__S)->data[__I].state = GS_HASH_SET_ENTRY_INACTIVE;*/\
            /*}*/\
            __CAP = gs_hash_set_capacity(__S);\
            gs_free(new_data);\
        }\
    \
        /* Get hash of key */\
        (__S)->tmp_val = (__T);\
        size_t __HSH = gs_hash_bytes((void*)&((__S)->tmp_val), sizeof((__S)->tmp_val), GS_HASH_SET_HASH_SEED);\
        size_t __HSH_IDX = __HSH % __CAP;\
        uint32_t c = 0;\
        bool exists = false;\
    \
        /* Find valid idx and place data */\
        while (c < __CAP) {\
            /*If active entry*/\
            if (__S->data[__HSH_IDX].state == GS_HASH_SET_ENTRY_ACTIVE) {\
                if (gs_compare_bytes((void*)&(__S->tmp_val), (void*)&((__S)->data[__HSH_IDX].val), sizeof((__S)->tmp_val))) {\
                    exists = true;\
                    break;\
                }\
                ++c;\
                __HSH_IDX = ((__HSH_IDX + c) % __CAP);\
            }\
            /*Inactive entry, so break*/\
            else {\
                break;\
            }\
        }\
        if (!exists) {\
            (__S)->data[__HSH_IDX].val = (__T);\
            (__S)->data[__HSH_IDX].state = GS_HASH_SET_ENTRY_ACTIVE;\
            gs_dyn_array_head((__S)->data)->size++;\
        }\
    } while (0)

// Get val at index
#define gs_hash_set_geti(__S, __I)\
    ((__S)->data[(__I)].val)

// Could search for the index in the macro instead now. Does this help me?
#define gs_hash_set_get(__S, __SK)\
    ((__S)->tmp_val = (__SK),\
        (gs_hash_set_geti((__S),\
            gs_hash_set_get_key_index_func((void**)&(__S)->data, (void*)&((__S)->tmp_val),\
                sizeof((__S)->tmp_val), (__S)->stride, (__S)->klpvl)))) 

#define gs_hash_set_getp(__S, __SK)\
    (\
        (__S)->tmp_val = (__SK),\
        ((__S)->tmp_idx = (uint32_t)gs_hash_set_get_key_index_func((void**)&(__S->data), (void*)&(__S->tmp_val), sizeof(__S->tmp_val),\
            __S->stride, __S->klpvl)),\
        ((__S)->tmp_idx != GS_HASH_SET_INVALID_INDEX ? &gs_hash_set_geti((__S), (__S)->tmp_idx) : NULL)\
    )

#define _gs_hash_set_key_exists_internal(__S, __SK)\
    ((__S)->tmp_val = (__SK),\
        (gs_hash_set_get_key_index_func((void**)&(__S->data), (void*)&(__S->tmp_val), sizeof(__S->tmp_val),\
            __S->stride, __S->klpvl) != GS_HASH_SET_INVALID_INDEX))

#define gs_hash_set_exists(__S, __SK)\
        (__S && _gs_hash_set_key_exists_internal((__S), (__SK)))

#define gs_hash_set_key_exists(__S, __SK)\
		(gs_hash_set_exists((__S), (__SK)))

#define gs_hash_set_erase(__S, __SK)\
    do {\
        if ((__S))\
        {\
            /* Get idx for key */\
            (__S)->tmp_val = (__SK);\
            uint32_t __IDX = gs_hash_set_get_key_index_func((void**)&(__S)->data, (void*)&((__S)->tmp_val), sizeof((__S)->tmp_val), (__S)->stride, (__S)->klpvl);\
            if (__IDX != GS_HASH_SET_INVALID_INDEX) {\
                (__S)->data[__IDX].state = GS_HASH_SET_ENTRY_INACTIVE;\
                if (gs_dyn_array_head((__S)->data)->size) gs_dyn_array_head((__S)->data)->size--;\
            }\
        }\
    } while (0)

gs_force_inline
bool _gs_hash_set_is_subset_of_internal(void** s0, void** s1, size_t key_len, size_t stride, size_t klpvl)
{
    // If sz0 > sz1, then cannot be subset
    uint32_t sz0 = gs_dyn_array_size(*s0);
    uint32_t sz1 = gs_dyn_array_size(*s1);
    if (sz0 > sz1) return false;

    // If there exists an element in s0 not in s1, then not a subset
    uint32_t c0 = gs_dyn_array_capacity(*s0);
    uint32_t c1 = gs_dyn_array_capacity(*s1);

    for (uint32_t i = 0; i < c0; ++i) {
        // Continue past invalid entry
        size_t offset = (i * stride);
        gs_hash_set_entry_state state = *(gs_hash_set_entry_state*)((char*)(*s0) + offset + (klpvl));
        if (state == GS_HASH_SET_ENTRY_INACTIVE) {
            continue;
        } 
        // Valid entry, check against s1
        void* k0 = ((char*)(*s0) + (offset));
        size_t kh = gs_hash_bytes(k0, key_len, GS_HASH_SET_HASH_SEED);
        uint32_t hidx = (kh % c1); // Hash idx into super set
        uint32_t cc = 0;
        bool found = false;
        while (
            !found &&
            cc < GS_HASH_SET_MAX_PROBE
        ) { 
            // Not active entry
            size_t o1 = hidx * stride;  // New offset
            gs_hash_set_entry_state es = *((gs_hash_set_entry_state*)((char*)(*s1) + (hidx * stride) + (klpvl)));
            void* k1 = ((char*)(*s1) + (o1));
            // Found
            if (es == GS_HASH_SET_ENTRY_ACTIVE && gs_compare_bytes(k0, k1, key_len)) { 
                found = true;
                break;
            }
            // Continue
            ++cc;
            hidx = ((hidx + cc)) % c1;
        }
        if (!found) return false;
    }

    return true;
} 

#define gs_hash_set_is_subset_of(__S0, __S1)\
    (__S0 && __S1 &&\
        sizeof((__S0)->tmp_val) == sizeof((__S1)->tmp_val) &&\
        (__S0)->stride == (__S1)->stride &&\
        (__S0)->klpvl == (__S1)->klpvl &&\
        _gs_hash_set_is_subset_of_internal((void**)&(__S0)->data, (void**)&(__S1)->data,\
            sizeof((__S0)->tmp_val), (__S0)->stride, (__S0)->klpvl))

/*===== Set Iterator ====*/

typedef uint32_t gs_hash_set_iter;
typedef gs_hash_set_iter gs_hash_set_iter_t;

gs_force_inline
uint32_t __gs_hash_set_find_first_valid_iterator(void* data, uint32_t idx, size_t stride, size_t klpvl)
{
    uint32_t it = (uint32_t)idx;
    for (; it < (uint32_t)gs_dyn_array_capacity(data); ++it) {
        size_t offset = (it * stride);
        gs_hash_set_entry_state state = *(gs_hash_set_entry_state*)((uint8_t*)data + offset + (klpvl));
        if (state == GS_HASH_SET_ENTRY_ACTIVE) {
            break;
        }
    }
    return it;
}

/* Find first valid iterator idx */
#define gs_hash_set_iter_new(__S)\
    (__S ? __gs_hash_set_find_first_valid_iterator((__S)->data, 0, (__S)->stride, (__S)->klpvl) : 0)

#define gs_hash_set_iter_valid(__S, __IT)\
    ((__IT) < gs_hash_set_capacity((__S)))

gs_force_inline
void __gs_hash_set_iter_advance_func(void** data, uint32_t* it, size_t stride, size_t klpvl)
{
    (*it)++;
    for (; *it < (uint32_t)gs_dyn_array_capacity(*data); ++*it) {
        size_t offset = (size_t)(*it * stride);
        gs_hash_set_entry_state state = *(gs_hash_set_entry_state*)((uint8_t*)*data + offset + (klpvl));
        if (state == GS_HASH_SET_ENTRY_ACTIVE) {
            break;
        }
    }
}

#define gs_hash_set_find_valid_iter(__S, __IT)\
    ((__IT) = __gs_hash_set_find_first_valid_iterator((void**)&(__S)->data, (__IT), (__S)->stride, (__S)->klpvl))

#define gs_hash_set_iter_advance(__S, __IT)\
    (__gs_hash_set_iter_advance_func((void**)&(__S)->data, &(__IT), (__S)->stride, (__S)->klpvl))

#define gs_hash_set_iter_get(__S, __IT)\
    gs_hash_set_geti(__S, __IT)

#define gs_hash_set_iter_getp(__S, __IT)\
    (&(gs_hash_set_geti(__S, __IT)))

#define gs_hash_set_iter_getk(__S, __IT)\
    (gs_hash_set_getk(__S, __IT))

#define gs_hash_set_iter_getkp(__S, __IT)\
    (&(gs_hash_set_getk(__S, __IT)))

/*===================================
// Slot Array
===================================*/

#define GS_SLOT_ARRAY_INVALID_HANDLE    UINT32_MAX

#define gs_slot_array_handle_valid(__SA, __ID)\
    ((__SA) && __ID < gs_dyn_array_size((__SA)->indices) && (__SA)->indices[__ID] != GS_SLOT_ARRAY_INVALID_HANDLE)

typedef struct __gs_slot_array_dummy_header {
    gs_dyn_array(uint32_t) indices;
    gs_dyn_array(uint32_t) data;
} __gs_slot_array_dummy_header;

#define gs_slot_array(__T)\
    struct\
    {\
        gs_dyn_array(uint32_t) indices;\
        gs_dyn_array(__T) data;\
        __T tmp;\
    }*

#define gs_slot_array_new(__T)\
    NULL

gs_force_inline
uint32_t __gs_slot_array_find_next_available_index(gs_dyn_array(uint32_t) indices)
{
    uint32_t idx = GS_SLOT_ARRAY_INVALID_HANDLE;
    for (uint32_t i = 0; i < (uint32_t)gs_dyn_array_size(indices); ++i)
    {
        uint32_t handle = indices[i];
        if (handle == GS_SLOT_ARRAY_INVALID_HANDLE)
        {
            idx = i;
            break;
        }
    }
    if (idx == GS_SLOT_ARRAY_INVALID_HANDLE)
    {
        idx = gs_dyn_array_size(indices);
    }

    return idx;
}

GS_API_DECL void** 
gs_slot_array_init(void** sa, size_t sz);

#define gs_slot_array_init_all(__SA)\
    (gs_slot_array_init((void**)&(__SA), sizeof(*(__SA))), gs_dyn_array_init((void**)&((__SA)->indices), sizeof(uint32_t)),\
        gs_dyn_array_init((void**)&((__SA)->data), sizeof((__SA)->tmp)))

gs_force_inline
uint32_t gs_slot_array_insert_func(void** indices, void** data, void* val, size_t val_len, uint32_t* ip)
{
    // Find next available index
    u32 idx = __gs_slot_array_find_next_available_index((uint32_t*)*indices);

    if (idx == gs_dyn_array_size(*indices)) {
        uint32_t v = 0;
        gs_dyn_array_push_data(indices, &v, sizeof(uint32_t));  
        idx = gs_dyn_array_size(*indices) - 1;
    }
    // Push data to array
    gs_dyn_array_push_data(data, val, val_len);

    // Set data in indices
    uint32_t bi = gs_dyn_array_size(*data) - 1;
    gs_dyn_array_set_data_i(indices, &bi, sizeof(uint32_t), idx);

    if (ip){
        *ip = idx;
    }

    return idx;
}

#define gs_slot_array_reserve(__SA, __NUM)\
    do {\
        gs_slot_array_init_all(__SA);\
        gs_dyn_array_reserve((__SA)->data, __NUM);\
        gs_dyn_array_reserve((__SA)->indices, __NUM);\
    } while (0)

#define gs_slot_array_insert(__SA, __VAL)\
    (gs_slot_array_init_all(__SA), (__SA)->tmp = (__VAL),\
        gs_slot_array_insert_func((void**)&((__SA)->indices), (void**)&((__SA)->data), (void*)&((__SA)->tmp), sizeof(((__SA)->tmp)), NULL))

#define gs_slot_array_insert_hp(__SA, __VAL, __hp)\
    (gs_slot_array_init_all(__SA), (__SA)->tmp = (__VAL),\
        gs_slot_array_insert_func((void**)&((__SA)->indices), (void**)&((__SA)->data), &((__SA)->tmp), sizeof(((__SA)->tmp)), (__hp)))

#define gs_slot_array_insert_no_init(__SA, __VAL)\
    ((__SA)->tmp = (__VAL), gs_slot_array_insert_func((void**)&((__SA)->indices), (void**)&((__SA)->data), &((__SA)->tmp), sizeof(((__SA)->tmp)), NULL))

#define gs_slot_array_size(__SA)\
    ((__SA) == NULL ? 0 : gs_dyn_array_size((__SA)->data))

 #define gs_slot_array_empty(__SA)\
    (gs_slot_array_size(__SA) == 0)

#define gs_slot_array_clear(__SA)\
    do {\
        if ((__SA) != NULL) {\
            gs_dyn_array_clear((__SA)->data);\
            gs_dyn_array_clear((__SA)->indices);\
        }\
    } while (0)

#define gs_slot_array_exists(__SA, __SID)\
    ((__SA) && (__SID) < (uint32_t)gs_dyn_array_size((__SA)->indices) && (__SA)->indices[__SID] != GS_SLOT_ARRAY_INVALID_HANDLE)

 #define gs_slot_array_get(__SA, __SID)\
    ((__SA)->data[(__SA)->indices[(__SID) % gs_dyn_array_size(((__SA)->indices))]])

 #define gs_slot_array_getp(__SA, __SID)\
    (&(gs_slot_array_get(__SA, (__SID))))

 #define gs_slot_array_free(__SA)\
    do {\
        if ((__SA) != NULL) {\
            gs_dyn_array_free((__SA)->data);\
            gs_dyn_array_free((__SA)->indices);\
            (__SA)->indices = NULL;\
            (__SA)->data = NULL;\
            gs_free((__SA));\
            (__SA) = NULL;\
        }\
    } while (0)

 #define gs_slot_array_erase(__SA, __id)\
    do {\
        uint32_t __H0 = (__id) /*% gs_dyn_array_size((__SA)->indices)*/;\
        if (gs_slot_array_size(__SA) == 1) {\
            gs_slot_array_clear(__SA);\
        }\
        else if (!gs_slot_array_handle_valid(__SA, __H0)) {\
            gs_println("Warning: Attempting to erase invalid slot array handle (%zu)", __H0);\
        }\
        else {\
            uint32_t __OG_DATA_IDX = (__SA)->indices[__H0];\
            /* Iterate through handles until last index of data found */\
            uint32_t __H = 0;\
            for (uint32_t __I = 0; __I < gs_dyn_array_size((__SA)->indices); ++__I)\
            {\
                if ((__SA)->indices[__I] == gs_dyn_array_size((__SA)->data) - 1)\
                {\
                    __H = __I;\
                    break;\
                }\
            }\
        \
            /* Swap and pop data */\
            (__SA)->data[__OG_DATA_IDX] = gs_dyn_array_back((__SA)->data);\
            gs_dyn_array_pop((__SA)->data);\
        \
            /* Point new handle, Set og handle to invalid */\
            (__SA)->indices[__H] = __OG_DATA_IDX;\
            (__SA)->indices[__H0] = GS_SLOT_ARRAY_INVALID_HANDLE;\
        }\
    } while (0)

/*=== Slot Array Iterator ===*/

// Slot array iterator new
typedef uint32_t gs_slot_array_iter;

#define gs_slot_array_iter_valid(__SA, __IT)\
    (__SA && gs_slot_array_exists(__SA, __IT))

gs_force_inline
void _gs_slot_array_iter_advance_func(gs_dyn_array(uint32_t) indices, uint32_t* it)
{
    if (!indices) {
       *it = GS_SLOT_ARRAY_INVALID_HANDLE; 
        return;
    }

    (*it)++;
    for (; *it < (uint32_t)gs_dyn_array_size(indices); ++*it)
    {\
        if (indices[*it] != GS_SLOT_ARRAY_INVALID_HANDLE)\
        {\
            break;\
        }\
    }\
}

gs_force_inline
uint32_t _gs_slot_array_iter_find_first_valid_index(gs_dyn_array(uint32_t) indices)
{
    if (!indices) return GS_SLOT_ARRAY_INVALID_HANDLE;

    for (uint32_t i = 0; i < (uint32_t)gs_dyn_array_size(indices); ++i)
    {
        if (indices[i] != GS_SLOT_ARRAY_INVALID_HANDLE)
        {
            return i;
        }
    }
    return GS_SLOT_ARRAY_INVALID_HANDLE;
}

#define gs_slot_array_iter_new(__SA) (_gs_slot_array_iter_find_first_valid_index((__SA) ? (__SA)->indices : 0))

#define gs_slot_array_iter_advance(__SA, __IT)\
    _gs_slot_array_iter_advance_func((__SA) ? (__SA)->indices : NULL, &(__IT))

#define gs_slot_array_iter_get(__SA, __IT)\
    gs_slot_array_get(__SA, __IT)

#define gs_slot_array_iter_getp(__SA, __IT)\
    gs_slot_array_getp(__SA, __IT)

/*===================================
// Slot Map
===================================*/

#define gs_slot_map(__SMK, __SMV)\
    struct {\
        gs_hash_table(__SMK, uint32_t) ht;\
        gs_slot_array(__SMV) sa;\
    }*

#define gs_slot_map_new(__SMK, __SMV)\
    NULL

GS_API_DECL void** 
gs_slot_map_init(void** sm);

// Could return something, I believe?
#define gs_slot_map_insert(__SM, __SMK, __SMV)\
    do {\
        gs_slot_map_init((void**)&(__SM));\
        uint32_t __H = gs_slot_array_insert((__SM)->sa, ((__SMV)));\
        gs_hash_table_insert((__SM)->ht, (__SMK), __H);\
    } while (0)

#define gs_slot_map_get(__SM, __SMK)\
    (gs_slot_array_get((__SM)->sa, gs_hash_table_get((__SM)->ht, (__SMK))))

#define gs_slot_map_getp(__SM, __SMK)\
    (gs_slot_array_getp((__SM)->sa, gs_hash_table_get((__SM)->ht, (__SMK))))

#define gs_slot_map_size(__SM)\
    (gs_slot_array_size((__SM)->sa))

#define gs_slot_map_clear(__SM)\
    do {\
        if ((__SM) != NULL) {\
            gs_hash_table_clear((__SM)->ht);\
            gs_slot_array_clear((__SM)->sa);\
        }\
    } while (0)

#define gs_slot_map_erase(__SM, __SMK)\
    do {\
        uint32_t __K = gs_hash_table_get((__SM)->ht, (__SMK));\
        gs_hash_table_erase((__SM)->ht, (__SMK));\
        gs_slot_array_erase((__SM)->sa, __K);\
    } while (0)

#define gs_slot_map_free(__SM)\
    do {\
        if (__SM != NULL) {\
            gs_hash_table_free((__SM)->ht);\
            gs_slot_array_free((__SM)->sa);\
            gs_free((__SM));\
            (__SM) = NULL;\
        }\
    } while (0)

#define gs_slot_map_capacity(__SM)\
    (gs_hash_table_capacity((__SM)->ht))

/*=== Slot Map Iterator ===*/

typedef uint32_t gs_slot_map_iter;

/* Find first valid iterator idx */
#define gs_slot_map_iter_new(__SM)\
    gs_hash_table_iter_new((__SM)->ht)

#define gs_slot_map_iter_valid(__SM, __IT)\
    ((__IT) < gs_hash_table_capacity((__SM)->ht))

#define gs_slot_map_iter_advance(__SM, __IT)\
    __gs_hash_table_iter_advance_func((void**)&((__SM)->ht->data), sizeof((__SM)->ht->tmp_key), sizeof((__SM)->ht->tmp_val), &(__IT), (__SM)->ht->stride, (__SM)->ht->klpvl)

#define gs_slot_map_iter_getk(__SM, __IT)\
    gs_hash_table_iter_getk((__SM)->ht, (__IT))
    //(gs_hash_table_find_valid_iter(__SM->ht, __IT), gs_hash_table_geti((__SM)->ht, (__IT)))

#define gs_slot_map_iter_getkp(__SM, __IT)\
    (gs_hash_table_find_valid_iter(__SM->ht, __IT), &(gs_hash_table_geti((__SM)->ht, (__IT))))

#define gs_slot_map_iter_get(__SM, __IT)\
    ((__SM)->sa->data[gs_hash_table_iter_get((__SM)->ht, (__IT))])

    // ((__SM)->sa->data[gs_hash_table_geti((__SM)->ht, (__IT))])
    // (gs_hash_table_find_valid_iter(__SM->ht, __IT), (__SM)->sa->data[gs_hash_table_geti((__SM)->ht, (__IT))])

#define gs_slot_map_iter_getp(__SM, __IT)\
    (&((__SM)->sa->data[gs_hash_table_geti((__SM)->ht, (__IT))]))

    // (gs_hash_table_find_valid_iter(__SM->ht, __IT), &((__SM)->sa->data[gs_hash_table_geti((__SM)->ht, (__IT))]))

/*===================================
// Priority Queue
===================================*/

// Min heap
#define gs_pqueue(__T)\
    struct\
    {\
        gs_dyn_array(__T) data;\
        gs_dyn_array(int32_t) priority;\
        __T tmp;\
    }*

#define gs_pqueue_parent_idx(I)      gs_max((uint32_t)(ceil(((float)I / 2.f) - 1)), 0)
#define gs_pqueue_child_left_idx(I)  ((I * 2) + 1)
#define gs_pqueue_child_right_idx(I) ((I * 2) + 2)

GS_API_DECL void** 
gs_pqueue_init(void** pq, size_t sz);

#define gs_pqueue_init_all(__PQ, __V)\
    (gs_pqueue_init((void**)&(__PQ), sizeof(*(__PQ))), gs_dyn_array_init((void**)&((__PQ)->priority), sizeof(int32_t)),\
        gs_dyn_array_init((void**)&((__PQ)->data), sizeof(__V)))

#define gs_pqueue_size(__PQ)\
    gs_dyn_array_size((__PQ)->data)

#define gs_pqueue_capacity(__PQ)\
    gs_dyn_array_capacity((__PQ)->data)

#define gs_pqueue_clear(__PQ)\
    do {\
        gs_dyn_array_clear((__PQ)->data);\
        gs_dyn_array_clear((__PQ)->priority);\
    } while (0) 

#define gs_pqueue_empty(__PQ)\
    (!(__PQ) || !gs_pqueue_size(__PQ))

#define __gs_pqueue_swp(__PQ, __I0, __I1, __SZ)\
    do {\
        /* Move data */\
        {\
            const size_t sz = (__SZ);\
            memmove(&((__PQ)->tmp), &((__PQ)->data[__I0]), sz);\
            memmove(&((__PQ)->data[__I0]), &((__PQ)->data[__I1]), sz);\
            memmove(&((__PQ)->data[__I1]), &((__PQ)->tmp), sz);\
        }\
        /* Move priority */\
        {\
            int32_t tmp = 0;\
            const size_t sz = sizeof(int32_t);\
            memmove(&tmp, &((__PQ)->priority[__I0]), sz);\
            memmove(&((__PQ)->priority[__I0]), &((__PQ)->priority[__I1]), sz);\
            memmove(&((__PQ)->priority[__I1]), &tmp, sz);\
        }\
    } while (0)

#define gs_pqueue_push(__PQ, __V, __PRI)\
    do {\
        /*Init*/\
        gs_pqueue_init_all((__PQ), (__V));\
        /*Push to end of array*/\
        gs_dyn_array_push((__PQ)->data, (__V));\
        gs_dyn_array_push((__PQ)->priority, (__PRI));\
        /*Compare and sort up*/\
        const size_t dsize = sizeof(__V);\
        int32_t i = gs_max(gs_pqueue_size((__PQ)) - 1, 0);\
        while (i)\
        {\
            /* Look at parent, compare, then swap indices with parent */\
            int32_t pidx = gs_pqueue_parent_idx(i);\
            if ((__PQ)->priority[pidx] > __PRI) {\
                __gs_pqueue_swp(__PQ, i, pidx, dsize);\
            }\
            else {\
                break;\
            }\
            i = pidx;\
        }\
    } while (0)

#if 0
/*
    Need to call into another function to return what I need... Not sure how to do this, since I need to know what TYPE to return...
*/
#define gs_pqueue_pop(__PQ)\
    (\
        __gs_pqueue_pop_internal(\
            (void**)&(__PQ),\
            &(__PQ)->tmp,\
            (void**)(&(__PQ)->data),\
            (__PQ)->priority,\
            gs_pqueue_size((__PQ)),\
            sizeof((__PQ)->tmp)\
        ),\
        (__PQ)->tmp = (__PQ)->data[gs_pqueue_size((__PQ)) - 1],\
        (gs_dyn_array_head((__PQ)->data))->size--,\
        (gs_dyn_array_head((__PQ)->priority))->size--,\
        (__PQ)->tmp\
    )
#endif

#if 1
/*
    No return.
*/
#define gs_pqueue_pop(__PQ)\
    do {\
        /* Swap elements */\
        if (gs_pqueue_empty((__PQ))) break;\
        __gs_pqueue_swp(__PQ, 0, gs_pqueue_size((__PQ)) - 1, sizeof((__PQ)->tmp));\
\
        int32_t i = 0;\
        int32_t c = 0;\
        int32_t psz = gs_dyn_array_size((__PQ)->priority) - 1;\
        for (int32_t i = 0; gs_pqueue_child_left_idx(i) < psz; i = c)\
        {\
            /* Set child to smaller of two */\
            c = gs_pqueue_child_left_idx(i);\
\
            /* Set to right child if valid and less priority */\
            if ((c + 1) < psz && (__PQ)->priority[c + 1] < (__PQ)->priority[c]) {\
                c++;\
            }\
\
            /* Check to swp, if necessary */\
            if ((__PQ)->priority[i] > (__PQ)->priority[c]) {\
                __gs_pqueue_swp((__PQ), i, c, sizeof((__PQ)->tmp));\
            }\
            /* Otherwise, we're done */\
            else\
            {\
                break;\
            }\
        }\
        (gs_dyn_array_head((__PQ)->data))->size--;\
        (gs_dyn_array_head((__PQ)->priority))->size--;\
    } while (0)
#endif

void __gs_pqueue_pop_internal(void** pqueue, void* tmp, void** data, int32_t* priority, int32_t pq_sz, size_t d_sz);

#define gs_pqueue_peek(__PQ)\
    (__PQ)->data[0]

#define gs_pqueue_peekp(__PQ)\
    &((__PQ)->data[0])

#define gs_pqueue_peek_pri(__PQ)\
    (__PQ)->priority[0]

#define gs_pqueue_free(__PQ)\
    do {\
        if ((__PQ) && (__PQ)->data) gs_dyn_array_free((__PQ)->data);\
        if ((__PQ) && (__PQ)->priority) gs_dyn_array_free((__PQ)->priority);\
        if ((__PQ)) gs_free((__PQ));\
    } while (0)

/*=== Priority Queue Iterator ===*/

typedef uint32_t gs_pqueue_iter;
typedef gs_pqueue_iter gs_pqueue_iter_t;

#define gs_pqueue_iter_new(__PQ)    0

#define gs_pqueue_iter_valid(__PQ, __IT)\
    ((__IT) < gs_pqueue_size((__PQ)))

#define gs_pqueue_iter_advance(__PQ, __IT) ++(__IT)

#define gs_pqueue_iter_get(__PQ, __IT)\
    (__PQ)->data[(__IT)]

#define gs_pqueue_iter_getp(__PQ, __IT)\
    &(__PQ)->data[(__IT)]

#define gs_pqueue_iter_get_pri(__PQ, __IT)\
    (__PQ)->priority[(__IT)]

#define gs_pqueue_iter_get_prip(__PQ, __IT)\
    &(__PQ)->priority[(__IT)]



/*==========================
// GS_OS
==========================*/

GS_API_DECL 
void* _gs_malloc_init_impl(size_t sz)
{
    void* data = gs_malloc(sz);
    memset(data, 0, sz);
    return data;
}

GS_API_DECL gs_os_api_t
gs_os_api_new_default()
{ 
    gs_os_api_t os = gs_default_val();
    os.malloc = malloc;
    os.malloc_init = _gs_malloc_init_impl;
    os.free = free;
    os.realloc = realloc;
    os.calloc = calloc;
#ifdef GS_PLATFORM_WIN
    os.alloca = malloc;
#else
    os.alloca = malloc;
#endif
    return os;
}

/*========================
// gs_byte_buffer
========================*/

void gs_byte_buffer_init(gs_byte_buffer_t* buffer)
{
    buffer->data     = (uint8_t*)gs_malloc(GS_BYTE_BUFFER_DEFAULT_CAPCITY);
    buffer->capacity = GS_BYTE_BUFFER_DEFAULT_CAPCITY;
    buffer->size     = 0;
    buffer->position = 0;
}

gs_byte_buffer_t gs_byte_buffer_new()
{
    gs_byte_buffer_t buffer;
    gs_byte_buffer_init(&buffer);
    return buffer;
}

void gs_byte_buffer_free(gs_byte_buffer_t* buffer)
{
    if (buffer && buffer->data) {
        gs_free(buffer->data);
    }
}

void gs_byte_buffer_clear(gs_byte_buffer_t* buffer)
{
    buffer->size = 0;
    buffer->position = 0;   
}

bool gs_byte_buffer_empty(gs_byte_buffer_t* buffer)
{
    return (buffer->size == 0);
}

size_t gs_byte_buffer_size(gs_byte_buffer_t* buffer)
{
    return buffer->size;
}

void gs_byte_buffer_resize(gs_byte_buffer_t* buffer, size_t sz)
{
    uint8_t* data = (uint8_t*)gs_realloc(buffer->data, sz);

    if (data == NULL) {
        return;
    }

    buffer->data = data;    
    buffer->capacity = (uint32_t)sz;
}

void gs_byte_buffer_copy_contents(gs_byte_buffer_t* dst, gs_byte_buffer_t* src)
{
    gs_byte_buffer_seek_to_beg(dst);
    gs_byte_buffer_seek_to_beg(src);
    gs_byte_buffer_write_bulk(dst, src->data, src->size);
}

void gs_byte_buffer_seek_to_beg(gs_byte_buffer_t* buffer)
{
    buffer->position = 0;
}

void gs_byte_buffer_seek_to_end(gs_byte_buffer_t* buffer)
{
    buffer->position = buffer->size;
}

void gs_byte_buffer_advance_position(gs_byte_buffer_t* buffer, size_t sz)
{
    buffer->position += (uint32_t)sz; 
}

void gs_byte_buffer_write_bulk(gs_byte_buffer_t* buffer, void* src, size_t size)
{
    // Check for necessary resize
    size_t total_write_size = buffer->position + size;
    if (total_write_size >= (size_t)buffer->capacity)
    {
        size_t capacity = buffer->capacity * 2;
        while(capacity <= total_write_size)
        {
            capacity *= 2;
        }

        gs_byte_buffer_resize(buffer, capacity);
    } 

    // memcpy data
     memcpy((buffer->data + buffer->position), src, size);

    buffer->size += (uint32_t)size;
    buffer->position += (uint32_t)size;
}

void gs_byte_buffer_read_bulk(gs_byte_buffer_t* buffer, void** dst, size_t size)
{
    memcpy(*dst, (buffer->data + buffer->position), size);
    buffer->position += (uint32_t)size;
}

void gs_byte_buffer_write_str(gs_byte_buffer_t* buffer, const char* str)
{
    // Write size of string
    uint32_t str_len = gs_string_length(str);
    gs_byte_buffer_write(buffer, uint16_t, str_len);

    size_t i; 
    for (i = 0; i < str_len; ++i)
    {
        gs_byte_buffer_write(buffer, uint8_t, str[i]);
    }
}

void gs_byte_buffer_read_str(gs_byte_buffer_t* buffer, char* str)
{
    // Read in size of string from buffer
    uint16_t sz;
    gs_byte_buffer_read(buffer, uint16_t, &sz);

    uint32_t i;
    for (i = 0; i < sz; ++i)
    {
        gs_byte_buffer_read(buffer, uint8_t, &str[i]);
    }
    str[i] = '\0';
}

GS_API_DECL void gs_byte_buffer_memset(gs_byte_buffer_t* buffer, uint8_t val)
{
    memset(buffer->data, val, buffer->capacity);
}

/*========================
// Dynamic Array
========================*/

GS_API_DECL void* 
gs_dyn_array_resize_impl(void* arr, size_t sz, size_t amount) 
{
    size_t capacity;

    if (arr) {
        capacity = amount;  
    } else {
        capacity = 0;
    }

    // Create new gs_dyn_array with just the header information
    gs_dyn_array* data = (gs_dyn_array*)gs_realloc(arr ? gs_dyn_array_head(arr) : 0, capacity * sz + sizeof(gs_dyn_array));

    if (data) {
        if (!arr) {
            data->size = 0;
        }
        data->capacity = (int32_t)capacity;
        return ((int32_t*)data + 2);
    }

    return NULL;
}

GS_API_DECL void** 
gs_dyn_array_init(void** arr, size_t val_len)
{
    if (*arr == NULL) {
        gs_dyn_array* data = (gs_dyn_array*)gs_malloc(val_len + sizeof(gs_dyn_array));  // Allocate capacity of one
        data->size = 0;
        data->capacity = 1;
        *arr = ((int32_t*)data + 2);
    }
    return arr;
}

GS_API_DECL void 
gs_dyn_array_push_data(void** arr, void* val, size_t val_len)
{
    if (*arr == NULL) {
        gs_dyn_array_init(arr, val_len);
    }
    if (gs_dyn_array_need_grow(*arr, 1)) 
    {
        int32_t capacity = gs_dyn_array_capacity(*arr) * 2;

        // Create new gs_dyn_array with just the header information
        gs_dyn_array* data = (gs_dyn_array*)gs_realloc(gs_dyn_array_head(*arr), capacity * val_len + sizeof(gs_dyn_array));

        if (data) {
            data->capacity = capacity;
            *arr = ((int32_t*)data + 2);
        }
    }
    size_t offset = gs_dyn_array_size(*arr);
    memcpy(((uint8_t*)(*arr)) + offset * val_len, val, val_len);
    gs_dyn_array_head(*arr)->size++;
}

/*========================
// Hash Table
========================*/

GS_API_DECL void 
__gs_hash_table_init_impl(void** ht, size_t sz)
{
    *ht = gs_malloc(sz);
}

/*========================
// Hash Set
========================*/

GS_API_DECL void 
__gs_hash_set_init_impl(void** hs, size_t sz)
{
    *hs = gs_malloc(sz);
}

/*========================
// Slot Array
========================*/

GS_API_DECL void** 
gs_slot_array_init(void** sa, size_t sz)
{
    if (*sa == NULL) {
        *sa = gs_malloc(sz);
        memset(*sa, 0, sz);
        return sa;
    }
    else {
        return NULL;
    }
}

/*========================
// Slot Map
========================*/

GS_API_DECL void** 
gs_slot_map_init(void** sm)
{
    if (*sm == NULL) {
        (*sm) = gs_malloc(sizeof(size_t) * 2);\
        memset((*sm), 0, sizeof(size_t) * 2);\
        return sm;
    }   
    return NULL;
}

/*========================
// Priotity Queue
========================*/

GS_API_DECL void**
gs_pqueue_init(void** pq, size_t sz)
{
    if (*pq == NULL) {
        (*pq) = gs_malloc(sz);
        memset((*pq), 0, sz);
        return pq;
    }
    return NULL;
}

GS_API_PRIVATE void
__gs_pqueue_pop_internal(void** pqueue, void* tmp, void** data, int32_t* priority, int32_t pq_sz, size_t d_sz)
{
    // TODO(): Remove these checks for perf
    if (!pqueue || !tmp || !data || !priority || !pq_sz || !d_sz) return;

    #define __SWP(__I0, __I1)\
    do {\
        {\
            size_t i0 = d_sz * (__I0);\
            size_t i1 = d_sz * (__I1);\
            uint8_t** d = (uint8_t**)data;\
            memcpy(tmp, ((char*)*(d)) + i0, d_sz);\
            memcpy(((char*)(*d)) + i0, ((char*)(*d)) + i1, d_sz);\
            memcpy(((char*)(*d)) + i1, tmp, d_sz);\
        }\
        {\
            int32_t t = 0;\
            uint32_t i0 = (__I0);\
            uint32_t i1 = (__I1);\
            uint32_t sz = sizeof(int32_t);\
            memcpy(&t, ((char*)((priority)) + i0), sz);\
            memcpy((char*)((priority) + i0), (char*)((priority) + i1), sz);\
            memcpy((char*)((priority) + i1), &t, sz);\
        }\
    } while (0)

    // Swap elements internal
    __SWP(0, pq_sz - 1);

    // Work down list from top until priority is sorted
    // THIS PART IS FUCKED
    // return;
    int32_t i = 0;
    int32_t c = 0;
    int32_t nwsz = pq_sz - 1;   // Right up until the last item we removed
    int32_t psz = gs_dyn_array_size(priority) - 1;
    // int32_t* pa = priority;
    for (int32_t i = 0; gs_pqueue_child_left_idx(i) < psz; i = c)
    {
        // Set child to smaller of two
        c = gs_pqueue_child_left_idx(i);

        // if (c >= psz) break;
        
        // Set to right child if valid and less priority
        if ((c + 1) < psz && priority[c + 1] < priority[c]) {
            c++;
        }

        // Check to swp, if necessary
        if (priority[i] > priority[c]) {
            __SWP(i, c);
        }
        // Otherwise, we're done
        else
        {
            break;
        }
    }

    // gs_println("P: %d", priority[nwsz - 1]);
}