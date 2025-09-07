#include "Iterator.h"


DEF_CTOR(Iterator)
END_CTOR

DEF_DTOR(Iterator)
END_DTOR


MEM_FUN_IMPL(Iterator, equals, object* other, bool* out_equal)
END_FUN

MEM_FUN_IMPL(Iterator, next)
END_FUN

MEM_FUN_IMPL(Iterator, prev)
END_FUN

MEM_FUN_IMPL(Iterator, get_ref, void** out_ptr)
END_FUN

MEM_FUN_IMPL(Iterator, get_cref, const void** out_ptr)
END_FUN

MEM_FUN_IMPL(Iterator, distance, object* other, ptrdiff_t* out_dist)
END_FUN

MEM_FUN_IMPL(Iterator, advance, ptrdiff_t n)
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