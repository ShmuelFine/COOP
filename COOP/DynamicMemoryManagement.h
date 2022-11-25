#ifndef __DYNAMIC_MEMORY_MANAGEMENT__H_
#define __DYNAMIC_MEMORY_MANAGEMENT__H_

#include "ExportDefs.h"
#include "coop.h"
#include "ICache.h"
#include <stdbool.h>


COOP_API extern ICache* TheGlobalCache;

typedef enum CACHE_TYPES_t { STACK_BASED_MEMORY, HEAP_BASED_MEMORY, NUM_MEMORY_TYPES } CACHE_TYPES;


FUN_DECL(init_global_memory, int size, CACHE_TYPES type);


#define NEW_VARIABLE(dest,whatToPutThere)\
	{\
		FUN(TheGlobalCache, AddNewBlock),(MEM_SIZE_T)(sizeof(whatToPutThere)),(void*)&(dest) CALL\
	}

#define NEW_ARRAY(dest,type,howMuchToPutThere)\
	{\
		FUN(TheGlobalCache, AddNewBlock),(MEM_SIZE_T)(sizeof(type)*(howMuchToPutThere)),(void*)&(dest) CALL\
	}

#define CREATE_PTR(type, instance_name)							\
	type * instance_name = NULL;                  							\
	NEW_VARIABLE(instance_name, type)\
	ASSERT_NOT_NULL(instance_name);	\
	INITIALIZE_INSTANCE(type, (*instance_name))

#define DELETE_OBJ(buff) FUN(TheGlobalCache, RemoveBlock), buff CALL buff = NULL


#endif


