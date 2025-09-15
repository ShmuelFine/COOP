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

/* ---- Generic API (Vector-style: char* + size) ---- */
MEM_FUN_DECL(GenericList, __push_back_generic, char* buff, MEM_SIZE_T buff_size);
MEM_FUN_DECL(GenericList, __push_front_generic, char* buff, MEM_SIZE_T buff_size);
MEM_FUN_DECL(GenericList, __pop_back_generic, char* buff, MEM_SIZE_T buff_size);
MEM_FUN_DECL(GenericList, __pop_front_generic, char* buff, MEM_SIZE_T buff_size);
MEM_FUN_DECL(GenericList, __front_generic, char* buff, MEM_SIZE_T buff_size);
MEM_FUN_DECL(GenericList, __back_generic, char* buff, MEM_SIZE_T buff_size);

/* helper */
MEM_FUN_DECL(GenericList, __make_node, const char* src_bytes, MEM_SIZE_T buff_size, ListNode** out_node);

/* services */
MEM_FUN_DECL(GenericList, size, MEM_SIZE_T* out_size);
MEM_FUN_DECL(GenericList, empty, bool* out_is_empty);
MEM_FUN_DECL(GenericList, clear);

/* Iteration API  Vector */
MEM_FUN_DECL(GenericList, begin, Iterator** out_it);
MEM_FUN_DECL(GenericList, end, Iterator** out_it);
MEM_FUN_DECL(GenericList, it_destroy, Iterator* it);

/* ---- Typed wrappers ON BASE (??? ?-Vector: push_back_int ???') ---- */
MEM_FUN_DECL(GenericList, push_back_int, int val);
MEM_FUN_DECL(GenericList, push_back_char, char val);
MEM_FUN_DECL(GenericList, push_back_float, float val);
MEM_FUN_DECL(GenericList, push_back_objSPtr, objSPtr val);

MEM_FUN_DECL(GenericList, push_front_int, int val);
MEM_FUN_DECL(GenericList, push_front_char, char val);
MEM_FUN_DECL(GenericList, push_front_float, float val);
MEM_FUN_DECL(GenericList, push_front_objSPtr, objSPtr val);

MEM_FUN_DECL(GenericList, pop_back_int, int* out_val);
MEM_FUN_DECL(GenericList, pop_back_char, char* out_val);
MEM_FUN_DECL(GenericList, pop_back_float, float* out_val);
MEM_FUN_DECL(GenericList, pop_back_objSPtr, objSPtr* out_val);

MEM_FUN_DECL(GenericList, pop_front_int, int* out_val);
MEM_FUN_DECL(GenericList, pop_front_char, char* out_val);
MEM_FUN_DECL(GenericList, pop_front_float, float* out_val);
MEM_FUN_DECL(GenericList, pop_front_objSPtr, objSPtr* out_val);

MEM_FUN_DECL(GenericList, front_int, int* out_val);
MEM_FUN_DECL(GenericList, front_char, char* out_val);
MEM_FUN_DECL(GenericList, front_float, float* out_val);
MEM_FUN_DECL(GenericList, front_objSPtr, objSPtr* out_val);

MEM_FUN_DECL(GenericList, back_int, int* out_val);
MEM_FUN_DECL(GenericList, back_char, char* out_val);
MEM_FUN_DECL(GenericList, back_float, float* out_val);
MEM_FUN_DECL(GenericList, back_objSPtr, objSPtr* out_val);

END_FUNCTIONS(GenericList);

/* ===== Iterator derived from Iterator =====*/
DEF_DERIVED_CLASS(ListIter, Iterator);
GenericList* list;
ListNode* node;
END_DEF_DERIVED(ListIter);

/* ===== ListIter overrides ===== */
DERIVED_FUNCTIONS(ListIter, Iterator);
FUN_OVERRIDE(Iterator, equals, Iterator* other, bool* out_equal);
FUN_OVERRIDE(Iterator, next);
FUN_OVERRIDE(Iterator, prev);
FUN_OVERRIDE(Iterator, get_ref, void** out_ptr);
FUN_OVERRIDE(Iterator, get_cref, const void** out_ptr);
FUN_OVERRIDE(Iterator, distance, Iterator* other, ptrdiff_t* out_dist);
FUN_OVERRIDE(Iterator, advance, ptrdiff_t n);
END_DERIVED_FUNCTIONS(ListIter);

/* ===== Typed facade  ===== */
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
MEM_FUN_DECL(List_ ##type, size,          MEM_SIZE_T* out_size);           \
MEM_FUN_DECL(List_ ##type, empty,         bool* out_is_empty);             \
MEM_FUN_DECL(List_ ##type, clear);                                         \
MEM_FUN_DECL(List_ ##type, begin,         Iterator** out_it);              \
MEM_FUN_DECL(List_ ##type, end,           Iterator** out_it);              \
MEM_FUN_DECL(List_ ##type, it_destroy,    Iterator* it);                   \
END_DERIVED_FUNCTIONS(List_ ##type)

DECLARE_SPECIFIC_LIST_TYPE(int);
DECLARE_SPECIFIC_LIST_TYPE(char);
DECLARE_SPECIFIC_LIST_TYPE(float);
DECLARE_SPECIFIC_LIST_TYPE(objSPtr);

#endif

//add print, sort