#ifndef __LINKED_LISTS__H_
#define __LINKED_LISTS__H_

#include "ExportDefs.h"
#include "ObjectLifecycleManagement.h"

#define MFUN(_this, funcName)\
{ int __INNER_FUNC_CALL_RET_VALUE__ = ((_this)->vTable->funcName).outer_function((_this))(_this


#define FUN_BASE(_this, funcName)\
{ int __INNER_FUNC_CALL_RET_VALUE__ = _this->_base.vTable->funcName.inner_function(&(_this->_base)

#endif