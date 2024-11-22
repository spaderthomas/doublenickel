//
//#define HASH_TABLE_HASH_SEED         0x31415296
//#define HASH_TABLE_INVALID_INDEX     UINT32_MAX
//
//typedef enum hash_table_entry_state
//{
//    HASH_TABLE_ENTRY_INACTIVE = 0x00,
//    HASH_TABLE_ENTRY_ACTIVE = 0x01
//} hash_table_entry_state;
//
//#define __hash_table_entry(__HMK, __HMV)\
//    struct\
//    {\
//        __HMK key;\
//        __HMV val;\
//        hash_table_entry_state state;\
//    }
//
//#define hash_table(__HMK, __HMV)\
//    struct {\
//        __hash_table_entry(__HMK, __HMV)* data;\
//        __HMK tmp_key;\
//        __HMV tmp_val;\
//        size_t stride;\
//        size_t klpvl;\
//        size_t tmp_idx;\
//    }*
//
//// Need a way to create a temporary key so I can take the address of it
//
//#define hash_table_new(__K, __V)\
//    NULL
//
//void  __hash_table_init_impl(void** ht, size_t sz);
//
//#define hash_table_init(__HT, __K, __V)\
//    do {\
//        size_t entry_sz = sizeof(*__HT->data);\
//        size_t ht_sz = sizeof(*__HT);\
//        __hash_table_init_impl((void**)&(__HT), ht_sz);\
//        memset((__HT), 0, ht_sz);\
//        dyn_array_reserve(__HT->data, 2);\
//        __HT->data[0].state = HASH_TABLE_ENTRY_INACTIVE;\
//        __HT->data[1].state = HASH_TABLE_ENTRY_INACTIVE;\
//        uintptr_t d0 = (uintptr_t)&((__HT)->data[0]);\
//        uintptr_t d1 = (uintptr_t)&((__HT)->data[1]);\
//        ptrdiff_t diff = (d1 - d0);\
//        ptrdiff_t klpvl = (uintptr_t)&(__HT->data[0].state) - (uintptr_t)(&__HT->data[0]);\
//        (__HT)->stride = (size_t)(diff);\
//        (__HT)->klpvl = (size_t)(klpvl);\
//    } while (0)
//
//#define hash_table_reserve(_HT, _KT, _VT, _CT)\
//    do {\
//        if ((_HT) == NULL) {\
//            hash_table_init((_HT), _KT, _VT);\
//        }\
//        dyn_array_reserve((_HT)->data, _CT);\
//    } while (0)
//
//    // ((__HT) != NULL ? (__HT)->size : 0) // dyn_array_size((__HT)->data) : 0)
//#define hash_table_size(__HT)\
//    ((__HT) != NULL ? dyn_array_size((__HT)->data) : 0)
//
//#define hash_table_capacity(__HT)\
//    ((__HT) != NULL ? dyn_array_capacity((__HT)->data) : 0)
//
//#define hash_table_load_factor(__HT)\
//    (hash_table_capacity(__HT) ? (float)(hash_table_size(__HT)) / (float)(hash_table_capacity(__HT)) : 0.f)
//
//#define hash_table_grow(__HT, __C)\
//    ((__HT)->data = dyn_array_resize_impl((__HT)->data, sizeof(*((__HT)->data)), (__C)))
//
//#define hash_table_empty(__HT)\
//    ((__HT) != NULL ? dyn_array_size((__HT)->data) == 0 : true)
//
//#define hash_table_clear(__HT)\
//    do {\
//        if ((__HT) != NULL) {\
//            u32 capacity = dyn_array_capacity((__HT)->data);\
//            for (u32 i = 0; i < capacity; ++i) {\
//                (__HT)->data[i].state = HASH_TABLE_ENTRY_INACTIVE;\
//            }\
//            /*memset((__HT)->data, 0, dyn_array_capacity((__HT)->data) * );*/\
//            dyn_array_clear((__HT)->data);\
//        }\
//    } while (0)
//
//#define hash_table_free(__HT)\
//    do {\
//        if ((__HT) != NULL) {\
//            dyn_array_free((__HT)->data);\
//            (__HT)->data = NULL;\
//            free(__HT);\
//            (__HT) = NULL;\
//        }\
//    } while (0)
//
//// Find available slot to insert k/v pair into
//#define hash_table_insert(__HT, __HMK, __HMV)\
//    do {\
//        /* Check for null hash table, init if necessary */\
//        if ((__HT) == NULL) {\
//            hash_table_init((__HT), (__HMK), (__HMV));\
//        }\
//    \
//        /* Grow table if necessary */\
//        u32 __CAP = hash_table_capacity(__HT);\
//        float __LF = hash_table_load_factor(__HT);\
//        if (__LF >= 0.5f || !__CAP)\
//        {\
//            u32 NEW_CAP = __CAP ? __CAP * 2 : 2;\
//            size_t ENTRY_SZ = sizeof((__HT)->tmp_key) + sizeof((__HT)->tmp_val) + sizeof(hash_table_entry_state);\
//            dyn_array_reserve((__HT)->data, NEW_CAP);\
//            /**((void **)&(__HT->data)) = dyn_array_resize_impl(__HT->data, ENTRY_SZ, NEW_CAP);*/\
//            /* Iterate through data and set state to null, from __CAP -> __CAP * 2 */\
//            /* Memset here instead */\
//            for (u32 __I = __CAP; __I < NEW_CAP; ++__I) {\
//                (__HT)->data[__I].state = HASH_TABLE_ENTRY_INACTIVE;\
//            }\
//            __CAP = hash_table_capacity(__HT);\
//        }\
//    \
//        /* Get hash of key */\
//        (__HT)->tmp_key = (__HMK);\
//        size_t __HSH = hash_bytes((void*)&((__HT)->tmp_key), sizeof((__HT)->tmp_key), HASH_TABLE_HASH_SEED);\
//        size_t __HSH_IDX = __HSH % __CAP;\
//        (__HT)->tmp_key = (__HT)->data[__HSH_IDX].key;\
//        u32 c = 0;\
//    \
//        /* Find valid idx and place data */\
//        while (\
//            c < __CAP\
//            && __HSH != hash_bytes((void*)&(__HT)->tmp_key, sizeof((__HT)->tmp_key), HASH_TABLE_HASH_SEED)\
//            && (__HT)->data[__HSH_IDX].state == HASH_TABLE_ENTRY_ACTIVE)\
//        {\
//            __HSH_IDX = ((__HSH_IDX + 1) % __CAP);\
//            (__HT)->tmp_key = (__HT)->data[__HSH_IDX].key;\
//            ++c;\
//        }\
//        (__HT)->data[__HSH_IDX].key = (__HMK);\
//        (__HT)->data[__HSH_IDX].val = (__HMV);\
//        (__HT)->data[__HSH_IDX].state = HASH_TABLE_ENTRY_ACTIVE;\
//        dyn_array_head((__HT)->data)->size++;\
//    } while (0)
//
//// Need size difference between two entries
//// Need size of key + val
//
//u32 hash_table_get_key_index_func(void** data, void* key, size_t key_len, size_t val_len, size_t stride, size_t klpvl)
//{
//    if (!data || !key) return HASH_TABLE_INVALID_INDEX;
//
//    // Need a better way to handle this. Can't do it like this anymore.
//    // Need to fix this. Seriously messing me up.
//    u32 capacity = dyn_array_capacity(*data);
//	u32 size = dyn_array_size(*data);
//	if (!capacity || !size) return (size_t)HASH_TABLE_INVALID_INDEX;
//    size_t idx = (size_t)HASH_TABLE_INVALID_INDEX;
//    size_t hash = (size_t)hash_bytes(key, key_len, HASH_TABLE_HASH_SEED);
//    size_t hash_idx = (hash % capacity);
//
//    // Iterate through data 
//    for (size_t i = hash_idx, c = 0; c < capacity; ++c, i = ((i + 1) % capacity))
//    {
//        size_t offset = (i * stride);
//        void* k = ((char*)(*data) + (offset));  
//        size_t kh = hash_bytes(k, key_len, HASH_TABLE_HASH_SEED);
//        bool comp = compare_bytes(k, key, key_len);
//        hash_table_entry_state state = *(hash_table_entry_state*)((char*)(*data) + offset + (klpvl)); 
//        if (comp && hash == kh && state == HASH_TABLE_ENTRY_ACTIVE) {
//            idx = i;
//            break;
//        }
//    }
//    return (u32)idx;
//}
//
//// Get key at index
//#define hash_table_getk(__HT, __I)\
//    (((__HT))->data[(__I)].key)
//
//// Get val at index
//#define hash_table_geti(__HT, __I)\
//    ((__HT)->data[(__I)].val)
//
//// Could search for the index in the macro instead now. Does this help me?
//#define hash_table_get(__HT, __HTK)\
//    ((__HT)->tmp_key = (__HTK),\
//        (hash_table_geti((__HT),\
//            hash_table_get_key_index_func((void**)&(__HT)->data, (void*)&((__HT)->tmp_key),\
//                sizeof((__HT)->tmp_key), sizeof((__HT)->tmp_val), (__HT)->stride, (__HT)->klpvl)))) 
//
//#define hash_table_getp(__HT, __HTK)\
//    (\
//        (__HT)->tmp_key = (__HTK),\
//        ((__HT)->tmp_idx = (u32)hash_table_get_key_index_func((void**)&(__HT->data), (void*)&(__HT->tmp_key), sizeof(__HT->tmp_key),\
//            sizeof(__HT->tmp_val), __HT->stride, __HT->klpvl)),\
//        ((__HT)->tmp_idx != HASH_TABLE_INVALID_INDEX ? &hash_table_geti((__HT), (__HT)->tmp_idx) : NULL)\
//    )
//
//#define _hash_table_key_exists_internal(__HT, __HTK)\
//    ((__HT)->tmp_key = (__HTK),\
//        (hash_table_get_key_index_func((void**)&(__HT->data), (void*)&(__HT->tmp_key), sizeof(__HT->tmp_key),\
//            sizeof(__HT->tmp_val), __HT->stride, __HT->klpvl) != HASH_TABLE_INVALID_INDEX))
//
//// u32 hash_table_get_key_index_func(void** data, void* key, size_t key_len, size_t val_len, size_t stride, size_t klpvl)
//
//#define hash_table_exists(__HT, __HTK)\
//        (__HT && _hash_table_key_exists_internal((__HT), (__HTK)))
//
//#define hash_table_key_exists(__HT, __HTK)\
//		(hash_table_exists((__HT), (__HTK)))
//
//#define hash_table_erase(__HT, __HTK)\
//    do {\
//        if ((__HT))\
//        {\
//            /* Get idx for key */\
//            (__HT)->tmp_key = (__HTK);\
//            u32 __IDX = hash_table_get_key_index_func((void**)&(__HT)->data, (void*)&((__HT)->tmp_key), sizeof((__HT)->tmp_key), sizeof((__HT)->tmp_val), (__HT)->stride, (__HT)->klpvl);\
//            if (__IDX != HASH_TABLE_INVALID_INDEX) {\
//                (__HT)->data[__IDX].state = HASH_TABLE_ENTRY_INACTIVE;\
//                if (dyn_array_head((__HT)->data)->size) dyn_array_head((__HT)->data)->size--;\
//            }\
//        }\
//    } while (0)
//
///*===== Hash Table Iterator ====*/
//
//typedef u32 hash_table_iter;
//typedef hash_table_iter hash_table_iter_t;
//
//u32 __find_first_valid_iterator(void* data, size_t key_len, size_t val_len, u32 idx, size_t stride, size_t klpvl)
//{
//    u32 it = (u32)idx;
//    for (; it < (u32)dyn_array_capacity(data); ++it)
//    {
//        size_t offset = (it * stride);
//        hash_table_entry_state state = *(hash_table_entry_state*)((uint8_t*)data + offset + (klpvl));
//        if (state == HASH_TABLE_ENTRY_ACTIVE)
//        {
//            break;
//        }
//    }
//    return it;
//}
//
///* Find first valid iterator idx */
//#define hash_table_iter_new(__HT)\
//    (__HT ? __find_first_valid_iterator((__HT)->data, sizeof((__HT)->tmp_key), sizeof((__HT)->tmp_val), 0, (__HT)->stride, (__HT)->klpvl) : 0)
//
//#define hash_table_iter_valid(__HT, __IT)\
//    ((__IT) < hash_table_capacity((__HT)))
//
//// Have to be able to do this for hash table...
//void __hash_table_iter_advance_func(void** data, size_t key_len, size_t val_len, u32* it, size_t stride, size_t klpvl)
//{
//    (*it)++;
//    for (; *it < (u32)dyn_array_capacity(*data); ++*it)
//    {
//        size_t offset = (size_t)(*it * stride);
//        hash_table_entry_state state = *(hash_table_entry_state*)((uint8_t*)*data + offset + (klpvl));
//        if (state == HASH_TABLE_ENTRY_ACTIVE)
//        {
//            break;
//        }
//    }
//}
//
//#define hash_table_find_valid_iter(__HT, __IT)\
//    ((__IT) = __find_first_valid_iterator((void**)&(__HT)->data, sizeof((__HT)->tmp_key), sizeof((__HT)->tmp_val), (__IT), (__HT)->stride, (__HT)->klpvl))
//
//#define hash_table_iter_advance(__HT, __IT)\
//    (__hash_table_iter_advance_func((void**)&(__HT)->data, sizeof((__HT)->tmp_key), sizeof((__HT)->tmp_val), &(__IT), (__HT)->stride, (__HT)->klpvl))
//
//#define hash_table_iter_get(__HT, __IT)\
//    hash_table_geti(__HT, __IT)
//
//#define hash_table_iter_getp(__HT, __IT)\
//    (&(hash_table_geti(__HT, __IT)))
//
//#define hash_table_iter_getk(__HT, __IT)\
//    (hash_table_getk(__HT, __IT))
//
//#define hash_table_iter_getkp(__HT, __IT)\
//    (&(hash_table_getk(__HT, __IT)))