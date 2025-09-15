#ifndef __LINKED_LISTS__H_
#define __LINKED_LISTS__H_

#include "ExportDefs.h"
#include "ObjectLifecycleManagement.h"
#define MFUN(_this, funcName)                                                                    \
{                                                                                                \
    int __INNER_FUNC_CALL_RET_VALUE__ = ERROR_VALUE;                                             \
    if ((_this) == NULL || (_this)->vTable == NULL) {                                            \
        THROW_MSG("Invalid object or vtable");                                                   \
    }                                                                                            \
    if (((_this)->vTable->funcName).outer_function == NULL) {                                    \
        THROW_MSG("Pure virtual function '" #funcName "' not implemented! (outer=NULL)");        \
    }                                                                                            \
    if (((_this)->vTable->funcName).outer_function((_this)) == NULL) {                           \
        THROW_MSG("Pure virtual function '" #funcName "' not implemented! (inner=NULL)");        \
    }                                                                                            \
    __INNER_FUNC_CALL_RET_VALUE__ = (((_this)->vTable->funcName).outer_function((_this)))(_this

#define FUN_BASE(_this, funcName)                                                                \
{                                                                                                \
    int __INNER_FUNC_CALL_RET_VALUE__ = ERROR_VALUE;                                             \
    if ((_this) == NULL || (_this)->_base.vTable == NULL) {                                      \
        THROW_MSG("Invalid base object or vtable");                                              \
    }                                                                                            \
    if ((_this)->_base.vTable->funcName.inner_function == NULL) {                                \
        THROW_MSG("Pure virtual function '" #funcName "' not implemented in base!");             \
    }                                                                                            \
    __INNER_FUNC_CALL_RET_VALUE__ = (_this)->_base.vTable->funcName.inner_function(&((_this)->_base)
#endif