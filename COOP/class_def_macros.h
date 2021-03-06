#ifndef __COOP_CLASS_DEF_MACROS__
#define __COOP_CLASS_DEF_MACROS__

#include "virtual_func_calling.h"
#include "obj_lifecycle_management.h"
#include <stdbool.h>
#include <string.h>
#include "ExportDefs.h"

///////////// The H file structure : //////////////////////////////////
// Macro that begins class name + member definitions:
#define DEF_CLASS(class_name)                                              \
typedef struct class_name ##VirtualTable_t class_name ##VirtualTable;            \
typedef struct class_name ##_t{                                            \
	object *_next;                                                   \
	class_name ##VirtualTable* vTable

// Define your members here, between DEF_CLASS and END_DEF

// Macro that ends class member definitions:
#define END_DEF(class_name)												\
}class_name;																\
COOP_API extern bool is_ ##class_name ##VirtualTable__initialized

// Macro that begins class function definitions section:
#define FUNCTIONS(class_name, ...)							\
	int __ctor__ ##class_name(class_name * _this, __VA_ARGS__);	\
	int __dtor__ ##class_name(class_name * _this);					\
	typedef struct class_name ##VirtualTable_t{				\
	int (*_ctor)(class_name * _this, __VA_ARGS__);			\
	int (*_dtor)(class_name * _this) 

// Define your members here, between FUNCTIONS and END_FUNCTIONS,
// using MEM_FUN_DECL.
#define MEM_FUN_DECL(type, function_name, ...)\
	struct type ##_ ##function_name ##_t_{								\
		int (* inner_function)(void* _this, __VA_ARGS__);	\
		int (*(* outer_function)(void* _this))(void* _this, __VA_ARGS__);	\
		struct type ##_ ##function_name ##_t_ * next;					\
	}function_name


// Macro that ends class function definitions section:
#define END_FUNCTIONS(class_name) } class_name ##VirtualTalbe;   \
COOP_API extern class_name ##VirtualTalbe class_name ##VTable;\
COOP_API void class_name ##_init()

///////////// The C file structure : //////////////////////////////////

// Macros that defines a constructor:
#define DEF_CTOR(class_name, ...) FUN_IMPL(__ctor__ ##class_name, class_name * _this, __VA_ARGS__)
#define END_CTOR END_FUN

// Macros that defines a destructor:
#define DEF_DTOR(class_name) FUN_IMPL(__dtor__ ##class_name, class_name * _this)

#define END_DTOR END_FUN


// Macros that define implementation of previously declared function:
#define MEM_FUN_IMPL(type, function_name, ...)									\
int (*type ##_ ##function_name ##_outer_function(type * _this))(void* _this, __VA_ARGS__)\
{																		\
	struct type ##_ ##function_name ##_t_ * toRun = &_this->vTable->function_name;			\
	while(toRun->next) { toRun = toRun->next; }							\
	return toRun->inner_function;										\
}																		\
																		\
FUN_IMPL(inner_function_ ##type ##_ ##function_name, type * _this, __VA_ARGS__)
		
// Macro for inner use in INIT_CLASS:
#define ATTACH_TORs_ToClass(class_name)       \
class_name ##VTable._ctor = __ctor__ ##class_name;  \
class_name ##VTable._dtor = __dtor__ ##class_name; 

// At the end of the C file, we bind the function implementations to the pointers of the vTable.
// It begins with:
#define INIT_CLASS(type)							\
bool is_ ##type ##VirtualTable__initialized = false;\
type ##VirtualTable type ##VTable;					\
	void type ##_init(){							\
	/*For safety, set all ptrs to NULL instead of garbage: */\
	memset(& type ##VTable, sizeof(type ##VTable), 0); \
	ATTACH_TORs_ToClass(type)


#define BIND(type, function_name)\
	type ##VTable.function_name.outer_function = type ##_ ##function_name ##_outer_function;				\
	type ##VTable.function_name.inner_function = inner_function_ ##type ##_ ##function_name;				\
	type ##VTable.function_name.next = NULL

// Finally, it ends with:
#define END_INIT_CLASS } 

//////////////////////////////////////////////////////////////////////


#endif
