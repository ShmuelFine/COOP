#include "DynamicMemoryManagement.h"
#include "InMemoryCache.h"
#include "HeapCache.h"
#include <stdlib.h>


ICache* TheGlobalCache = NULL;

///////////////

FUN_IMPL(init_global_memory, int size, CACHE_TYPES type)
{
	if (TheGlobalCache != NULL) {
		printf("Global cache is already initialized. Re-defining.\n");
		DESTROY(TheGlobalCache);
		free(TheGlobalCache);
	}

	switch (type)
	{
	case STACK_BASED_MEMORY: {
		(InMemoryCache*)(TheGlobalCache) = (InMemoryCache*)malloc(sizeof(InMemoryCache));
		ASSERT_NOT_NULL(TheGlobalCache);
		INITIALIZE_INSTANCE(InMemoryCache, (*((InMemoryCache*)TheGlobalCache))), size CALL;
	} break;
	case HEAP_BASED_MEMORY: {
		(HeapCache*)(TheGlobalCache) = (HeapCache*)malloc(sizeof(HeapCache));
		ASSERT_NOT_NULL(TheGlobalCache);
		INITIALIZE_INSTANCE(HeapCache, (*((HeapCache*)TheGlobalCache))) CALL;
	} break;
	default:
		THROW_MSG("Unknown memory type");
	}
}
END_FUN;
