#include "List.h"
#include "COOP.h"
#include <string.h>

/* ======================== CTOR / DTOR ======================== */

DEF_CTOR(GenericList, MEM_SIZE_T dataTypeSize)
{
    _this->size = 0;
    _this->elementSize = dataTypeSize;
    _this->head = NULL;
    _this->tail = NULL;
}
END_CTOR

DEF_DTOR(GenericList)
{
    ListNode* cur = _this->head;

    WHILE(cur != NULL)
        ListNode* next = cur->next;
    FREE(cur);
    cur = next;
    END_LOOP;

    _this->head = NULL;
    _this->tail = NULL;
    _this->size = 0;
}
END_DTOR

/* ======================== Helpers ======================== */

MEM_FUN_IMPL(GenericList, __make_node, const char* src_bytes, MEM_SIZE_T buff_size, ListNode** out_node)
{
    THROW_MSG_UNLESS(out_node, "out_node must not be NULL");
    IF(src_bytes != NULL)
        THROW_MSG_UNLESS(buff_size == _this->elementSize, "Invalid Data Size");
    END_IF;

    MEM_SIZE_T bytes = (MEM_SIZE_T)(sizeof(ListNode) + _this->elementSize);

    ListNode* nd = NULL;
    ALLOC_ARRAY(nd, char, bytes);

    nd->prev = NULL;
    nd->next = NULL;

    IF(src_bytes != NULL)
        memcpy(nd->payload, src_bytes, (size_t)_this->elementSize);
    ELSE
        memset(nd->payload, 0, (size_t)_this->elementSize);
    END_IF;

    *out_node = nd;
}
END_FUN

/* ======================== size / empty / clear ======================== */

MEM_FUN_IMPL(GenericList, size, MEM_SIZE_T* out_size)
{
    THROW_MSG_UNLESS(out_size, "out_size must not be NULL");
    *out_size = _this->size;
}
END_FUN

MEM_FUN_IMPL(GenericList, empty, bool* out_is_empty)
{
    THROW_MSG_UNLESS(out_is_empty, "out_is_empty must not be NULL");
    *out_is_empty = (_this->size == 0);
}
END_FUN

MEM_FUN_IMPL(GenericList, clear)
{
    ListNode* cur = _this->head;

    WHILE(cur != NULL)
        ListNode* next = cur->next;
    FREE(cur);
    cur = next;
    END_LOOP;

    _this->head = NULL;
    _this->tail = NULL;
    _this->size = 0;
}
END_FUN

/* ======================== list functions (generic, Vector-style) ======================== */


/* ---- push_back ---- */
MEM_FUN_IMPL(GenericList, __push_back_generic, char* buff, MEM_SIZE_T buff_size)
{
    THROW_MSG_UNLESS(buff_size == _this->elementSize, "Invalid Data Size");

    ListNode* nd = NULL;
    MFUN(_this, __make_node), (const char*)buff, buff_size, & nd CALL;

    IF(_this->tail == NULL)
        _this->head = nd;
    _this->tail = nd;
    ELSE
        nd->prev = _this->tail;
    _this->tail->next = nd;
    _this->tail = nd;
    END_IF;

    _this->size += 1;
}
END_FUN

#define IMPL_PUSH_BACK_OF_TYPE(type) \
MEM_FUN_IMPL(GenericList, push_back_ ##type, type val) { \
    MFUN(_this, __push_back_generic), (char*)&(val), (MEM_SIZE_T)sizeof(type) CALL; \
} END_FUN;
IMPL_PUSH_BACK_OF_TYPE(int)

/* ---- push_front ---- */
MEM_FUN_IMPL(GenericList, __push_front_generic, char* buff, MEM_SIZE_T buff_size)
{
    THROW_MSG_UNLESS(buff_size == _this->elementSize, "Invalid Data Size");

    ListNode* nd = NULL;
    MFUN(_this, __make_node), (const char*)buff, buff_size, & nd CALL;

    IF(_this->head == NULL)
        _this->head = nd;
    _this->tail = nd;
    ELSE
        nd->next = _this->head;
    _this->head->prev = nd;
    _this->head = nd;
    END_IF;

    _this->size += 1;
}
END_FUN

#define IMPL_PUSH_FRONT_OF_TYPE(type) \
MEM_FUN_IMPL(GenericList, push_front_ ##type, type val) { \
    MFUN(_this, __push_front_generic), (char*)&(val), (MEM_SIZE_T)sizeof(type) CALL; \
} END_FUN;
IMPL_PUSH_FRONT_OF_TYPE(int)

/* ---- back ---- */
MEM_FUN_IMPL(GenericList, __back_generic, char* buff, MEM_SIZE_T buff_size)
{
    THROW_MSG_UNLESS(_this->tail, "back() on empty list");
    THROW_MSG_UNLESS(buff_size == _this->elementSize, "Invalid Data Size");
    ASSERT_NOT_NULL(buff);
    memcpy(buff, _this->tail->payload, (size_t)_this->elementSize);
}
END_FUN

#define IMPL_BACK_OF_TYPE(type) \
MEM_FUN_IMPL(GenericList, back_ ##type, type* out_val) { \
    MFUN(_this, __back_generic), (char*)(out_val), (MEM_SIZE_T)sizeof(type) CALL; \
} END_FUN;
IMPL_BACK_OF_TYPE(int)

/* ---- front ---- */
MEM_FUN_IMPL(GenericList, __front_generic, char* buff, MEM_SIZE_T buff_size)
{
    THROW_MSG_UNLESS(_this->head, "front() on empty list");
    THROW_MSG_UNLESS(buff_size == _this->elementSize, "Invalid Data Size");
    ASSERT_NOT_NULL(buff);
    memcpy(buff, _this->head->payload, (size_t)_this->elementSize);
}
END_FUN

#define IMPL_FRONT_OF_TYPE(type) \
MEM_FUN_IMPL(GenericList, front_ ##type, type* out_val) { \
    MFUN(_this, __front_generic), (char*)(out_val), (MEM_SIZE_T)sizeof(type) CALL; \
} END_FUN;
IMPL_FRONT_OF_TYPE(int)

/* ---- pop_back ---- */
MEM_FUN_IMPL(GenericList, __pop_back_generic, char* buff, MEM_SIZE_T buff_size)
{
    THROW_MSG_UNLESS(_this->tail, "pop_back() on empty list");
    THROW_MSG_UNLESS(buff_size == _this->elementSize, "Invalid Data Size");
    ASSERT_NOT_NULL(buff);

    ListNode* nd = _this->tail;
    memcpy(buff, nd->payload, (size_t)_this->elementSize);

    _this->tail = nd->prev;
    IF(_this->tail) _this->tail->next = NULL;
    ELSE             _this->head = NULL;
    END_IF;

    FREE(nd);
    _this->size -= 1;
}
END_FUN

#define IMPL_POP_BACK_OF_TYPE(type) \
MEM_FUN_IMPL(GenericList, pop_back_ ##type, type* out_val) { \
    MFUN(_this, __pop_back_generic), (char*)(out_val), (MEM_SIZE_T)sizeof(type) CALL; \
} END_FUN;
IMPL_POP_BACK_OF_TYPE(int)

/* ---- pop_front ---- */
MEM_FUN_IMPL(GenericList, __pop_front_generic, char* buff, MEM_SIZE_T buff_size)
{
    THROW_MSG_UNLESS(_this->head, "pop_front() on empty list");
    THROW_MSG_UNLESS(buff_size == _this->elementSize, "Invalid Data Size");
    ASSERT_NOT_NULL(buff);

    ListNode* nd = _this->head;
    memcpy(buff, nd->payload, (size_t)_this->elementSize);

    _this->head = nd->next;
    IF(_this->head) _this->head->prev = NULL;
    ELSE             _this->tail = NULL;
    END_IF;

    FREE(nd);
    _this->size -= 1;
}
END_FUN

#define IMPL_POP_FRONT_OF_TYPE(type) \
MEM_FUN_IMPL(GenericList, pop_front_ ##type, type* out_val) { \
    MFUN(_this, __pop_front_generic), (char*)(out_val), (MEM_SIZE_T)sizeof(type) CALL; \
} END_FUN;
IMPL_POP_FRONT_OF_TYPE(int)

/* ======================== Iteration API (лое Vector) ======================== */

MEM_FUN_IMPL(GenericList, begin, Iterator** out_it)
{
    THROW_MSG_UNLESS(out_it, "out_it must not be NULL");

    ListIter* it = NULL;
    ALLOC(it, ListIter);
    INITIALIZE_INSTANCE(ListIter, (*it)) CALL;

    it->list = _this;
    it->node = _this->head;

    *out_it = (Iterator*)it;
}
END_FUN

MEM_FUN_IMPL(GenericList, end, Iterator** out_it)
{
    THROW_MSG_UNLESS(out_it, "out_it must not be NULL");

    ListIter* it = NULL;
    ALLOC(it, ListIter);
    INITIALIZE_INSTANCE(ListIter, (*it)) CALL;

    it->list = _this;
    it->node = NULL;

    *out_it = (Iterator*)it;
}
END_FUN

MEM_FUN_IMPL(GenericList, it_destroy, Iterator* it)
{
    ListIter* lit = (ListIter*)it;
    DESTROY(lit);
}
END_FUN

/* ======================== BIND Ц GenericList ======================== */

INIT_CLASS(GenericList);
BIND(GenericList, size);
BIND(GenericList, empty);
BIND(GenericList, clear);
BIND(GenericList, __make_node);

BIND(GenericList, __push_back_generic);
BIND(GenericList, __push_front_generic);
BIND(GenericList, __pop_back_generic);
BIND(GenericList, __pop_front_generic);
BIND(GenericList, __front_generic);
BIND(GenericList, __back_generic);

/* typed-on-base (exact vector pattern) */
BIND(GenericList, push_back_int);
BIND(GenericList, push_front_int);
BIND(GenericList, pop_back_int);
BIND(GenericList, pop_front_int);
BIND(GenericList, front_int);
BIND(GenericList, back_int);

BIND(GenericList, begin);
BIND(GenericList, end);
BIND(GenericList, it_destroy);
END_INIT_CLASS(GenericList)

/* =========================================================
 *                 ListIter (derived from Iterator)
 * ========================================================= */

DEF_DERIVED_CTOR(ListIter, Iterator) SUPER ME
{
    _this->list = NULL;
    _this->node = NULL;
}
END_DERIVED_CTOR

DEF_DERIVED_DTOR(ListIter, Iterator)
{
    _this->list = NULL;
    _this->node = NULL;
}
END_DERIVED_DTOR

FUN_OVERRIDE_IMPL(ListIter, Iterator, equals, object* other, bool* out_equal)
{
    THROW_MSG_UNLESS(out_equal, "out_equal must not be NULL");
    *out_equal = 0;
    IF(other)
        ListIter* o = (ListIter*)other;
    *out_equal = (o->list == _this->list) && (o->node == _this->node);
    END_IF;
}
END_FUN

FUN_OVERRIDE_IMPL(ListIter, Iterator, next)
{
    IF(_this->node)
        _this->node = _this->node->next;
    END_IF;
}
END_FUN

FUN_OVERRIDE_IMPL(ListIter, Iterator, prev)
{
    IF(_this->node)
        IF(_this->node->prev)
        _this->node = _this->node->prev;
    ELSE
        THROW_MSG("Iterator::prev called on begin()");
    END_IF;
    ELSE
        _this->node = _this->list ? _this->list->tail : NULL;
    END_IF;
}
END_FUN

FUN_OVERRIDE_IMPL(ListIter, Iterator, get_ref, void** out_ptr)
{
    THROW_MSG_UNLESS(out_ptr, "out_ptr must not be NULL");
    THROW_MSG_UNLESS(_this->node, "dereference end() iterator");
    *out_ptr = (void*)_this->node->payload;
}
END_FUN

FUN_OVERRIDE_IMPL(ListIter, Iterator, get_cref, const void** out_ptr)
{
    THROW_MSG_UNLESS(out_ptr, "out_ptr must not be NULL");
    THROW_MSG_UNLESS(_this->node, "dereference end() iterator");
    *out_ptr = (const void*)_this->node->payload;
}
END_FUN

FUN_OVERRIDE_IMPL(ListIter, Iterator, distance, object* other, ptrdiff_t* out_dist)
{
    THROW_MSG_UNLESS(out_dist, "out_dist must not be NULL");
    ListIter* o = (ListIter*)other;
    THROW_MSG_UNLESS(o && o->list == _this->list, "iterators not from same list");

    ptrdiff_t d = 0;
    ListNode* cur = _this->node;
    WHILE(cur)
        IF(cur == o->node)* out_dist = d; return; END_IF;
    cur = cur->next; d += 1;
    END_LOOP;

    d = 0; cur = _this->node;
    WHILE(cur)
        IF(cur == o->node)* out_dist = d; return; END_IF;
    cur = cur->prev; d -= 1;
    END_LOOP;

    THROW_MSG_UNLESS(0, "iterators not reachable from each other");
}
END_FUN

FUN_OVERRIDE_IMPL(ListIter, Iterator, advance, ptrdiff_t n)
{
    IF(n >= 0)
        WHILE(n > 0)
        IF(_this->node) _this->node = _this->node->next; END_IF;
    n -= 1;
    END_LOOP;
    ELSE
        WHILE(n < 0)
        IF(_this->node) _this->node = _this->node->prev;
    ELSE             _this->node = _this->list ? _this->list->tail : NULL;
    END_IF;
    n += 1;
    END_LOOP;
    END_IF;
}
END_FUN

INIT_DERIVED_CLASS(ListIter, Iterator);
BIND_OVERIDE(ListIter, Iterator, equals);
BIND_OVERIDE(ListIter, Iterator, next);
BIND_OVERIDE(ListIter, Iterator, prev);
BIND_OVERIDE(ListIter, Iterator, get_ref);
BIND_OVERIDE(ListIter, Iterator, get_cref);
BIND_OVERIDE(ListIter, Iterator, distance);
BIND_OVERIDE(ListIter, Iterator, advance);
END_INIT_CLASS(ListIter)

////////////////////////////////////////////////


#define IMPL_SPECIFIC_LIST_TYPE_xTORs(type)                                                    \
DEF_DERIVED_CTOR(List_ ##type, GenericList) SUPER, sizeof(type) ME {} END_DERIVED_CTOR         \
DEF_DERIVED_DTOR(List_ ##type, GenericList) {} END_DERIVED_DTOR

#define IMPL_SPECIFIC_LIST_TYPE_FUNCTIONS(type)                                                \
MEM_FUN_IMPL(List_ ##type, push_back,  type val)   { FUN_BASE(_this, push_back_  ##type), val CALL; } END_FUN; \
MEM_FUN_IMPL(List_ ##type, push_front, type val)   { FUN_BASE(_this, push_front_ ##type), val CALL; } END_FUN; \
MEM_FUN_IMPL(List_ ##type, pop_back,   type* out)  { FUN_BASE(_this, pop_back_   ##type), out CALL; } END_FUN; \
MEM_FUN_IMPL(List_ ##type, pop_front,  type* out)  { FUN_BASE(_this, pop_front_  ##type), out CALL; } END_FUN; \
MEM_FUN_IMPL(List_ ##type, front,      type* out)  { FUN_BASE(_this, front_      ##type), out CALL; } END_FUN; \
MEM_FUN_IMPL(List_ ##type, back,       type* out)  { FUN_BASE(_this, back_       ##type), out CALL; } END_FUN; \
MEM_FUN_IMPL(List_ ##type, size,       MEM_SIZE_T* out) { FUN_BASE(_this, size),  out CALL; } END_FUN; \
MEM_FUN_IMPL(List_ ##type, empty,      bool* out)       { FUN_BASE(_this, empty), out CALL; } END_FUN; \
MEM_FUN_IMPL(List_ ##type, clear)                      { FUN_BASE(_this, clear) CALL; } END_FUN; \
MEM_FUN_IMPL(List_ ##type, begin, Iterator** it)  { FUN_BASE(_this, begin),     it CALL; } END_FUN; \
MEM_FUN_IMPL(List_ ##type, end,   Iterator** it)  { FUN_BASE(_this, end),       it CALL; } END_FUN; \
MEM_FUN_IMPL(List_ ##type, it_destroy, Iterator* it) { FUN_BASE(_this, it_destroy), it CALL; } END_FUN; \
INIT_DERIVED_CLASS(List_ ##type, GenericList);                                                  \
BIND(List_ ##type, push_back);                                                                  \
BIND(List_ ##type, push_front);                                                                 \
BIND(List_ ##type, pop_back);                                                                   \
BIND(List_ ##type, pop_front);                                                                  \
BIND(List_ ##type, front);                                                                      \
BIND(List_ ##type, back);                                                                       \
BIND(List_ ##type, size);                                                                       \
BIND(List_ ##type, empty);                                                                      \
BIND(List_ ##type, clear);                                                                      \
BIND(List_ ##type, begin);                                                                      \
BIND(List_ ##type, end);                                                                        \
BIND(List_ ##type, it_destroy);                                                                 \
END_INIT_CLASS(List_ ##type)


////////////////////////////////////////////////
IMPL_SPECIFIC_LIST_TYPE_xTORs(int);
IMPL_SPECIFIC_LIST_TYPE_FUNCTIONS(int);

