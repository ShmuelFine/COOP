#include "Iterator.h"


DEF_CTOR(Iterator, IteratorCategory category, void *container_ptr)
{
    _this->_category = category;
	_this->container_ptr = container_ptr;
}
END_CTOR

DEF_DTOR(Iterator)
END_DTOR

PURE_VIRTUAL(Iterator, next);
PURE_VIRTUAL(Iterator, get_ref, void** out_ptr);
PURE_VIRTUAL(Iterator, get_cref, const void** out_ptr);
PURE_VIRTUAL(Iterator, distance, Iterator* other, ptrdiff_t* out_dist);
PURE_VIRTUAL(Iterator, reset_begin);

MEM_FUN_IMPL(Iterator, equals, Iterator* other, bool* out_equal)
{
    *out_equal = 0;
    IF(other) {
        *out_equal = (_this->_category == other->_category) &&
            (_this->container_ptr == other->container_ptr);
    }END_IF

}END_FUN

MEM_FUN_IMPL(Iterator, prev)
{
    IF (_this->_category < ITER_BIDIRECTIONAL)
    {
        THROW_MSG("prev() not supported for non-bidirectional iterator.");
    }END_IF
    THROW_MSG("Pure virtual: prev() not implemented.");
}
END_FUN


MEM_FUN_IMPL(Iterator, advance, ptrdiff_t n)
{
    IF (_this->_category >= ITER_RANDOM_ACCESS)
    {
        THROW_MSG("Pure virtual: advance() not implemented for random access iterator.");
    }END_IF


    IF (n > 0)
    {
        FOR(ptrdiff_t i = 0; i < n; ++i)
        {
            ITER_NEXT((Iterator*)_this);
        }END_LOOP
    }
    ELSE_IF (n < 0)
    {
        IF(_this->_category < ITER_BIDIRECTIONAL)
        {
            THROW_MSG("Cannot advance backward: Iterator does not support prev().");
        }END_IF
        FOR(ptrdiff_t i = 0; i > n; --i)
        {
            ITER_PREV((Iterator*)_this);
        }END_LOOP
    }END_IF
}
END_FUN

INIT_CLASS(Iterator);
BIND(Iterator, equals);
BIND(Iterator, next);
BIND(Iterator, prev);
BIND(Iterator, get_ref);
BIND(Iterator, get_cref);
BIND(Iterator, distance);
BIND(Iterator, advance);
BIND(Iterator, reset_begin);
END_INIT_CLASS(Iterator);
