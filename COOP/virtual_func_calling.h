#ifndef __LINKED_LISTS__H_
#define __LINKED_LISTS__H_

#include "ExportDefs.h"
#include "obj_lifecycle_management.h"
#include "object.h"


#ifdef __cplusplus
extern "C" {
#endif

//#define FUNCTION_TYPE(type,function_name,...)				\
//	struct type ##_ ##function_name ##_t_{								\
//		int (* function_name ##_inner_function)(void* _this, __VA_ARGS__);	\
//		function_name ##_inner_function * (*outer_function)(void* _this, __VA_ARGS__);	\
//		struct function_name ##_t_ * next;					\
//	}__ ##function_name,	*function_name


// Call function of base class:
//#define _BASE(base, funcName,...) _this->vTable->_base.__ ##funcName.func((base *)_this,__VA_ARGS__)

// Call any member function: // TODO:Tested?
// The C CALL syntax does not work for C++, here we need full qualification of the inner struct
#define FUN(funcName,_this) ((_this)->vTable->funcName)->outer_function((_this))(
	
	//(*(_this.vTable->funcName.outer_function))


#ifdef __cplusplus
}
#endif // __cplusplus

#endif