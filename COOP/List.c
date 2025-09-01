#include "List.h"
#include "COOP.h"
#include <stdlib.h>
#include <string.h>

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

/* Helpers as COOP FUN */

//create a new node
MEM_FUN_IMPL(GenericList, __make_node, const void* src_bytes, ListNode** out_node)
{
    THROW_MSG_UNLESS(out_node, "out_node must not be NULL");

    MEM_SIZE_T bytes = (MEM_SIZE_T)(sizeof(ListNode) + _this->elementSize);

    ListNode* nd = NULL;

    ALLOC_ARRAY(nd, char, bytes);

    nd->prev = NULL;
    nd->next = NULL;

    IF(src_bytes)
        memcpy(nd->payload, src_bytes, (size_t)_this->elementSize);
    END_IF;

    *out_node = nd;
}
END_FUN;


/* size / empty / clear */

MEM_FUN_IMPL(GenericList, size, MEM_SIZE_T* out_size)
{
    THROW_MSG_UNLESS(out_size, "out_size must not be NULL");
    *out_size = _this->size;
}
END_FUN;

MEM_FUN_IMPL(GenericList, empty, bool* out_is_empty)
{
    THROW_MSG_UNLESS(out_is_empty, "out_is_empty must not be NULL");
    *out_is_empty = (_this->size == 0);
}
END_FUN;

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
END_FUN;



/* list functions */

MEM_FUN_IMPL(GenericList, push_back_generic, const void* src)
{
    ListNode* nd = NULL;
    MFUN(_this, __make_node), src, & nd CALL;

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
END_FUN;


MEM_FUN_IMPL(GenericList, push_front_generic, const void* src)
{
    ListNode* nd = NULL;
    MFUN(_this, __make_node), src, & nd CALL;

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
END_FUN;


MEM_FUN_IMPL(GenericList, back_generic, void* out_elem)
{
    THROW_MSG_UNLESS(_this->tail, "back() on empty list");
    IF(out_elem)
        memcpy(out_elem, _this->tail->payload, (size_t)_this->elementSize);
    END_IF;
}
END_FUN;


MEM_FUN_IMPL(GenericList, front_generic, void* out_elem)
{
    THROW_MSG_UNLESS(_this->head, "front() on empty list");
    IF(out_elem)
        memcpy(out_elem, _this->head->payload, (size_t)_this->elementSize);
    END_IF;
}
END_FUN;


MEM_FUN_IMPL(GenericList, pop_back_generic, void* out_elem)
{
    THROW_MSG_UNLESS(_this->tail, "pop_back() on empty list");

    ListNode* nd = _this->tail;

    IF(out_elem)
        memcpy(out_elem, nd->payload, (size_t)_this->elementSize);
    END_IF;

    _this->tail = nd->prev;
    IF(_this->tail)
        _this->tail->next = NULL;
    ELSE
        _this->head = NULL; //empty list
    END_IF;

    FREE(nd);
    _this->size -= 1;
}
END_FUN;


MEM_FUN_IMPL(GenericList, pop_front_generic, void* out_elem)
{
    THROW_MSG_UNLESS(_this->head, "pop_front() on empty list");

    ListNode* nd = _this->head;

    IF(out_elem)
        memcpy(out_elem, nd->payload, (size_t)_this->elementSize);
    END_IF;

    _this->head = nd->next;
    IF(_this->head)
        _this->head->prev = NULL;
    ELSE
        _this->tail = NULL; //empty list
    END_IF;

    FREE(nd);
    _this->size -= 1;
}
END_FUN;


/*  ListIterator overrides  */

MEM_FUN_IMPL(GenericList, begin, ListIterator** out_it)
{
    THROW_MSG_UNLESS(out_it, "out_it must not be NULL");

    ListIterator* it = NULL;
    ALLOC(it, ListIterator);                       
    INITIALIZE_INSTANCE(ListIterator, (*it)) CALL;  //connect vtable + ctor 

    it->owner = _this;
    it->node = _this->head;  

    *out_it = it;
}
END_FUN;

MEM_FUN_IMPL(GenericList, end, ListIterator** out_it)
{
    THROW_MSG_UNLESS(out_it, "out_it must not be NULL");

    ListIterator* it = NULL;
    ALLOC(it, ListIterator);
    INITIALIZE_INSTANCE(ListIterator, (*it)) CALL;

    it->owner = _this;
    it->node = NULL;          // past-the-end

    *out_it = it;
}
END_FUN;


FUN_OVERRIDE_IMPL(ListIterator, Iterator, equals, object* other, bool* out_equal)
{
    THROW_MSG_UNLESS(out_equal, "out_equal must not be NULL");
    ListIterator* o = (ListIterator*)other;
    *out_equal = (o && _this->owner == o->owner && _this->node == o->node);
}
END_FUN;

FUN_OVERRIDE_IMPL(ListIterator, Iterator, next)
{
    IF(_this->node)
        _this->node = _this->node->next;
    END_IF;
}
END_FUN;

FUN_OVERRIDE_IMPL(ListIterator, Iterator, prev)
{
    IF(_this->node)
        IF(_this->node->prev)
        _this->node = _this->node->prev;
    ELSE
        //node == head
        THROW_MSG("Iterator::prev called on begin()");
    END_IF;
    ELSE
        /* end()-- =>jump to-tail */
        _this->node = _this->owner ? _this->owner->tail : NULL;
    END_IF;
}
END_FUN;


FUN_OVERRIDE_IMPL(ListIterator, Iterator, get_ref, void** out_ptr)
{
    THROW_MSG_UNLESS(out_ptr, "out_ptr must not be NULL");
    THROW_MSG_UNLESS(_this->node, "dereference end() iterator");
    *out_ptr = (void*)_this->node->payload;
}
END_FUN;

FUN_OVERRIDE_IMPL(ListIterator, Iterator, get_cref, const void** out_ptr)
{
    THROW_MSG_UNLESS(out_ptr, "out_ptr must not be NULL");
    THROW_MSG_UNLESS(_this->node, "dereference end() iterator");
    *out_ptr = (const void*)_this->node->payload;
}
END_FUN;

FUN_OVERRIDE_IMPL(ListIterator, Iterator, distance, object* other, ptrdiff_t* out_dist)
{
    THROW_MSG_UNLESS(out_dist, "out_dist must not be NULL");
    ListIterator* o = (ListIterator*)other;
    THROW_MSG_UNLESS(o && o->owner == _this->owner, "iterators not from same list");

    //Forward
    ptrdiff_t d = 0;
    ListNode* cur = _this->node;
    WHILE(cur)
        IF(cur == o->node)
        * out_dist = d;
    return;
    END_IF;
    cur = cur->next;
    d += 1;
    END_LOOP;

    //Back
    d = 0;
    cur = _this->node;
    WHILE(cur)
        IF(cur == o->node)
        * out_dist = d;
    return;
    END_IF;
    cur = cur->prev;
    d -= 1;
    END_LOOP;

    THROW_MSG_UNLESS(0, "iterators not reachable from each other");
}
END_FUN;


FUN_OVERRIDE_IMPL(ListIterator, Iterator, advance, ptrdiff_t n)
{
    IF(n >= 0)
        WHILE(n > 0)
        IF(_this->node) _this->node = _this->node->next; END_IF;
    n -= 1;
    END_LOOP;
    ELSE
        WHILE(n < 0)
        IF(_this->node) _this->node = _this->node->prev;
    ELSE            _this->node = _this->owner ? _this->owner->tail : NULL;
    END_IF;
    n += 1;
    END_LOOP;
    END_IF;
}
END_FUN;

/*GenericList - BIND*/
INIT_CLASS(GenericList);
    BIND(GenericList, size);
    BIND(GenericList, empty);
    BIND(GenericList, clear);
    BIND(GenericList, __make_node);
    BIND(GenericList, push_back_generic);
    BIND(GenericList, push_front_generic);
    BIND(GenericList, pop_back_generic);
    BIND(GenericList, pop_front_generic);
    BIND(GenericList, front_generic);
    BIND(GenericList, back_generic);
    BIND(GenericList, begin);
    BIND(GenericList, end);
END_INIT_CLASS(GenericList);


/* ListIterator –  Overrides */
INIT_DERIVED_CLASS(ListIterator, Iterator);
    BIND_OVERIDE(ListIterator, Iterator, equals);
    BIND_OVERIDE(ListIterator, Iterator, next);
    BIND_OVERIDE(ListIterator, Iterator, prev);
    BIND_OVERIDE(ListIterator, Iterator, get_ref);
    BIND_OVERIDE(ListIterator, Iterator, get_cref);
    BIND_OVERIDE(ListIterator, Iterator, distance);
    BIND_OVERIDE(ListIterator, Iterator, advance);
END_INIT_CLASS(ListIterator);



