
#include "dynamic_memory_management.h"
#include "Cache.h"
#include "HeapCache.h"


iCache * TheGlobalCache;
#define MAX_CACHE_SIZE (sizeof(InMemoryCache))
char BuffForCacheObject[MAX_CACHE_SIZE];

#define CREATE_GLOBAL_CACHE(type, ...)				                     \
	if (! is_ ##type ##VirtualTable__initialized) type ##_init();            \
	type * __g_cache_##type = (type *)BuffForCacheObject;					     \
	__g_cache_##type->vTable = &type ##VTable;									 \
	__g_cache_##type->vTable->_ctor(__g_cache_##type, __VA_ARGS__);            \
	TheGlobalCache = (iCache*)BuffForCacheObject

bool CreateGlobalCache(int size, CACHE_TYPES type)
{
	switch (type)
	{
	case IN_MEMORY_CACHE_: { CREATE_GLOBAL_CACHE(InMemoryCache, size); } break;
	case HEAP_CACHE_: {CREATE_GLOBAL_CACHE(HeapCache); } break;
	
	default: return false;
	}

	return true;
}

void DestroyGlobalCache()
{
	TheGlobalCache->vTable->_dtor(TheGlobalCache);
}
