#ifndef __GLOBALS__H_
#define __GLOBALS__H_

#include "ExportDefs.h"
#include "coop.h"
#include "iCache.h"
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif

COOP_API extern iCache* TheGlobalCache;

typedef enum CACHE_TYPES_t { STACK_BASED_MEMORY, HEAP_BASED_MEMORY, NUM_MEMORY_TYPES } CACHE_TYPES;

COOP_API bool CreateGlobalCache(int size, CACHE_TYPES type);
COOP_API void DestroyGlobalCache();

DEF_CLASS(MemoryManager);
END_DEF(MemoryManager);

FUNCTIONS(MemoryManager, int size, CACHE_TYPES type);
END_FUNCTIONS(MemoryManager);


#define NEW(dest,whatToPutThere)\
	{\
		FUN(TheGlobalCache, AddNewBlock),(sizeof(whatToPutThere)),(void*)&dest CALL\
	}

#define NEW_OF_SIZE(dest,type,howMuchToPutThere)\
	{\
		FUN(TheGlobalCache, AddNewBlock),(sizeof(type)*howMuchToPutThere),(void*)&dest CALL\
	}


#define DELETE_OBJ(buff) FUN(TheGlobalCache, RemoveBlock), buff CALL buff = NULL

#ifdef __cplusplus
}
#endif

#endif


