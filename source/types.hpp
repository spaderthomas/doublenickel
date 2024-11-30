#ifndef DN_NO_HIJACK_NUMERIC_TYPES
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;
#endif

typedef size_t dn_hash_t;

#define DN_ASSET_NAME_LEN 64
typedef char dn_asset_name_t [DN_ASSET_NAME_LEN];

typedef char* dn_tstring_t;

typedef HMM_Vec2 dn_vector2_t;
typedef HMM_Vec3 dn_vector3_t;
typedef HMM_Vec4 dn_vector4_t;
struct dn_vector2i_t {
	i32 x = 0;
	i32 y = 0;
};

#define dn_quad_literal(top, bottom, left, right) \
    {                                     \
        { left, top },                    \
        { left, bottom },                 \
        { right, bottom },                \
                                          \
        { left, top },                    \
        { right, bottom },                \
        { right, top }                    \
    }                                     
