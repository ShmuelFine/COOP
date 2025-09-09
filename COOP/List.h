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

/* ===== GenericList (homogeneous by elementSize) ===== */
DEF_CLASS(GenericList);
MEM_SIZE_T size;           /* number of elements */
MEM_SIZE_T elementSize;    /* size of each element in bytes */
ListNode* head;            /* first node */
ListNode* tail;            /* last node */
List_ElementType elem_type;
//ListIter begin_iter;
//ListIter end_iter;
END_DEF(GenericList);

/* ===== Iterator derived from Iterator =====*/
DEF_DERIVED_CLASS(ListIter, Iterator);
GenericList* list;
ListNode* node;
END_DEF_DERIVED(ListIter);

/* ===== Ctors/Methods Declarations ===== */
FUNCTIONS(GenericList, MEM_SIZE_T dataTypeSize, List_ElementType enumTag);

/* ---- Generic API ---- */
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

/* printing */
MEM_FUN_DECL(GenericList, __print_value, const void* p); /* helper for single element */
MEM_FUN_DECL(GenericList, print);                        /* print entire list using iterators */


/* Iteration API */
MEM_FUN_DECL(GenericList, begin, Iterator** out_it);
MEM_FUN_DECL(GenericList, end, Iterator** out_it);
MEM_FUN_DECL(GenericList, it_destroy, Iterator* it);

/* ---- Typed wrappers ON BASE (int) ---- */
MEM_FUN_DECL(GenericList, push_back_int, int val);
MEM_FUN_DECL(GenericList, push_front_int, int val);
MEM_FUN_DECL(GenericList, pop_back_int, int* out_val);
MEM_FUN_DECL(GenericList, pop_front_int, int* out_val);
MEM_FUN_DECL(GenericList, front_int, int* out_val);
MEM_FUN_DECL(GenericList, back_int, int* out_val);

END_FUNCTIONS(GenericList);

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
MEM_FUN_DECL(List_ ##type, print);	\
END_DERIVED_FUNCTIONS(List_ ##type)
DECLARE_SPECIFIC_LIST_TYPE(int);

#endif

//add, sort
