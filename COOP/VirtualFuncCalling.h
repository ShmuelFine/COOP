#ifndef __LINKED_LISTS__H_
#define __LINKED_LISTS__H_

#include "ExportDefs.h"
#include "ObjectLifecycleManagement.h"

#define MFUN(_this, funcName)\
{ int _retVal_ = ((_this)->vTable->funcName).outer_function((_this))(_this


#define FUN_BASE(_this, funcName)\
{ int _retVal_ = _this->_base.vTable->funcName.inner_function(&(_this->_base)

#endif