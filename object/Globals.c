
#include "Globals.h"
#include "Cache.h"
#include "HeapCache.h"


iCache * TheGlobalCache;
#define MAX_CACHE_SIZE (sizeof(InMemoryCache))
char BuffForCacheObject[MAX_CACHE_SIZE];


#define CREATE_GLOBAL_CACHE(type,name, ...)				                     \
	if (! is_ ##type ##VirtualTable__initialized) type ##_init();            \
	type * name = (type *)BuffForCacheObject;					     \
	name->vTable = &type ##VTable;									 \
	name->vTable->_ctor((type *)name, __VA_ARGS__);            \
	TheGlobalCache = (iCache*)BuffForCacheObject

void CreateGlobalCache(int size,const char * name, CACHE_TYPES type)
{
	switch (type)
	{
	//case IN_MEMORY_CACHE_: CREATE_GLOBAL_CACHE(InMemoryCache,name, size); break;
	//case HEAP_CACHE_: CREATE_GLOBAL_CACHE(HeapCache,name,size); break;
	default: CREATE_GLOBAL_CACHE(InMemoryCache, name, size); break;
	}
}
