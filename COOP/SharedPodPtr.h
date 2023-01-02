#ifndef __MY__SHARED_PTR__H__
#define __MY__SHARED_PTR__H__

#include "COOP.h"

DEF_CLASS(SharedPodPtr);
void * px;
int * pn;
END_DEF(SharedPodPtr);

FUNCTIONS(SharedPodPtr, MEM_SIZE_T size_bytes);
MEM_FUN_DECL(SharedPodPtr, reset, void * newPtr);
MEM_FUN_DECL(SharedPodPtr, copyFrom, SharedPodPtr const* other);
MEM_FUN_DECL(SharedPodPtr, release);
MEM_FUN_DECL(SharedPodPtr, isEmpty, bool * out);
END_FUNCTIONS(SharedPodPtr);

#define GET(s_ptr, Type) ((Type*)(s_ptr.px))

#endif
