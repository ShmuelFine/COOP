
#include "Globals.h"
#include "Cache.h"
#include "HeapCache.h"


iCache * TheGlobalCache;
#define MAX_CACHE_SIZE (sizeof(InMemoryCache))
char BuffForCacheObject[MAX_CACHE_SIZE];

int global_cache_type;

#define GLOBAL_CACHE_TYPE(type) global_cache_type = (int)type

#define CREATE_GLOBAL_CACHE(type,name, ...)				                     \
	if (! is_ ##type ##VirtualTable__initialized) type ##_init();            \
	type * name = (type *)BuffForCacheObject;					     \
	name->vTable = &type ##VTable;									 \
	name->vTable->_ctor((type *)name, __VA_ARGS__);            \
	TheGlobalCache = (iCache*)BuffForCacheObject

void CreateGlobalCache(int size,const char * name, CACHE_TYPES type)
{
	GLOBAL_CACHE_TYPE(type);
	switch (type)
	{
	case IN_MEMORY_CACHE_: { CREATE_GLOBAL_CACHE(InMemoryCache, name, size); } break;
	case HEAP_CACHE_: {CREATE_GLOBAL_CACHE(HeapCache, name); } break;

	default: return;// CREATE_GLOBAL_CACHE(HeapCache, name, size); break;
	}
}

void DestroyGlobalCache()
{
	switch ((CACHE_TYPES)global_cache_type)
	{
	case IN_MEMORY_CACHE_:  ((InMemoryCache*)TheGlobalCache)->vTable->_dtor((InMemoryCache*)TheGlobalCache);break;
	case HEAP_CACHE_: ((HeapCache*)TheGlobalCache)->vTable->_dtor((HeapCache*)TheGlobalCache); break;
	default: return;
	}
}
