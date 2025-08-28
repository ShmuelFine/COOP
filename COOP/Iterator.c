#include "Iterator.h"


DEF_CTOR(Iterator)
{
    _this->category = ITER_INPUT;
}
END_CTOR

DEF_DTOR(Iterator)
{
  //??
}
END_DTOR


MEM_FUN_IMPL(Iterator, equals, object* other, bool* out_equal)
{
    *out_equal = ((object*)_this) == other;
    return 0;
}
END_FUN

MEM_FUN_IMPL(Iterator, next)
{
    return 0;
}
END_FUN

MEM_FUN_IMPL(Iterator, prev)
{
    return 0;
}
END_FUN

MEM_FUN_IMPL(Iterator, get_ref, void** out_ptr)
{
    *out_ptr = NULL;
    return 0;
}
END_FUN

MEM_FUN_IMPL(Iterator, get_cref, const void** out_ptr)
{
    *out_ptr = NULL;
    return 0;
}
END_FUN

MEM_FUN_IMPL(Iterator, distance, object* other, ptrdiff_t* out_dist)
{
    *out_dist = 0;
    return 0;
}
END_FUN

MEM_FUN_IMPL(Iterator, advance, ptrdiff_t n)
{
    (void)n;
    return 0;
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
END_INIT_CLASS(Iterator);
