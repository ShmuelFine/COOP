#ifndef __LINKED_LISTS__H_
#define __LINKED_LISTS__H_

#include "ExportDefs.h"
#include "obj_lifecycle_management.h"
#include "object.h"


#ifdef __cplusplus
extern "C" {
#endif

// Call function of base class:
//#define _BASE(base, funcName,...) _this->vTable->_base.__ ##funcName.func((base *)_this,__VA_ARGS__)

// Call any member function: // TODO:Tested?
// The C CALL syntax does not work for C++, here we need full qualification of the inner struct
#define FUN(funcName,_this) ((_this)->vTable->funcName)->outer_function((_this))(_this
#define FUN_STATIC(type, funcName,_this) ((_this)->vTable->funcName)->outer_function((_this))(_this
	
	//(*(_this.vTable->funcName.outer_function))


#ifdef __cplusplus
}
#endif // __cplusplus

#endif