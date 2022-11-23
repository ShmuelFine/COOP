#ifndef __GLOBALS__H_
#define __GLOBALS__H_

#include "ExportDefs.h"
#include "coop.h"
#include "iCache.h"
#include <stdbool.h>


COOP_API extern iCache* TheGlobalCache;

typedef enum CACHE_TYPES_t { STACK_BASED_MEMORY, HEAP_BASED_MEMORY, NUM_MEMORY_TYPES } CACHE_TYPES;


FUN_DECL(init_global_memory, int size, CACHE_TYPES type);


#define NEW(dest,whatToPutThere)\
	{\
		FUN(TheGlobalCache, AddNewBlock),(sizeof(whatToPutThere)),(void*)&(dest) CALL\
	}

#define NEW_OF_SIZE(dest,type,howMuchToPutThere)\
	{\
		FUN(TheGlobalCache, AddNewBlock),(sizeof(type)*howMuchToPutThere),(void*)&(dest) CALL\
	}


#define DELETE_OBJ(buff) FUN(TheGlobalCache, RemoveBlock), buff CALL buff = NULL


#endif


