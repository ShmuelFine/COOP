#include "Queue.h"
#include "MathUtils.h"
#include <string.h>

/* =============================
 *     QueueIter (Iterator)
 * ============================= */
DEF_DERIVED_CTOR(QueueIter, Iterator) SUPER, (ITER_FORWARD)ME
{
    _this->list = NULL;
    _this->node = NULL;
}
END_DERIVED_CTOR

DEF_DERIVED_DTOR(QueueIter, Iterator) { } END_DERIVED_DTOR

FUN_OVERRIDE_IMPL(QueueIter, Iterator, equals, Iterator* other, bool* out_equal)
{
    *out_equal = 0;
    IF(other) {
        QueueIter* o = (QueueIter*)other;
        *out_equal = (_this->list == o->list) && (_this->node == o->node);
    } END_IF
}
END_FUN;

FUN_OVERRIDE_IMPL(QueueIter, Iterator, next)
{
    THROW_MSG_UNLESS(_this->list != NULL, "Iterator not bound");
    if (_this->node) _this->node = _this->node->next;
}
END_FUN;


FUN_OVERRIDE_IMPL(QueueIter, Iterator, get_ref, void** out_ptr)
{
    THROW_MSG_UNLESS(_this->list != NULL, "Iterator not bound");
    IF(_this->node == NULL) {
        *out_ptr = NULL; /* end() */
    } ELSE{
        *out_ptr = (void*)_this->node->payload;
    } END_IF
}
END_FUN;

FUN_OVERRIDE_IMPL(QueueIter, Iterator, get_cref, const void** out_ptr)
{
    void* p = NULL;
    MFUN(_this, get_ref), & p CALL;
    *out_ptr = (const void*)p;
}
END_FUN;

FUN_OVERRIDE_IMPL(QueueIter, Iterator, distance, Iterator* other, ptrdiff_t* out_dist)
{
    THROW_MSG_UNLESS(other != NULL, "Null other iterator");
    QueueIter* o = (QueueIter*)other;
    THROW_MSG_UNLESS(o->list == _this->list, "Iterators of different queues");

    ptrdiff_t d = 0;
    ListNode* it = _this->node;
    while (it && it != o->node) {
        it = it->next;
        ++d;
    }
    THROW_MSG_UNLESS(it == o->node, "Iterators not reachable (Queue is forward-only)");
    *out_dist = d;
}
END_FUN;

FUN_OVERRIDE_IMPL(QueueIter, Iterator, advance, ptrdiff_t n)
{
    THROW_MSG_UNLESS(_this->list != NULL, "Iterator not bound");
    THROW_MSG_UNLESS(n >= 0, "QueueIter advance() only supports non-negative n");
    ListNode* it = _this->node;
    while (n-- > 0 && it) it = it->next;
    _this->node = it;
}
END_FUN;

FUN_OVERRIDE_IMPL(QueueIter, Iterator, reset_begin)
{
    THROW_MSG_UNLESS(_this->list != NULL, "Iterator not bound");
    _this->node = _this->list ? _this->list->head : NULL;
}
END_FUN;

INIT_DERIVED_CLASS(QueueIter, Iterator);
BIND_OVERIDE(QueueIter, Iterator, equals);
BIND_OVERIDE(QueueIter, Iterator, next);
BIND_OVERIDE(QueueIter, Iterator, get_ref);
BIND_OVERIDE(QueueIter, Iterator, get_cref);
BIND_OVERIDE(QueueIter, Iterator, distance);
BIND_OVERIDE(QueueIter, Iterator, advance);
BIND_OVERIDE(QueueIter, Iterator, reset_begin);
END_INIT_CLASS(QueueIter)


/* =============================
 *        GenericQueue
 * ============================= */

DEF_CTOR(GenericQueue, MEM_SIZE_T elementSize, ElementType elem_type)
{
    CREATE(QueueIter, b) CALL;
    CREATE(QueueIter, e) CALL;
    _this->begin_iter = b;
    _this->end_iter = e;

    _this->begin_iter.list = &_this->list;
    _this->begin_iter.node = _this->list.head; /* head or NULL */
    _this->end_iter.list = &_this->list;
    _this->end_iter.node = NULL; /* end() is NULL */

	_this->elementSize = elementSize;
	_this->elem_type = elem_type;
    DESTROY(&b);
    DESTROY(&e);
}
END_CTOR

DEF_DTOR(GenericQueue)
{   
    MFUN(&_this->list, clear) CALL;
    _this->begin_iter.list = NULL;  
    _this->begin_iter.node = NULL;
    _this->end_iter.list = NULL;
    _this->end_iter.node = NULL;
}
END_DTOR

/* ==== Core ops map directly to List ==== */
MEM_FUN_IMPL(GenericQueue, enqueue_generic, const char* buff, MEM_SIZE_T buff_size)
{
    MFUN(&_this->list, __push_back_generic), (char*)buff, buff_size CALL;
    /* update begin/end iters */
    _this->begin_iter.node = _this->list.head;
    _this->end_iter.node = NULL;
}
END_FUN;

MEM_FUN_IMPL(GenericQueue, dequeue_generic, char* out_buff, MEM_SIZE_T buff_size)
{
    MFUN(&_this->list, __pop_front_generic), out_buff, buff_size CALL;
    _this->begin_iter.node = _this->list.head;
    _this->end_iter.node = NULL;
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
    _this->begin_iter.node = NULL;
    _this->end_iter.node = NULL;
}
END_FUN;

MEM_FUN_IMPL(GenericQueue, size, MEM_SIZE_T* out_size)
{
    MFUN(&_this->list, size), out_size CALL;
}
END_FUN;

MEM_FUN_IMPL(GenericQueue, empty, bool* out_empty)
{
    MFUN(&_this->list, empty), out_empty CALL;
}
END_FUN;
MEM_FUN_IMPL(GenericQueue, print)
{
	Iterator* it = (Iterator*) & _this->begin_iter;
	Iterator* end_it = (Iterator*) & _this->end_iter;
	bool eq = false;
	MFUN(it, equals), end_it, &eq CALL;
	WHILE (!eq) {
		const void* val = NULL;
		MFUN(it, get_cref), & val CALL;
		MFUN(_this, __print), val CALL;
		MFUN(it, next) CALL;
		MFUN(it, equals), end_it, & eq CALL;
	}END_LOOP
	printf("\n");
}END_FUN

MEM_FUN_IMPL(GenericQueue, __print, const void* val)
{
    IF(_this->elem_type == INT)
    {
        printf("%d ", *(const int*)val);
    }
    ELSE_IF(_this->elem_type == FLOAT)
    {
        printf("%f ", *(const float*)val);
    }
    ELSE_IF(_this->elem_type == CHAR)
    {
        printf("%c ", *(const char*)val);
    }
    ELSE_IF(_this->elem_type == OBJ_SPTR)
    {
        MFUN((objSPtr*)val, print) CALL;
        printf(" ");
    }
    ELSE /* RAW_BYTES */
    {
        const unsigned char* bytes = (const unsigned char*)val;
        FOR(MEM_SIZE_T i = 0; i < _this->elementSize; i++)
        {
            printf("%02X ", bytes[i]);  
        }
        END_LOOP;
        printf(" ");
    }
    END_IF;
}
END_FUN

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

#define IMPL_SPECIFIC_QUEUE_TYPE_xTORs(type,enum_type)                                \
DEF_DERIVED_CTOR(Queue_##type, GenericQueue) SUPER,sizeof(type),enum_type ME                       \
{                                                                           \
    GenericQueue* base=(GenericQueue*)_this; \
    CREATE(List_##type, list_tmp)  CALL;                                           \
    base->list = *((GenericList*)&list_tmp);                                   \
    DESTROY(&list_tmp);                                                         \
} END_DERIVED_CTOR                                                          \
DEF_DERIVED_DTOR(Queue_##type, GenericQueue)                                \
{                                                                           \
    FUN_BASE(_this, clear) CALL;                                            \
} END_DERIVED_DTOR


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
MEM_FUN_IMPL(Queue_##type, size, MEM_SIZE_T* out_size)                       \
{                                                                            \
    FUN_BASE(_this, size), out_size CALL;                                    \
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
BIND(Queue_##type, empty);                                                   \
BIND(Queue_##type, print);                                                   \
END_INIT_CLASS(Queue_##type)

IMPL_SPECIFIC_QUEUE_TYPE_xTORs(int,INT);
IMPL_SPECIFIC_QUEUE_TYPE_FUNCTIONS(int);

IMPL_SPECIFIC_QUEUE_TYPE_xTORs(char,CHAR);
IMPL_SPECIFIC_QUEUE_TYPE_FUNCTIONS(char);

IMPL_SPECIFIC_QUEUE_TYPE_xTORs(float,FLOAT);
IMPL_SPECIFIC_QUEUE_TYPE_FUNCTIONS(float);

IMPL_SPECIFIC_QUEUE_TYPE_xTORs(objSPtr,OBJ_SPTR);
IMPL_SPECIFIC_QUEUE_TYPE_FUNCTIONS(objSPtr);

