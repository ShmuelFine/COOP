#ifndef __GLOBALS__H_
#define __GLOBALS__H_

#include "ExportDefs.h"
#include "iCache.h"
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif

COOP_API extern iCache* TheGlobalCache;

typedef enum CACHE_TYPES_t { IN_MEMORY_CACHE_ = 0, HEAP_CACHE_ = 1, NUM_CACHE_TYPES } CACHE_TYPES;

COOP_API bool CreateGlobalCache(int size, CACHE_TYPES type);
COOP_API void DestroyGlobalCache();

#define NEW(dest,whatToPutThere)\
	{\
		FUN(TheGlobalCache, AddNewBlock),(sizeof(whatToPutThere)),(void*)&dest CALL\
	}

	#define DELETE_OBJ(buff) FUN(TheGlobalCache, RemoveBlock), buff CALL buff = NULL

#ifdef __cplusplus
}
#endif

#endif


