#ifndef __COOP__LIST__H_
#define __COOP__LIST__H_

#include "COOP.h"
#include "Iterator.h"
#include "SharedObjPtr.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* ===== Element type enum for printing/formatting ===== */
typedef enum {
    LIST_ELEM_INT,
    LIST_ELEM_FLOAT,
    LIST_ELEM_CHAR,
    LIST_ELEM_OBJ_SPTR,
    LIST_ELEM_RAW_BYTES   /* default: hex dump of elementSize bytes */
} List_ElementType;


/* ===== Node ===== */
DEF_CLASS(ListNode);
ListNode* prev;
ListNode* next;
unsigned char payload[];   /* element bytes (length = elementSize) */
END_DEF(ListNode);

/* ===== Iterator derived from Iterator =====*/
DEF_DERIVED_CLASS(ListIter, Iterator);
ListNode* node;
END_DEF_DERIVED(ListIter);

/* ===== GenericList (homogeneous by elementSize) ===== */
DEF_CLASS(GenericList);
MEM_SIZE_T size;           /* number of elements */
MEM_SIZE_T elementSize;    /* size of each element in bytes */
ListNode* head;            /* first node */
ListNode* tail;            /* last node */
List_ElementType elem_type;
END_DEF(GenericList);

/* ===== Ctors/Methods Declarations ===== */
FUNCTIONS(GenericList, MEM_SIZE_T dataTypeSize, List_ElementType enumTag);

/* ---- Generic API ---- */
MEM_FUN_DECL(GenericList, __push_back_generic, const char* buff, MEM_SIZE_T buff_size);
MEM_FUN_DECL(GenericList, __push_front_generic, const char* buff, MEM_SIZE_T buff_size);
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
/* printing */
MEM_FUN_DECL(GenericList, __print_value, const void* p); /* helper for single element */
MEM_FUN_DECL(GenericList, print);                        /* print entire list using iterators */

/* ---- Typed wrappers ON BASE (int) ---- */
MEM_FUN_DECL(GenericList, push_back_int, const int val);
MEM_FUN_DECL(GenericList, push_front_int, const int val);
MEM_FUN_DECL(GenericList, pop_back_int, int* out_val);
MEM_FUN_DECL(GenericList, pop_front_int, int* out_val);
MEM_FUN_DECL(GenericList, front_int, int* out_val);
MEM_FUN_DECL(GenericList, back_int, int* out_val);

/* ---- Typed wrappers ON BASE (char) ---- */
MEM_FUN_DECL(GenericList, push_back_char, const char val);
MEM_FUN_DECL(GenericList, push_front_char, const char val);
MEM_FUN_DECL(GenericList, pop_back_char, char* out_val);
MEM_FUN_DECL(GenericList, pop_front_char, char* out_val);
MEM_FUN_DECL(GenericList, front_char, char* out_val);
MEM_FUN_DECL(GenericList, back_char, char* out_val);

/* ---- Typed wrappers ON BASE (float) ---- */
MEM_FUN_DECL(GenericList, push_back_float, const float val);
MEM_FUN_DECL(GenericList, push_front_float, const float val);
MEM_FUN_DECL(GenericList, pop_back_float, float* out_val);
MEM_FUN_DECL(GenericList, pop_front_float, float* out_val);
MEM_FUN_DECL(GenericList, front_float, float* out_val);
MEM_FUN_DECL(GenericList, back_float, float* out_val);

/* ---- Typed wrappers ON BASE (objSPtr) ---- */
MEM_FUN_DECL(GenericList, push_back_objSPtr, const objSPtr val);
MEM_FUN_DECL(GenericList, push_front_objSPtr, const objSPtr val);
MEM_FUN_DECL(GenericList, pop_back_objSPtr, objSPtr* out_val);
MEM_FUN_DECL(GenericList, pop_front_objSPtr, objSPtr* out_val);
MEM_FUN_DECL(GenericList, front_objSPtr, objSPtr* out_val);
MEM_FUN_DECL(GenericList, back_objSPtr, objSPtr* out_val);

/* ---- Iterator support ---- */
MEM_FUN_DECL(GenericList, begin, Iterator** out_iter);
MEM_FUN_DECL(GenericList, end, Iterator** out_iter);


END_FUNCTIONS(GenericList);

/* ===== ListNode  ===== */
FUNCTIONS(ListNode, MEM_SIZE_T element_size, const void* src_bytes);
END_FUNCTIONS(ListNode);

/* ===== ListIter overrides ===== */
DERIVED_FUNCTIONS(ListIter, Iterator,IteratorCategory category, void* container_ptr);
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
MEM_FUN_DECL(List_ ##type, print);	                                       \
MEM_FUN_DECL(List_ ##type, begin,         Iterator **out_iter);            \
MEM_FUN_DECL(List_ ##type, end,           Iterator **out_iter);            \
END_DERIVED_FUNCTIONS(List_ ##type)


DECLARE_SPECIFIC_LIST_TYPE(int);
DECLARE_SPECIFIC_LIST_TYPE(float);
DECLARE_SPECIFIC_LIST_TYPE(char);
DECLARE_SPECIFIC_LIST_TYPE(objSPtr);

#endif

//add, sort
