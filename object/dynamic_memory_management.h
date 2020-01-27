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

COOP_API bool CreateGlobalCache(int size,const char *  name, CACHE_TYPES type);
COOP_API void DestroyGlobalCache();



#ifdef __cplusplus
#define NEW(obj,typeToAlloc,size)\
	{ \
		void * returned; \
		CALL(AddNewBlock,(sizeof(typeToAlloc)*size),&returned);\
		obj = (typeToAlloc*)returned;\
	}
#else
#define NEW(obj,typeToAlloc,size)\
	{\
		void * returned; \
		CALL(AddNewBlock,*TheGlobalCache,(sizeof(typeToAlloc)*size),&returned);\
		obj = (typeToAlloc*)returned;\
	}
#endif

#ifdef __cplusplus
	#define DELETE_OBJ(buff) CALL(RemoveBlock, buff); buff = NULL
#else
	#define DELETE_OBJ(buff) CALL(RemoveBlock, *TheGlobalCache, buff); buff = NULL
#endif

#ifdef __cplusplus
}
#endif

#endif


