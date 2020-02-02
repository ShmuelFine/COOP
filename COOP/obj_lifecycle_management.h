#ifndef __COOP_obj_lifecycle_management__
#define __COOP_obj_lifecycle_management__


#include "ExportDefs.h"
#include "obj_base_structs.h"
#include <setjmp.h>

COOP_API void _scope_obj_list_add(object* scope_list, object* obj);
COOP_API void _scope_obj_list_free(object* scope_list);
COOP_API void FreeMostInnerScope(object* _scope_obj_list);

#define REGISTER_OBJECT(obj) _scope_obj_list_add(&_scope_obj_list, (object*)obj)

#define CREATE(type, instance_name)							\
	if (! is_ ##type ##VirtualTable__initialized) type ##_init();   \
	type instance_name;                  							\
	instance_name._next= NULL;										\
	instance_name.vTable=&type ##VTable;							\
	REGISTER_OBJECT(&instance_name);								\
	instance_name.vTable->_ctor(&instance_name


// The obj lifecycle built upon scopes, that has to account with exception handling:

#define SUCCESS_VALUE 0
#define ERROR_VALUE -1
#define IN_THROWING_VALUE -10
#define IS_IN_THROWING (__RET_VAL__ == IN_THROWING_VALUE)

#define TRY IS_BREAKING = false; \
for (int i_ ##__LINE__= 0; i_ ##__LINE__ < 1; i_ ##__LINE__++)\
{

#define BREAK {IS_BREAKING = true; FreeMostInnerScope(&_scope_obj_list); break;}

#define CATCH \
}\
if (IS_BREAKING) {break;} \
else if (IS_IN_THROWING) { __RET_VAL__ = SUCCESS_VALUE;

#define END_TRY }\
{if (IS_BREAKING || IS_IN_THROWING) break;}

#define THROW \
__RET_VAL__ = IN_THROWING_VALUE; break;

COOP_API extern const char* LAST_EXCEPTION_ERROR_MSG;

#define THROW_MSG(msg) LAST_EXCEPTION_ERROR_MSG = msg; THROW

#define SCOPE_START					\
object _scope_obj_list;				\
TRY									\
	_scope_obj_list.vTable=NULL;	\
	_scope_obj_list._next=NULL		

#define END_SCOPE\
	FreeMostInnerScope(&_scope_obj_list); \
	}\
	{if (IS_BREAKING) {IS_BREAKING = false; break;}\
	else if (IS_IN_THROWING) break;}


#define FUN_IMPL(function_name, ...)\
int function_name(__VA_ARGS__)\
{ \
int __RET_VAL__ = SUCCESS_VALUE;\
int IS_BREAKING = false;\
SCOPE_START;

#define END_FUN \
}FreeMostInnerScope(&_scope_obj_list);\
return __RET_VAL__;\
}

#define FUN_DECL(function_name, ...) int function_name(__VA_ARGS__)

#define RETURN(i) __RET_VAL__ = i; break;
#define ASSERT(x) if (!(x)) {THROW;}

#endif

