#ifndef __GLOBALS__H_
#define __GLOBALS__H_


//#ifndef __I_CACHE__H_
#include "iCache.h"
//#endif

//#ifndef __EXPORT_DEFS__H_
#include "ExportDefs.h"
//#endif



#ifdef __cplusplus
extern "C" {
#endif

COOP_API extern iCache* TheGlobalCache;

typedef enum CACHE_TYPES_t { IN_MEMORY_CACHE_ = 0, HEAP_CACHE_ = 1, NUM_CACHE_TYPES } CACHE_TYPES;

COOP_API void CreateGlobalCache(int size,const char *  name, CACHE_TYPES type);
COOP_API void DestroyGlobalCache();


#ifdef __cplusplus
}
#endif
//COOP_API extern Cache TheGlobalConsistantMatsMetadata;
//
//
#endif


