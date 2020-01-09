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

#define FUNCTION_TYPE(type,function_name,...)        \
struct function_name ##_t_{							 \
	int (*func)(void* _this, __VA_ARGS__);          \
	struct function_name ##_t_ * next;               \
}__ ##function_name,*function_name


COOP_API function* callFunction(function* func);


#define _BASE(base,funcName,...) _this->vTable->_base.__ ##funcName.func((base *)_this,__VA_ARGS__)

#define CALL(funcName,_this,...)     \
{\
struct funcName ##_t_ * f = (struct funcName ##_t_ *)callFunction((function *)(_this).vTable->funcName); \
if(f->func(&(_this),__VA_ARGS__)==-1);\
longjmp(jmp_buffers[curr_buff],1);\
}
#ifdef __cplusplus
}
#endif // __cplusplus

#endif