#include "Queue.h"
#include "MathUtils.h"
#include <string.h>

/* =============================
 *        GenericQueue
 * ============================= */
DEF_CTOR(GenericQueue)
{
}
END_CTOR

DEF_DTOR(GenericQueue)
{
    DESTROY(&(_this->list));
}
END_DTOR

/* ==== Core ops map directly to List ==== */
MEM_FUN_IMPL(GenericQueue, enqueue_generic, const char* buff, MEM_SIZE_T buff_size)
{
    MFUN(&_this->list, __push_back_generic), (char*)buff, buff_size CALL;
}
END_FUN;

MEM_FUN_IMPL(GenericQueue, dequeue_generic, char* out_buff, MEM_SIZE_T buff_size)
{
    MFUN(&_this->list, __pop_front_generic), out_buff, buff_size CALL;
}
END_FUN;

MEM_FUN_IMPL(GenericQueue, front_generic, char** out_ptr)
{
    THROW_MSG_UNLESS(out_ptr, "Output pointer is NULL");
    THROW_MSG_UNLESS(_this->list.head, "Queue is empty");
    *out_ptr = (char*)_this->list.head->payload;
}
END_FUN;

MEM_FUN_IMPL(GenericQueue, front_generic_cref, const char** out_ptr)
{
    char* p = NULL;
    MFUN(_this, front_generic), & p CALL;
    *out_ptr = (const char*)p;
}
END_FUN;

MEM_FUN_IMPL(GenericQueue, clear)
{
    MFUN(&_this->list, clear) CALL;
}
END_FUN;

MEM_FUN_IMPL(GenericQueue, size, int* out_size)
{
    MFUN(&_this->list, size), out_size CALL;
}
END_FUN;
MEM_FUN_IMPL(GenericQueue, begin, Iterator** out_it)
{
    MFUN(&_this->list, begin), out_it CALL;
}END_FUN;

MEM_FUN_IMPL(GenericQueue, end, Iterator** out_it)
{
    MFUN(&_this->list, end), out_it CALL;
}END_FUN;

MEM_FUN_IMPL(GenericQueue, empty, bool* out_empty)
{
    MFUN(&_this->list, empty), out_empty CALL;
}
END_FUN;

MEM_FUN_IMPL(GenericQueue, print)
{
    MFUN(&_this->list, print) CALL;
}END_FUN


/* ===== Typed wrappers ===== */
#define IMPL_ENQUEUE_OF_TYPE(type)                                         \
MEM_FUN_IMPL(GenericQueue, enqueue_##type, type val) {                     \
    MFUN(_this, enqueue_generic), (const char*)&val, (MEM_SIZE_T)sizeof(type) CALL; \
} END_FUN;

#define IMPL_DEQUEUE_OF_TYPE(type)                                         \
MEM_FUN_IMPL(GenericQueue, dequeue_##type, type* out_val) {                \
    MFUN(_this, dequeue_generic), (char*)out_val, (MEM_SIZE_T)sizeof(type) CALL;     \
} END_FUN;

#define IMPL_FRONT_OF_TYPE(type)                                           \
MEM_FUN_IMPL(GenericQueue, front_##type, type** out_ptr) {                 \
    MFUN(_this, front_generic), (char**)out_ptr CALL;                      \
} END_FUN;                                                                  \
MEM_FUN_IMPL(GenericQueue, front_##type##_cref, const type** out_ptr) {    \
    MFUN(_this, front_generic_cref), (const char**)out_ptr CALL;           \
} END_FUN;

IMPL_ENQUEUE_OF_TYPE(int)
IMPL_ENQUEUE_OF_TYPE(char)
IMPL_ENQUEUE_OF_TYPE(float)
IMPL_ENQUEUE_OF_TYPE(objSPtr)

IMPL_DEQUEUE_OF_TYPE(int)
IMPL_DEQUEUE_OF_TYPE(char)
IMPL_DEQUEUE_OF_TYPE(float)
IMPL_DEQUEUE_OF_TYPE(objSPtr)

IMPL_FRONT_OF_TYPE(int)
IMPL_FRONT_OF_TYPE(char)
IMPL_FRONT_OF_TYPE(float)
IMPL_FRONT_OF_TYPE(objSPtr)


INIT_CLASS(GenericQueue)
BIND(GenericQueue, enqueue_generic);
BIND(GenericQueue, dequeue_generic);
BIND(GenericQueue, front_generic);
BIND(GenericQueue, front_generic_cref);
BIND(GenericQueue, clear);
BIND(GenericQueue, size);
BIND(GenericQueue, begin);
BIND(GenericQueue, end);
BIND(GenericQueue, empty);
BIND(GenericQueue, print);

BIND(GenericQueue, enqueue_int);
BIND(GenericQueue, enqueue_char);
BIND(GenericQueue, enqueue_float);
BIND(GenericQueue, enqueue_objSPtr);

BIND(GenericQueue, dequeue_int);
BIND(GenericQueue, dequeue_char);
BIND(GenericQueue, dequeue_float);
BIND(GenericQueue, dequeue_objSPtr);

BIND(GenericQueue, front_int);
BIND(GenericQueue, front_char);
BIND(GenericQueue, front_float);
BIND(GenericQueue, front_objSPtr);

BIND(GenericQueue, front_int_cref);
BIND(GenericQueue, front_char_cref);
BIND(GenericQueue, front_float_cref);
BIND(GenericQueue, front_objSPtr_cref);


END_INIT_CLASS(GenericQueue)

/* =============================
 *     Specific Queue types
 * ============================= */

#define IMPL_SPECIFIC_QUEUE_TYPE_xTORs(type)                                \
DEF_DERIVED_CTOR(Queue_##type, GenericQueue) SUPER ME                       \
{                                                                           \
    GenericQueue* base=(GenericQueue*)_this; \
    CREATE(List_##type, list_tmp)  CALL;                                           \
    base->list = *((GenericList*)&list_tmp);                                   \
} END_DERIVED_CTOR                                                          \
DEF_DERIVED_DTOR(Queue_##type, GenericQueue){ } END_DERIVED_DTOR


#define IMPL_SPECIFIC_QUEUE_TYPE_FUNCTIONS(type)                             \
MEM_FUN_IMPL(Queue_##type, enqueue, type val)                                \
{                                                                            \
    FUN_BASE(_this, enqueue_##type), val CALL;                               \
} END_FUN;                                                                   \
MEM_FUN_IMPL(Queue_##type, dequeue, type* out_val)                           \
{                                                                            \
    FUN_BASE(_this, dequeue_##type), out_val CALL;                           \
} END_FUN;                                                                   \
MEM_FUN_IMPL(Queue_##type, front, type** out_ptr)                            \
{                                                                            \
    FUN_BASE(_this, front_##type), out_ptr CALL;                             \
} END_FUN;                                                                   \
MEM_FUN_IMPL(Queue_##type, front_cref, const type** out_ptr)                 \
{                                                                            \
    FUN_BASE(_this, front_##type##_cref), out_ptr CALL;                      \
} END_FUN;                                                                   \
MEM_FUN_IMPL(Queue_##type, clear)                                            \
{                                                                            \
    FUN_BASE(_this, clear) CALL;                                             \
} END_FUN;                                                                   \
MEM_FUN_IMPL(Queue_##type, size, int* out_size)                       \
{                                                                            \
    FUN_BASE(_this, size), out_size CALL;                                    \
} END_FUN;                                                                   \
MEM_FUN_IMPL(Queue_##type, begin, Iterator** out_it)                         \
{                                                                            \
	FUN_BASE(_this, begin), out_it CALL;                                     \
} END_FUN;                                                                   \
MEM_FUN_IMPL(Queue_##type, end, Iterator** out_it)                           \
{                                                                            \
	FUN_BASE(_this, end), out_it CALL;                                       \
} END_FUN;                                                                   \
MEM_FUN_IMPL(Queue_##type, empty, bool* out_empty)                           \
{                                                                            \
    FUN_BASE(_this, empty), out_empty CALL;                                  \
} END_FUN;                                                                   \
MEM_FUN_IMPL(Queue_##type, print)                                            \
{                                                                            \
	FUN_BASE(_this, print) CALL;                                            \
} END_FUN;                                                                   \
INIT_DERIVED_CLASS(Queue_##type, GenericQueue);                              \
BIND(Queue_##type, enqueue);                                                 \
BIND(Queue_##type, dequeue);                                                 \
BIND(Queue_##type, front);                                                   \
BIND(Queue_##type, front_cref);                                              \
BIND(Queue_##type, clear);                                                   \
BIND(Queue_##type, size);                                                    \
BIND(Queue_##type, begin);                                                   \
BIND(Queue_##type, end);                                                     \
BIND(Queue_##type, empty);                                                   \
BIND(Queue_##type, print);                                                   \
END_INIT_CLASS(Queue_##type)

    IMPL_SPECIFIC_QUEUE_TYPE_xTORs(int);
IMPL_SPECIFIC_QUEUE_TYPE_FUNCTIONS(int);

IMPL_SPECIFIC_QUEUE_TYPE_xTORs(char);
IMPL_SPECIFIC_QUEUE_TYPE_FUNCTIONS(char);

IMPL_SPECIFIC_QUEUE_TYPE_xTORs(float);
IMPL_SPECIFIC_QUEUE_TYPE_FUNCTIONS(float);

IMPL_SPECIFIC_QUEUE_TYPE_xTORs(objSPtr);
IMPL_SPECIFIC_QUEUE_TYPE_FUNCTIONS(objSPtr);