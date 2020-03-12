#ifndef __LINKED_LISTS__H_
#define __LINKED_LISTS__H_

#include "ExportDefs.h"
#include "obj_lifecycle_management.h"


#ifdef __cplusplus
extern "C" {
#endif

// Call function of base class:

// Call any member function: // TODO:Tested?
// The C CALL syntax does not work for C++, here we need full qualification of the inner struct
//#define TOKEN_PASTE(x, y) x##y
//#define CAT(x,y) TOKEN_PASTE(x,y)
//#define UNIQUE_INT \
//  int CAT(prefix, __LINE__)

#define FUN(_this, funcName)\
{ int _retVal_ = ((_this)->vTable->funcName).outer_function((_this))(_this
#define CALL ); if (IN_THROWING_VALUE == _retVal_) {THROW;} }

#define FUN_BASE(_this, funcName)\
{ int _retVal_ = _this->_base.vTable->GetVolume.inner_function(&(_this->_base)
// _BASE macro calls the methods of the parent class. It's meant to be called from within a member 
// function, where "_this" is defined.
//#define _BASE(base, funcName) FUN_STATIC(base, funcName, _this)

	//(*(_this.vTable->funcName.outer_function))


#ifdef __cplusplus
}
#endif // __cplusplus

#endif