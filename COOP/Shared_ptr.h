#ifndef __MY__SHARED_PTR__H__
#define __MY__SHARED_PTR__H__

#include "coop.h"

DEF_CLASS(Shared_ptr);
// Define member fields here. e.g:
int* px; // the pointed object
int* pn; // ref count
END_DEF(Shared_ptr);

FUNCTIONS(Shared_ptr, int* ptr_x);
MEM_FUN_DECL(Shared_ptr, initialize_ref_count, int* ptr_x, int* PRefCount);
END_FUNCTIONS(Shared_ptr);

#endif