#ifndef __LINKED_LISTS__H_
#define __LINKED_LISTS__H_

#include "ExportDefs.h"
#include "obj_lifecycle_management.h"

#define FUN(_this, funcName)\
{ int _retVal_ = ((_this)->vTable->funcName).outer_function((_this))(_this

#define CALL ); if (IN_THROWING_VALUE == _retVal_) {THROW;} } 

#define FUN_BASE(_this, funcName)\
{ int _retVal_ = _this->_base.vTable->GetVolume.inner_function(&(_this->_base)

#endif