//#define SLOT_ARRAY_INVALID_HANDLE    UINT32_MAX
//
//#define slot_array_handle_valid(__SA, __ID)\
//    ((__SA) && __ID < dyn_array_size((__SA)->indices) && (__SA)->indices[__ID] != SLOT_ARRAY_INVALID_HANDLE)
//
//typedef struct __slot_array_dummy_header {
//    dyn_array(uint32_t) indices;
//    dyn_array(uint32_t) data;
//} __slot_array_dummy_header;
//
//#define slot_array(__T)\
//    struct\
//    {\
//        dyn_array(uint32_t) indices;\
//        dyn_array(__T) data;\
//        __T tmp;\
//    }*
//
//#define slot_array_new(__T)\
//    NULL
//
//uint32_t __slot_array_find_next_available_index(dyn_array(uint32_t) indices)
//{
//    uint32_t idx = SLOT_ARRAY_INVALID_HANDLE;
//    for (uint32_t i = 0; i < (uint32_t)dyn_array_size(indices); ++i)
//    {
//        uint32_t handle = indices[i];
//        if (handle == SLOT_ARRAY_INVALID_HANDLE)
//        {
//            idx = i;
//            break;
//        }
//    }
//    if (idx == SLOT_ARRAY_INVALID_HANDLE)
//    {
//        idx = dyn_array_size(indices);
//    }
//
//    return idx;
//}
//
//void** slot_array_init(void** sa, size_t sz);
//
//#define slot_array_init_all(__SA)\
//    (slot_array_init((void**)&(__SA), sizeof(*(__SA))), dyn_array_init((void**)&((__SA)->indices), sizeof(uint32_t)),\
//        dyn_array_init((void**)&((__SA)->data), sizeof((__SA)->tmp)))
//
//uint32_t slot_array_insert_func(void** indices, void** data, void* val, size_t val_len, uint32_t* ip)
//{
//    // Find next available index
//    u32 idx = __slot_array_find_next_available_index((uint32_t*)*indices);
//
//    if (idx == dyn_array_size(*indices)) {
//        uint32_t v = 0;
//        dyn_array_push_data(indices, &v, sizeof(uint32_t));  
//        idx = dyn_array_size(*indices) - 1;
//    }
//
//    // Push data to array
//    dyn_array_push_data(data, val, val_len);
//
//    // Set data in indices
//    uint32_t bi = dyn_array_size(*data) - 1;
//    dyn_array_set_data_i(indices, &bi, sizeof(uint32_t), idx);
//
//    if (ip){
//        *ip = idx;
//    }
//
//    return idx;
//}
//
//#define slot_array_reserve(__SA, __NUM)\
//    do {\
//        slot_array_init_all(__SA);\
//        dyn_array_reserve((__SA)->data, __NUM);\
//        dyn_array_reserve((__SA)->indices, __NUM);\
//    } while (0)
//
//#define slot_array_insert(__SA, __VAL)\
//    (slot_array_init_all(__SA), (__SA)->tmp = (__VAL),\
//        slot_array_insert_func((void**)&((__SA)->indices), (void**)&((__SA)->data), (void*)&((__SA)->tmp), sizeof(((__SA)->tmp)), NULL))
//
//#define slot_array_insert_hp(__SA, __VAL, __hp)\
//    (slot_array_init_all(__SA), (__SA)->tmp = (__VAL),\
//        slot_array_insert_func((void**)&((__SA)->indices), (void**)&((__SA)->data), &((__SA)->tmp), sizeof(((__SA)->tmp)), (__hp)))
//
//#define slot_array_insert_no_init(__SA, __VAL)\
//    ((__SA)->tmp = (__VAL), slot_array_insert_func((void**)&((__SA)->indices), (void**)&((__SA)->data), &((__SA)->tmp), sizeof(((__SA)->tmp)), NULL))
//
//#define slot_array_size(__SA)\
//    ((__SA) == NULL ? 0 : dyn_array_size((__SA)->data))
//
// #define slot_array_empty(__SA)\
//    (slot_array_size(__SA) == 0)
//
//#define slot_array_clear(__SA)\
//    do {\
//        if ((__SA) != NULL) {\
//            dyn_array_clear((__SA)->data);\
//            dyn_array_clear((__SA)->indices);\
//        }\
//    } while (0)
//
//#define slot_array_exists(__SA, __SID)\
//    ((__SA) && (__SID) < (uint32_t)dyn_array_size((__SA)->indices) && (__SA)->indices[__SID] != SLOT_ARRAY_INVALID_HANDLE)
//
// #define slot_array_get(__SA, __SID)\
//    ((__SA)->data[(__SA)->indices[(__SID) % dyn_array_size(((__SA)->indices))]])
//
// #define slot_array_getp(__SA, __SID)\
//    (&(slot_array_get(__SA, (__SID))))
//
// #define slot_array_free(__SA)\
//    do {\
//        if ((__SA) != NULL) {\
//            dyn_array_free((__SA)->data);\
//            dyn_array_free((__SA)->indices);\
//            (__SA)->indices = NULL;\
//            (__SA)->data = NULL;\
//            free((__SA));\
//            (__SA) = NULL;\
//        }\
//    } while (0)
//
// #define slot_array_erase(__SA, __id)\
//    do {\
//        uint32_t __H0 = (__id) /*% dyn_array_size((__SA)->indices)*/;\
//        if (slot_array_size(__SA) == 1) {\
//            slot_array_clear(__SA);\
//        }\
//        else if (!slot_array_handle_valid(__SA, __H0)) {\
//            println("Warning: Attempting to erase invalid slot array handle (%zu)", __H0);\
//        }\
//        else {\
//            uint32_t __OG_DATA_IDX = (__SA)->indices[__H0];\
//            /* Iterate through handles until last index of data found */\
//            uint32_t __H = 0;\
//            for (uint32_t __I = 0; __I < dyn_array_size((__SA)->indices); ++__I)\
//            {\
//                if ((__SA)->indices[__I] == dyn_array_size((__SA)->data) - 1)\
//                {\
//                    __H = __I;\
//                    break;\
//                }\
//            }\
//        \
//            /* Swap and pop data */\
//            (__SA)->data[__OG_DATA_IDX] = dyn_array_back((__SA)->data);\
//            dyn_array_pop((__SA)->data);\
//        \
//            /* Point new handle, Set og handle to invalid */\
//            (__SA)->indices[__H] = __OG_DATA_IDX;\
//            (__SA)->indices[__H0] = SLOT_ARRAY_INVALID_HANDLE;\
//        }\
//    } while (0)
//
///*=== Slot Array Iterator ===*/
//
//// Slot array iterator new
//typedef uint32_t slot_array_iter;
//
//#define slot_array_iter_valid(__SA, __IT)\
//    (__SA && slot_array_exists(__SA, __IT))
//
//void _slot_array_iter_advance_func(dyn_array(uint32_t) indices, uint32_t* it)
//{
//    if (!indices) {
//       *it = SLOT_ARRAY_INVALID_HANDLE; 
//        return;
//    }
//
//    (*it)++;
//    for (; *it < (uint32_t)dyn_array_size(indices); ++*it)
//    {\
//        if (indices[*it] != SLOT_ARRAY_INVALID_HANDLE)\
//        {\
//            break;\
//        }\
//    }\
//}
//
//uint32_t _slot_array_iter_find_first_valid_index(dyn_array(uint32_t) indices)
//{
//    if (!indices) return SLOT_ARRAY_INVALID_HANDLE;
//
//    for (uint32_t i = 0; i < (uint32_t)dyn_array_size(indices); ++i)
//    {
//        if (indices[i] != SLOT_ARRAY_INVALID_HANDLE)
//        {
//            return i;
//        }
//    }
//    return SLOT_ARRAY_INVALID_HANDLE;
//}
//
//#define slot_array_iter_new(__SA) (_slot_array_iter_find_first_valid_index((__SA) ? (__SA)->indices : 0))
//
//#define slot_array_iter_advance(__SA, __IT)\
//    _slot_array_iter_advance_func((__SA) ? (__SA)->indices : NULL, &(__IT))
//
//#define slot_array_iter_get(__SA, __IT)\
//    slot_array_get(__SA, __IT)
//
//#define slot_array_iter_getp(__SA, __IT)\
//    slot_array_getp(__SA, __IT)