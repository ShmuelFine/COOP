#ifndef __COOP__LIST__H_
#define __COOP__LIST__H_

#include "COOP.h"
#include "Iterator.h"
#include "SharedObjPtr.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

DEF_CLASS(ListNode);
    ListNode* prev;
    ListNode* next;
    unsigned char payload[];   /* element bytes (length = elementSize) */
END_DEF(ListNode);

/* ===== GenericList (homogeneous by elementSize) ===== */
DEF_CLASS(GenericList);
    MEM_SIZE_T size;           /* number of elements */
    MEM_SIZE_T elementSize;    /* size of each element in bytes */
    ListNode* head;            /* first node */
    ListNode* tail;            /* last node */
END_DEF(GenericList)


/* ===== Iterator derived from Iterator =====
   node == NULL => end()
*/
DEF_DERIVED_CLASS(ListIterator, Iterator)
    GenericList* owner;
    ListNode* node;
END_DEF_DERIVED(ListIterator)

/* ===== Ctors/Methods Declarations ===== */
FUNCTIONS(GenericList, MEM_SIZE_T dataTypeSize);

    /* ---- Generic (untyped, uses _this->elementSize) ---- */
    MEM_FUN_DECL(GenericList, push_back_generic, const void* src);
    MEM_FUN_DECL(GenericList, push_front_generic, const void* src);
    MEM_FUN_DECL(GenericList, pop_back_generic, void* out_elem);
    MEM_FUN_DECL(GenericList, pop_front_generic, void* out_elem);
    MEM_FUN_DECL(GenericList, front_generic, void* out_elem);
    MEM_FUN_DECL(GenericList, back_generic, void* out_elem);

    MEM_FUN_DECL(GenericList, __make_node, const void* src_bytes, ListNode** out_node);

    MEM_FUN_DECL(GenericList, size, MEM_SIZE_T* out_size);
    MEM_FUN_DECL(GenericList, empty, bool* out_is_empty);
    MEM_FUN_DECL(GenericList, clear);


    /* ---- Iterators ---- */
    MEM_FUN_DECL(GenericList, begin, ListIterator** out_it); /* head or end */
    MEM_FUN_DECL(GenericList, end, ListIterator** out_it); /* node = NULL */
    MEM_FUN_DECL(GenericList, insert_before_generic, ListIterator* pos, const void* src, ListIterator** out_it);
    MEM_FUN_DECL(GenericList, insert_after_generic, ListIterator* pos, const void* src, ListIterator** out_it);
    MEM_FUN_DECL(GenericList, erase_generic, ListIterator* pos, void* out_elem, ListIterator** out_next);

END_FUNCTIONS(GenericList);

/* ===== ListIterator overrides ===== */
DERIVED_FUNCTIONS(ListIterator, Iterator);
    FUN_OVERRIDE(Iterator, equals, object* other, bool* out_equal);
    FUN_OVERRIDE(Iterator, next);
    FUN_OVERRIDE(Iterator, prev);
    FUN_OVERRIDE(Iterator, get_ref, void** out_ptr);
    FUN_OVERRIDE(Iterator, get_cref, const void** out_ptr);
    FUN_OVERRIDE(Iterator, distance, object* other, ptrdiff_t* out_dist);
    FUN_OVERRIDE(Iterator, advance, ptrdiff_t n);
END_DERIVED_FUNCTIONS(ListIterator);



/* ===== Typed Facade Macro (creates a derived type per C type) ===== */
#define DECLARE_SPECIFIC_LIST_TYPE(type_name, c_type)                              \
DEF_DERIVED_CLASS(List_##type_name, GenericList);                                  \
END_DEF_DERIVED(List_##type_name);                                                 \
DERIVED_FUNCTIONS(List_##type_name, GenericList);                                  \
    MEM_FUN_DECL(List_##type_name, push_back,      c_type val);                    \
    MEM_FUN_DECL(List_##type_name, push_front,     c_type val);                    \
    MEM_FUN_DECL(List_##type_name, pop_back,       c_type* out_val);               \
    MEM_FUN_DECL(List_##type_name, pop_front,      c_type* out_val);               \
    MEM_FUN_DECL(List_##type_name, front,          c_type* out_val);               \
    MEM_FUN_DECL(List_##type_name, back,           c_type* out_val);               \
    MEM_FUN_DECL(List_##type_name, insert_before,  ListIterator* pos, c_type val,  \
                 ListIterator** out_it);                                           \
    MEM_FUN_DECL(List_##type_name, insert_after,   ListIterator* pos, c_type val,  \
                 ListIterator** out_it);                                           \
    MEM_FUN_DECL(List_##type_name, erase,          ListIterator* pos,              \
                 c_type* out_val, ListIterator** out_next);                        \
END_DERIVED_FUNCTIONS(List_##type_name)


/* ===== Instantiate a typed list: int ===== */
DECLARE_SPECIFIC_LIST_TYPE(int, int)

#endif /* __COOP__LIST__H_ */
