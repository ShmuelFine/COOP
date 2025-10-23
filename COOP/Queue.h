#ifndef __COOP__QUEUE__H_
#define __COOP__QUEUE__H_

#include "COOP.h"
#include "Iterator.h"
#include "SharedObjPtr.h"
#include "List.h"   
#include <stdbool.h>
#include <stddef.h>
#include "SharedObjPtr.h"

/* =============================
 *        Generic Queue
 * ============================= */
DEF_CLASS(GenericQueue);
GenericList list;
END_DEF(GenericQueue);

FUNCTIONS(GenericQueue);

/* basic ops (Vector-style generic + typed wrappers) */
MEM_FUN_DECL(GenericQueue, enqueue_generic, const char* buff, MEM_SIZE_T buff_size);
MEM_FUN_DECL(GenericQueue, dequeue_generic, char* out_buff, MEM_SIZE_T buff_size);
MEM_FUN_DECL(GenericQueue, front_generic, char** out_ptr);
MEM_FUN_DECL(GenericQueue, front_generic_cref, const char** out_ptr);
MEM_FUN_DECL(GenericQueue, clear);
MEM_FUN_DECL(GenericQueue, size, int* out_size);
MEM_FUN_DECL(GenericQueue, begin, Iterator** out_it);
MEM_FUN_DECL(GenericQueue, end, Iterator** out_it);
MEM_FUN_DECL(GenericQueue, empty, bool* out_empty);
MEM_FUN_DECL(GenericQueue, print);

/* ====== Typed helpers ====== */
MEM_FUN_DECL(GenericQueue, enqueue_int, int val);
MEM_FUN_DECL(GenericQueue, enqueue_char, char val);
MEM_FUN_DECL(GenericQueue, enqueue_float, float val);
MEM_FUN_DECL(GenericQueue, enqueue_objSPtr, objSPtr val);

MEM_FUN_DECL(GenericQueue, dequeue_int, int* out_val);
MEM_FUN_DECL(GenericQueue, dequeue_char, char* out_val);
MEM_FUN_DECL(GenericQueue, dequeue_float, float* out_val);
MEM_FUN_DECL(GenericQueue, dequeue_objSPtr, objSPtr* out_val);

MEM_FUN_DECL(GenericQueue, front_int, int** out_ptr);
MEM_FUN_DECL(GenericQueue, front_char, char** out_ptr);
MEM_FUN_DECL(GenericQueue, front_float, float** out_ptr);
MEM_FUN_DECL(GenericQueue, front_objSPtr, objSPtr** out_ptr);

MEM_FUN_DECL(GenericQueue, front_int_cref, const int** out_ptr);
MEM_FUN_DECL(GenericQueue, front_char_cref, const char** out_ptr);
MEM_FUN_DECL(GenericQueue, front_float_cref, const float** out_ptr);
MEM_FUN_DECL(GenericQueue, front_objSPtr_cref, const objSPtr** out_ptr);

END_FUNCTIONS(GenericQueue);

/* =============================
 *     Specific Queue types
 * ============================= */
#define DECLARE_SPECIFIC_QUEUE_TYPE(type)                                   \
DEF_DERIVED_CLASS(Queue_##type, GenericQueue);                              \
END_DEF_DERIVED(Queue_##type);                                              \
DERIVED_FUNCTIONS(Queue_##type, GenericQueue);                              \
MEM_FUN_DECL(Queue_##type, enqueue, type val);                              \
MEM_FUN_DECL(Queue_##type, dequeue, type* out_val);                         \
MEM_FUN_DECL(Queue_##type, front, type** out_ptr);                          \
MEM_FUN_DECL(Queue_##type, front_cref, const type** out_ptr);               \
MEM_FUN_DECL(Queue_##type, clear);                                          \
MEM_FUN_DECL(Queue_##type, size, int* out_size);                     \
MEM_FUN_DECL(Queue_ ##type, begin, Iterator **out_it);                     \
MEM_FUN_DECL(Queue_ ##type, end, Iterator **out_it);                        \
MEM_FUN_DECL(Queue_##type, empty, bool* out_empty);                         \
MEM_FUN_DECL(Queue_##type, print);                                         \
END_DERIVED_FUNCTIONS(Queue_##type);

DECLARE_SPECIFIC_QUEUE_TYPE(int);
DECLARE_SPECIFIC_QUEUE_TYPE(char);
DECLARE_SPECIFIC_QUEUE_TYPE(float);
DECLARE_SPECIFIC_QUEUE_TYPE(objSPtr);

#endif 