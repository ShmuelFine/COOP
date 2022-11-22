
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
	case STACK_BASED_MEMORY: { CREATE_GLOBAL_CACHE(InMemoryCache, size); } break;
	case HEAP_BASED_MEMORY: {CREATE_GLOBAL_CACHE(HeapCache); } break;
	
	default: return false;
	}

	return true;
}

void DestroyGlobalCache()
{
	TheGlobalCache->vTable->_dtor(TheGlobalCache);
}

///////////////

DEF_CTOR(MemoryManager, int size, CACHE_TYPES type)
{
	CreateGlobalCache(size, type);
}
END_CTOR

DEF_DTOR(MemoryManager)
{
	DestroyGlobalCache();
}
END_DTOR


INIT_CLASS(MemoryManager)
END_INIT_CLASS(MemoryManager)

