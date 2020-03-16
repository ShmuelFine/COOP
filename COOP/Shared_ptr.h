#ifndef __MY__SHARED_PTR__H__
#define __MY__SHARED_PTR__H__

#include "coop.h"

DEF_CLASS(Shared_ptr);
// Define member fields here. e.g:
int* px; // the pointed object
int* pn; // ref count
END_DEF(Shared_ptr);

FUNCTIONS(Shared_ptr);
MEM_FUN_DECL(Shared_ptr, Reset, int* newPtr);
MEM_FUN_DECL(Shared_ptr, CopyTo, Shared_ptr* other);
MEM_FUN_DECL(Shared_ptr, CopyFrom, Shared_ptr const* other);
MEM_FUN_DECL(Shared_ptr, Release);
MEM_FUN_DECL(Shared_ptr, IsEmpty, bool * out);
END_FUNCTIONS(Shared_ptr);

#endif