#include "DynamicMemoryManagement.h"
#include "InMemoryCache.h"
#include "HeapCache.h"
#include <stdlib.h>


ICache* TheGlobalCache = NULL;

///////////////

FUN_IMPL(init_global_memory, int size, CACHE_TYPES type)
{
	if (TheGlobalCache != NULL) {
		printf("Global cache is already initialized. Recreating.\n");
		DESTROY(TheGlobalCache);
		free(TheGlobalCache);
	}

	switch (type)
	{
	case LIMITED_SIZE_MEMORY: {
        TheGlobalCache = (ICache*)malloc(sizeof(InMemoryCache));
		ASSERT_NOT_NULL(TheGlobalCache);
		INITIALIZE_INSTANCE(InMemoryCache, (*((InMemoryCache*)TheGlobalCache))), size CALL;
	} break;
	case HEAP_BASED_MEMORY: {
        TheGlobalCache = (ICache*)malloc(sizeof(HeapCache));
		ASSERT_NOT_NULL(TheGlobalCache);
		INITIALIZE_INSTANCE(HeapCache, (*((HeapCache*)TheGlobalCache))) CALL;
	} break;
	default:
		THROW_MSG("Unknown memory type");
	}
}
END_FUN;

FUN_IMPL(get_total_free_bytes, MEM_SIZE_T* out_count)
{
	MFUN(TheGlobalCache, getTotalFreeBytes), out_count CALL;
}
END_FUN;
