#ifndef __COOP__LIST__H_
#define __COOP__LIST__H_

#include "COOP.h"
#include "Iterator.h"
#include "SharedObjPtr.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* ===== Node ===== */
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
END_DEF(GenericList);

/* ===== Ctors/Methods Declarations ===== */
FUNCTIONS(GenericList, MEM_SIZE_T dataTypeSize);

/* ---- Generic (untyped, uses _this->elementSize) ---- */
MEM_FUN_DECL(GenericList, __push_back_generic, const void* src);
MEM_FUN_DECL(GenericList, __push_front_generic, const void* src);
MEM_FUN_DECL(GenericList, __pop_back_generic, void* out_elem);
MEM_FUN_DECL(GenericList, __pop_front_generic, void* out_elem);
MEM_FUN_DECL(GenericList, __front_generic, void* out_elem);
MEM_FUN_DECL(GenericList, __back_generic, void* out_elem);

MEM_FUN_DECL(GenericList, __make_node, const void* src_bytes, ListNode** out_node);

MEM_FUN_DECL(GenericList, size, MEM_SIZE_T* out_size);
MEM_FUN_DECL(GenericList, empty, bool* out_is_empty);
MEM_FUN_DECL(GenericList, clear); 

MEM_FUN_DECL(GenericList, begin, Iterator** out_it);
MEM_FUN_DECL(GenericList, end, Iterator** out_it);
MEM_FUN_DECL(GenericList, it_destroy, Iterator* it);

END_FUNCTIONS(GenericList);

/* ===== Iterator derived from Iterator =====*/
DEF_DERIVED_CLASS(ListIter, Iterator);
GenericList* list;
ListNode* node;
END_DEF_DERIVED(ListIter);

/* ===== ListIter overrides ===== */
DERIVED_FUNCTIONS(ListIter, Iterator);
FUN_OVERRIDE(Iterator, equals, object* other, bool* out_equal);
FUN_OVERRIDE(Iterator, next);
FUN_OVERRIDE(Iterator, prev);
FUN_OVERRIDE(Iterator, get_ref, void** out_ptr);
FUN_OVERRIDE(Iterator, get_cref, const void** out_ptr);
FUN_OVERRIDE(Iterator, distance, object* other, ptrdiff_t* out_dist);
FUN_OVERRIDE(Iterator, advance, ptrdiff_t n);
END_DERIVED_FUNCTIONS(ListIter);

/* ===== Typed facade ===== */
#define DECLARE_SPECIFIC_LIST_TYPE(type)                                   \
DEF_DERIVED_CLASS(List_ ##type, GenericList);                              \
END_DEF_DERIVED(List_ ##type);                                             \
DERIVED_FUNCTIONS(List_ ##type, GenericList);                              \
MEM_FUN_DECL(List_ ##type, push_back,     type val);                       \
MEM_FUN_DECL(List_ ##type, push_front,    type val);                       \
MEM_FUN_DECL(List_ ##type, pop_back,      type* out_val);                  \
MEM_FUN_DECL(List_ ##type, pop_front,     type* out_val);                  \
MEM_FUN_DECL(List_ ##type, front,         type* out_val);                  \
MEM_FUN_DECL(List_ ##type, back,          type* out_val);                  \
MEM_FUN_DECL(List_ ##type, begin,         Iterator** out_it);              \
MEM_FUN_DECL(List_ ##type, end,           Iterator** out_it);              \
MEM_FUN_DECL(List_ ##type, it_destroy,    Iterator* it);                   \
END_DERIVED_FUNCTIONS(List_ ##type)


DECLARE_SPECIFIC_LIST_TYPE(int);

#endif 
