#ifndef __MY__SHARED_PTR__H__
#define __MY__SHARED_PTR__H__

#include "coop.h"
#include "DynamicMemoryManagement.h"

#define GET_VARIABLE_NAME(Variable) (#Variable)

DEF_CLASS(Shared_ptr);
void * px; // the pointed object
int * pn; // ref count
END_DEF(Shared_ptr);

FUNCTIONS(Shared_ptr);
MEM_FUN_DECL(Shared_ptr, reset, void * newPtr);
MEM_FUN_DECL(Shared_ptr, copyTo, Shared_ptr* other);
MEM_FUN_DECL(Shared_ptr, copyFrom, Shared_ptr const* other);
MEM_FUN_DECL(Shared_ptr, release);
MEM_FUN_DECL(Shared_ptr, isEmpty, bool * out);
END_FUNCTIONS(Shared_ptr);

#define GET(s_ptr, Type) ((Type*)(s_ptr.px))

#define CREATE_SHARED(type, shared_ptr_instance_name, ...)\
CREATE_PTR(type, __ ##shared_ptr_instance_name ##__ ##type ##__ptr), __VA_ARGS__ CALL;\
CREATE(Shared_ptr, shared_ptr_instance_name) CALL;\
MFUN(&shared_ptr_instance_name, reset), __ ##shared_ptr_instance_name ##__ ##type ##__ptr CALL;

#endif