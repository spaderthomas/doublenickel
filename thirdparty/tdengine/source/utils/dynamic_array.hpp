// Ripped verbatim from the excellent https://github.com/MrFrenik/gunslinger, since I never got around
// to making quite a few pure C data structures. Thank you kindly for your excellent code!

#ifndef DN_DYNAMIC_ARRAY_H
#define DN_DYNAMIC_ARRAY_H

typedef struct {
    u32 size;
    u32 capacity;
    u32 element_size;
    dn_allocator_t* allocator;
} dn_dynamic_array_header_t;

#define dn_dyn_array_t              void*
#define dn_dyn_array_element_t      void
#define DN_DYN_ARRAY_VOIDPP(ptr) ((void**)&ptr)
#define DN_DYN_ARRAY_VOIDP(ptr) ((void*)ptr)
#define dn_dyn_array(type) type*

DN_API dn_dyn_array_t             dn_dynamic_array_create(u32 element_size, dn_allocator_t* allocator);
DN_API dn_dyn_array_element_t*    dn_dynamic_array_push_n(dn_dyn_array_t* array, dn_dyn_array_element_t* data, u32 num_elements);
DN_API dn_dyn_array_element_t*    dn_dynamic_array_reserve(dn_dyn_array_t* array, u32 num_elements);
DN_API dn_dynamic_array_header_t* dn_dynamic_array_head(dn_dyn_array_t* array);
DN_API u32                        dn_dynamic_array_size(dn_dyn_array_t* array);
DN_API u32                        dn_dynamic_array_capacity(dn_dyn_array_t* array);
DN_API u32                        dn_dynamic_array_element_size(dn_dyn_array_t* array);
DN_API dn_allocator_t*            dn_dynamic_array_allocator(dn_dyn_array_t* array);
DN_API bool                       dn_dynamic_array_full(dn_dyn_array_t* array);
DN_API bool                       dn_dynamic_array_need_grow(dn_dyn_array_t* array, u32 num_elements);
DN_API void                       dn_dynamic_array_grow(dn_dyn_array_t* array, u32 requested_size);
DN_API u32                        dn_dynamic_array_byte_size(dn_dyn_array_t* array);

#define                          _dn_dynamic_array_create(element_size, allocator) dn_dynamic_array_create(element_size, allocator)
#define                          _dn_dynamic_array_push_n(array, data, num_elements) dn_dynamic_array_create(DN_DYN_ARRAY_VOIDPP(array), data, num_elements)
#define                          _dn_dynamic_array_reserve(array, num_elements) dn_dynamic_array_reserve(DN_DYN_ARRAY_VOIDPP(array), num_elements)
#define                          _dn_dynamic_array_head(array) dn_dynamic_array_head(DN_DYN_ARRAY_VOIDPP(array))
#define                          _dn_dynamic_array_size(array) dn_dynamic_array_size(DN_DYN_ARRAY_VOIDPP(array))
#define                          _dn_dynamic_array_capacity(array) dn_dynamic_array_capacity(DN_DYN_ARRAY_VOIDPP(array))
#define                          _dn_dynamic_array_element_size(array) dn_dynamic_array_element_size(DN_DYN_ARRAY_VOIDPP(array))
#define                          _dn_dynamic_array_allocator(array) dn_dynamic_array_allocator(DN_DYN_ARRAY_VOIDPP(array))
#define                          _dn_dynamic_array_full(array) dn_dynamic_array_full(DN_DYN_ARRAY_VOIDPP(array))
#define                          _dn_dynamic_array_need_grow(array, num_elements) dn_dynamic_array_need_grow(DN_DYN_ARRAY_VOIDPP(array), num_elements)
#define                          _dn_dynamic_array_grow(array, requested_size) dn_dynamic_array_grow(DN_DYN_ARRAY_VOIDPP(array), requested_size)
#define                          _dn_dynamic_array_byte_size(array) dn_dynamic_array_byte_size(DN_DYN_ARRAY_VOIDPP(array))

template<typename T>
T* dn_dynamic_array_create_t(dn_allocator_t* allocator) {
    return (T*)dn_dynamic_array_create(sizeof(T), allocator);
}

template<typename T>
T* dn_dynamic_array_push_t(T* dynamic_array, T value) {
    return (T*)dn_dynamic_array_push_n(DN_DYN_ARRAY_VOIDPP(dynamic_array), DN_DYN_ARRAY_VOIDP(&value), 1);
}


#endif

#ifdef DN_DYNAMIC_ARRAY_IMPLEMENTATION

dn_dyn_array_t dn_dynamic_array_create(u32 element_size, dn_allocator_t* allocator) {
    assert(allocator);
 
    dn_dynamic_array_header_t* header = (dn_dynamic_array_header_t*)dn_allocator_alloc(allocator, sizeof(dn_dynamic_array_header_t) + element_size);
    header->size = 0;
    header->capacity = 1;
    header->element_size = element_size;
    header->allocator = allocator;
    return header + 1;
}

dn_dynamic_array_header_t* dn_dynamic_array_head(dn_dyn_array_t* array) {
    return ((dn_dynamic_array_header_t*)*array) - 1;
}

u32 dn_dynamic_array_byte_size(dn_dyn_array_t* array) {
    return dn_dynamic_array_size(array) * dn_dynamic_array_element_size(array);
}

u32 dn_dynamic_array_size(dn_dyn_array_t* array) {
    return dn_dynamic_array_head(array)->size;
}

u32 dn_dynamic_array_capacity(dn_dyn_array_t* array) {
    return dn_dynamic_array_head(array)->capacity;
}

u32 dn_dynamic_array_element_size(dn_dyn_array_t* array) {
    return dn_dynamic_array_head(array)->element_size;
}

dn_allocator_t* dn_dynamic_array_allocator(dn_dyn_array_t* array) {
    return dn_dynamic_array_head(array)->allocator;
}

bool dn_dynamic_array_need_grow(dn_dyn_array_t* array, u32 num_elements) {
    return dn_dynamic_array_size(array) + num_elements >= dn_dynamic_array_capacity(array);
}

void dn_dynamic_array_grow(dn_dyn_array_t* array, u32 requested_size) {
    while (dn_dynamic_array_capacity(array) < requested_size) {
        dn_dynamic_array_head(array)->capacity = dn_dynamic_array_capacity(array) * 2;

        dn_dynamic_array_header_t* header = (dn_dynamic_array_header_t*)dn_allocator_realloc(
            dn_dynamic_array_allocator(array), 
            dn_dynamic_array_head(array),
            dn_dynamic_array_capacity(array) * dn_dynamic_array_element_size(array) + sizeof(dn_dynamic_array_header_t)
        );

        *array = header + 1;
    }
}

void* dn_dynamic_array_reserve(dn_dyn_array_t* array, u32 num_elements) {
    assert(array);

    if (dn_dynamic_array_need_grow(array, num_elements)) {
        u32 size = dn_dynamic_array_size(array);
        dn_dynamic_array_grow(array, size + num_elements);
    }

    u8* memory = (u8*)(*array);
    u8* reserved_memory =  memory + dn_dynamic_array_byte_size(array);
    dn_dynamic_array_head(array)->size += num_elements;

    return reserved_memory;
}

#define dn_dynamic_array_push(__ARR, __ARRVAL)\
    do {\
        if (dn_dynamic_array_need_grow(__ARR, 1)) {\
            dn_dynamic_array_grow(__ARR, dn_dynamic_array_size(__ARR) + 1); \
        }\
        (__ARR)[dn_dynamic_array_size(__ARR)] = (__ARRVAL);\
        dn_dynamic_array_head(__ARR)->size++;\
    } while(0)

void* dn_dynamic_array_push_n(dn_dyn_array_t* array, dn_dyn_array_element_t* data, u32 num_elements) {
    void* memory = dn_dynamic_array_reserve(array, num_elements);
    copy_memory(data, memory, dn_dynamic_array_element_size(array) * num_elements);
    return memory;
}
#endif
