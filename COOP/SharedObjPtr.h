#ifndef __MY__SHARED_OBJ_PTR__H__
#define __MY__SHARED_OBJ_PTR__H__

#include "SharedPodPtr.h"

DEF_DERIVED_CLASS(objSPtr, SharedPodPtr);
object* objPtr;
END_DEF_DERIVED(objSPtr);

DERIVED_FUNCTIONS(objSPtr, SharedPodPtr, object* obj);
MEM_FUN_DECL(objSPtr, reset, object* newObjr);
MEM_FUN_DECL(objSPtr, copyFrom, objSPtr const* other);
MEM_FUN_DECL(objSPtr, print);
FUN_OVERRIDE(SharedPodPtr, release);
//MEM_FUN_DECL(objSPtr, isEmpty, bool * out);
END_DERIVED_FUNCTIONS(objSPtr);

#endif