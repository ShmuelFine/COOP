#ifndef __COOP_obj_lifecycle_management__
#define __COOP_obj_lifecycle_management__


#include "ExportDefs.h"
#include "obj_base_structs.h"
#include <setjmp.h>

COOP_API void _scope_obj_list_add(object* scope_list, object* obj);
COOP_API void _scope_obj_list_free(object* scope_list);
COOP_API void FreeMostInnerScope(object* _scope_obj_list);

#define REGISTER_OBJECT(obj) _scope_obj_list_add(&_scope_obj_list, (object*)obj)

#define CREATE_OBJECT(type, instance_name, ...)						\
	if (! is_ ##type ##VirtualTable__initialized) type ##_init();   \
	type instance_name;                  							\
	instance_name.vTable=&type ##VTable;							\
	instance_name.vTable->_ctor(&instance_name, __VA_ARGS__);		\
	instance_name._next= NULL;										\
	REGISTER_OBJECT(&instance_name)

#define CREATE_DERIVED_OBJECT(type, base, instance_name, ...)	 \
	CREATE_OBJECT(type,instance_name, __VA_ARGS__)


// The obj lifecycle built upon scopes, that has to account with exception handling:
COOP_API extern jmp_buf SCOPE_FALLBACK_ADDR[10];
COOP_API extern int _CurrScope_Idx;

#define ERROR_VALUE -1

#define TRY if(!setjmp(SCOPE_FALLBACK_ADDR[_CurrScope_Idx]))

#define CATCH \
else { 

#define END_TRY }

#define THROW \
longjmp(SCOPE_FALLBACK_ADDR[--_CurrScope_Idx],1)

#define SCOPE_START \
object _scope_obj_list; \
int __RET_VAL__ = 0;\
if(!setjmp(SCOPE_FALLBACK_ADDR[_CurrScope_Idx++]))\
{  \
	_scope_obj_list.vTable=NULL; \
	_scope_obj_list._next=NULL		

#define SCOPE_END\
	FreeMostInnerScope(&_scope_obj_list); \
	_CurrScope_Idx--;\
}else{\
	FreeMostInnerScope(&_scope_obj_list);\
	if(_CurrScope_Idx > 0)\
		_CurrScope_Idx--;\
	__RET_VAL__ = ERROR_VALUE;\
}\
return __RET_VAL__; 

#define LOCAL_SCOPE_START \
if(!setjmp(SCOPE_FALLBACK_ADDR[_CurrScope_Idx++]))\
{\
	object LocalScopeStartMarker; \
	LocalScopeStartMarker.vTable=NULL;\
	LocalScopeStartMarker._next=NULL; \
	_scope_obj_list_add(&_scope_obj_list,&LocalScopeStartMarker)


#define LOCAL_SCOPE_END \
	FreeMostInnerScope(&_scope_obj_list);\
	_CurrScope_Idx--; \
}\
else {\
	FreeMostInnerScope(&_scope_obj_list);\
	_CurrScope_Idx--; \
	longjmp(SCOPE_FALLBACK_ADDR[_CurrScope_Idx], 1);\
}

#endif

