#ifndef __COOP_OBJ_LIEFCYCLE_MANAGEMENT__
#define __COOP_OBJ_LIEFCYCLE_MANAGEMENT__

#include "ExportDefs.h"
#include "ObjectBaseStructs.h"

///////////////// PREDEFINED RETURN CODES: ///////////////////////////////

#define SUCCESS_VALUE 0
#define ERROR_VALUE 1
#define IN_THROWING_VALUE 2
#define IN_RETURNING_VALUE 3
#define IS_IN_THROWING() (__RET_VAL__ == IN_THROWING_VALUE)
#define IS_IN_RETURNING() (__RET_VAL__ == IN_RETURNING_VALUE)

///////////////// EXCEPTION HANDLING: /////////////////////////////// <-- note the BREAK

#define TRY IS_BREAKING = false; IS_CONTINUING=false; \
for (int i_ ##__LINE__= 0; i_ ##__LINE__ < 1; i_ ##__LINE__++)\
{

#define BREAK {IS_BREAKING = true;IS_CONTINUING=false; _scope_obj_list_call_dtors(&_scope_obj_list); break;}

#define CONTINUE {IS_BREAKING = false;IS_CONTINUING=true; _scope_obj_list_call_dtors(&_scope_obj_list); break;}

#define CATCH												\
}															\
if (IS_BREAKING) {break;}									\
else if (IS_IN_THROWING()) { __RET_VAL__ = SUCCESS_VALUE;

#define END_TRY												\
}															\
{if (IS_BREAKING ||IS_CONTINUING || IS_IN_THROWING()) break;}

#define THROW \
__RET_VAL__ = IN_THROWING_VALUE; /*__debugbreak()*/; break;

#define RETURN \
__RET_VAL__ = IN_RETURNING_VALUE; break;


COOP_API extern const char* LAST_EXCEPTION_ERROR_MSG;

#define THROW_MSG(msg) LAST_EXCEPTION_ERROR_MSG = (msg); THROW

///////////////// THE SCOPE: ///////////////////////////////

#define SCOPE_START					\
object _scope_obj_list;				\
TRY									\
	_scope_obj_list.vTable=NULL;	\
	_scope_obj_list._next=NULL

#define SCOPE_END }

COOP_API void _scope_obj_list_add(object* scope_list, object* obj);
COOP_API void _scope_obj_list_call_dtors(object* _scope_obj_list);

#define REGISTER_OBJECT(obj) _scope_obj_list_add(&_scope_obj_list, (object*)(obj))

#define INITIALIZE_INSTANCE(type, instance_name)					\
	if (!is_ ##type ##VirtualTable__initialized) type ##_init();	\
	(instance_name).vTable = &V_TABLE_INSTANCE(type);					\
	{ int __INNER_FUNC_CALL_RET_VALUE__ = (instance_name).vTable->_ctor(&(instance_name)


#define CREATE(type, instance_name)									\
	type instance_name;                  							\
	REGISTER_OBJECT(&(instance_name));								\
	INITIALIZE_INSTANCE(type, instance_name)

// D'TOR IS NOT ALLOWED TO THROW... otherwise it creates a cyclic dependence with _scope_obj_list_call_dtors.
#define DESTROY(instance_ptr) {if (instance_ptr) (instance_ptr)->vTable->_dtor(instance_ptr);}

///////////////// CONDITIONS: ///////////////////////////////
#define IF(...) if (__VA_ARGS__) { SCOPE_START; 
#define ELSE_IF(...) _scope_obj_list_call_dtors(&_scope_obj_list); SCOPE_END } \
else if (IS_BREAKING ||IS_CONTINUING || IS_IN_RETURNING() || IS_IN_THROWING()) break;		\
else if (__VA_ARGS__) { SCOPE_START; 

#define ELSE ELSE_IF(1)

#define END_IF   _scope_obj_list_call_dtors(&_scope_obj_list); SCOPE_END\
 } if (IS_BREAKING ||IS_CONTINUING || IS_IN_RETURNING() || IS_IN_THROWING()) break;

///////////////// LOOPS: ///////////////////////////////
#define FOR(...) for (__VA_ARGS__) { SCOPE_START; 
#define WHILE(...) while (__VA_ARGS__) { SCOPE_START; 

#define END_LOOP 	_scope_obj_list_call_dtors(&_scope_obj_list); }	\
	{																\
		if (IS_BREAKING) {IS_BREAKING = false; break;}				\
		else if (IS_IN_RETURNING() || IS_IN_THROWING()) break;		\
	}																\
} if (IS_IN_RETURNING() || IS_IN_THROWING()) break;


///////////////// FUNCTIONS: ///////////////////////////////
#define FUN_DECL(function_name, ...) int function_name(__VA_ARGS__)

#define FUN_IMPL(function_name, ...)			\
int function_name(__VA_ARGS__)					\
{												\
int __RET_VAL__ = SUCCESS_VALUE;				\
int IS_BREAKING = false;						\
int IS_CONTINUING = false;					    \
SCOPE_START;


#define END_FUN												\
	SCOPE_END												\
	_scope_obj_list_call_dtors(&_scope_obj_list);			\
	if (IS_IN_RETURNING()) __RET_VAL__ = SUCCESS_VALUE;		\
	return __RET_VAL__;										\
}

#define FUN(funcName)\
{ int __INNER_FUNC_CALL_RET_VALUE__ = (funcName)(

#define CALL ); if (IN_THROWING_VALUE == __INNER_FUNC_CALL_RET_VALUE__) {THROW;} } 


///////////////// EXCEPTION UTILS: /////////////////////////////// Utilities
#define ASSERT(x) if (!(x)) {THROW;}
#define THROW_MSG_UNLESS(x, msg) if (!(x)) {THROW_MSG(msg);}
#define ASSERT_NOT_NULL(x) if (!(x)) {THROW_MSG(#x " is null");}

#endif

