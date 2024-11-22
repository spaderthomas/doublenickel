// Ripped verbatim from the excellent https://github.com/MrFrenik/gunslinger, since I never got around
// to making quite a few pure C data structures. Thank you kindly for your excellent code!

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

typedef struct dyn_array
{
    u32 size;
    u32 capacity;
    u32 element_size;
    MemoryAllocator* allocator;
} dyn_array_header;

#define dyn_array void*
#define DYN_ARRAY_VOIDP(ptr) ((void**)&ptr)
#define DYN_ARRAY(type) type*

FM_LUA_EXPORT dyn_array         _dyn_array_alloc(u32 element_size, MemoryAllocator* allocator);
#define                         dyn_array_alloc(element_size, allocator) _dyn_array_alloc(element_size, allocator)
FM_LUA_EXPORT void              _dyn_array_push_n(dyn_array* array, void* data, u32 num_elements);
#define                          dyn_array_push_n(array, data, num_elements) _dyn_array_alloc(DYN_ARRAY_VOIDP(array), data, num_elements)
FM_LUA_EXPORT void*             _dyn_array_reserve(dyn_array* array, u32 num_elements);
#define                          dyn_array_reserve(array, num_elements) _dyn_array_reserve(DYN_ARRAY_VOIDP(array), num_elements)
FM_LUA_EXPORT dyn_array_header* _dyn_array_head(dyn_array* array);
#define                          dyn_array_head(array) _dyn_array_head(DYN_ARRAY_VOIDP(array))
FM_LUA_EXPORT u32               _dyn_array_size(dyn_array* array);
#define                          dyn_array_size(array) _dyn_array_size(DYN_ARRAY_VOIDP(array))
FM_LUA_EXPORT u32               _dyn_array_capacity(dyn_array* array);
#define                          dyn_array_capacity(array) _dyn_array_capacity(DYN_ARRAY_VOIDP(array))
FM_LUA_EXPORT u32               _dyn_array_element_size(dyn_array* array);
#define                          dyn_array_element_size(array) _dyn_array_element_size(DYN_ARRAY_VOIDP(array))
FM_LUA_EXPORT MemoryAllocator*  _dyn_array_allocator(dyn_array* array);
#define                          dyn_array_allocator(array) _dyn_array_allocator(DYN_ARRAY_VOIDP(array))
FM_LUA_EXPORT bool              _dyn_array_full(dyn_array* array);
#define                          dyn_array_full(array) _dyn_array_full(DYN_ARRAY_VOIDP(array))
FM_LUA_EXPORT bool              _dyn_array_need_grow(dyn_array* array, u32 num_elements);
#define                          dyn_array_need_grow(array, num_elements) _dyn_array_need_grow(DYN_ARRAY_VOIDP(array), num_elements)
FM_LUA_EXPORT void              _dyn_array_grow(dyn_array* array, u32 requested_size);
#define                          dyn_array_grow(array, requested_size) _dyn_array_grow(DYN_ARRAY_VOIDP(array), requested_size)
FM_LUA_EXPORT u32               _dyn_array_byte_size(dyn_array* array);
#define                          dyn_array_byte_size(array) _dyn_array_byte_size(DYN_ARRAY_VOIDP(array))

#endif

#ifdef DYNAMIC_ARRAY_IMPLEMENTATION

template<typename T>
T* dyn_array_alloc_t(MemoryAllocator* allocator) {
    return (T*)dyn_array_alloc(sizeof(T), allocator);
}

dyn_array _dyn_array_alloc(u32 element_size, MemoryAllocator* allocator) {
    assert(allocator);
 
    dyn_array_header* header = (dyn_array_header*)ma_alloc(allocator, sizeof(dyn_array_header) + element_size);
    header->size = 0;
    header->capacity = 1;
    header->element_size = element_size;
    header->allocator = allocator;
    return header + 1;
}

dyn_array_header* _dyn_array_head(dyn_array* array) {
    return ((dyn_array_header*)*array) - 1;
}

u32 _dyn_array_byte_size(dyn_array* array) {
    return _dyn_array_size(array) * _dyn_array_element_size(array);
}

u32 _dyn_array_size(dyn_array* array) {
    return _dyn_array_head(array)->size;
}

u32 _dyn_array_capacity(dyn_array* array) {
    return _dyn_array_head(array)->capacity;
}

u32 _dyn_array_element_size(dyn_array* array) {
    return _dyn_array_head(array)->element_size;
}

MemoryAllocator* _dyn_array_allocator(dyn_array* array) {
    return _dyn_array_head(array)->allocator;
}

bool _dyn_array_need_grow(dyn_array* array, u32 num_elements) {
    return _dyn_array_size(array) + num_elements >= _dyn_array_capacity(array);
}

void _dyn_array_grow(dyn_array* array, u32 requested_size) {
    while (_dyn_array_capacity(array) < requested_size) {
        _dyn_array_head(array)->capacity = _dyn_array_capacity(array) * 2;

        dyn_array_header* header = (dyn_array_header*)ma_realloc(
            _dyn_array_allocator(array), 
            _dyn_array_head(array),
            _dyn_array_capacity(array) * _dyn_array_element_size(array) + sizeof(dyn_array_header)
        );

        *array = header + 1;
    }
}

void* _dyn_array_reserve(dyn_array* array, u32 num_elements) {
    assert(array);

    if (_dyn_array_need_grow(array, num_elements)) {
        u32 size = _dyn_array_size(array);
        _dyn_array_grow(array, size + num_elements);
    }

    u8* memory = (u8*)(*array);
    u8* reserved_memory =  memory + _dyn_array_byte_size(array);
    _dyn_array_head(array)->size += num_elements;

    return reserved_memory;
}

#define dyn_array_push(__ARR, __ARRVAL)\
    do {\
        if (dyn_array_need_grow(__ARR, 1)) {\
            dyn_array_grow(__ARR, dyn_array_size(__ARR) + 1); \
        }\
        (__ARR)[dyn_array_size(__ARR)] = (__ARRVAL);\
        dyn_array_head(__ARR)->size++;\
    } while(0)

void _dyn_array_push_n(dyn_array* array, void* data, u32 num_elements) {

    void* memory = _dyn_array_reserve(array, num_elements);
    copy_memory(data, memory, _dyn_array_element_size(array) * num_elements);
}
#endif