#ifndef __LINKED_LISTS__H_
#define __LINKED_LISTS__H_

#include "ExportDefs.h"
#include "object.h"


#ifdef __cplusplus
extern "C" {
#endif

	typedef struct func_t {
		int (*func)(void);
		struct func_t* next;
	}function;

#define FUNCTION_TYPE(type,function_name,...)				\
	struct function_name ##_t_{								\
		int (*func)(void* _this, __VA_ARGS__);				\
		struct function_name ##_t_ * next;					\
	}__ ##function_name,	*function_name

COOP_API function* call_obj_function(function* func);

// Call function of base class:
#define _BASE(base, funcName,...) _this->vTable->_base.__ ##funcName.func((base *)_this,__VA_ARGS__)

// Call any member function: // TODO:Tested?
#ifdef __cplusplus
// The C CALL syntax does not work for C++, here we need full qualification of the inner struct
#define CALL(funcName,...)     \
	{\
		struct iCacheVirtualTable_t::funcName ##_t_ * f = (struct iCacheVirtualTable_t::funcName ##_t_*)call_obj_function((function*)(TheGlobalCache)->vTable->funcName);\
		if(f->func(&(_this),__VA_ARGS__)==-1)\
		longjmp(SCOPE_FALLBACK_ADDR[_CurrScope_Idx--],1);\
	}

#else

#define CALL(funcName,_this,...)     \
	{\
		struct funcName ##_t_ * f = (struct funcName ##_t_ *)call_obj_function((function *)(_this).vTable->funcName); \
		if(f->func(&(_this),__VA_ARGS__)==-1)\
		longjmp(SCOPE_FALLBACK_ADDR[_CurrScope_Idx--],1);\
	}

#endif


#ifdef __cplusplus
}
#endif // __cplusplus

#endif