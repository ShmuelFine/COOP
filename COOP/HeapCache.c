#include "HeapCache.h"
#include <stdlib.h>
#include <limits.h>


DEF_DERIVED_CTOR(HeapCache, ICache) SUPER ME
END_DERIVED_CTOR

DEF_DERIVED_DTOR(HeapCache, ICache)
END_DERIVED_DTOR


FUN_OVERRIDE_IMPL(HeapCache, ICache, AddNewBlock, MEM_SIZE_T block_size, void** returned)
{
	*returned = (char*)malloc(sizeof(char) * block_size);
}
END_FUN

FUN_OVERRIDE_IMPL(HeapCache, ICache, RemoveBlock, void* toDelete)
{
	free(toDelete);
}
END_FUN


FUN_OVERRIDE_IMPL(HeapCache, ICache, getTotalFreeBytes, MEM_SIZE_T* out_count)
{
	*out_count = (MEM_SIZE_T)INT_MAX;
}

END_FUN
INIT_DERIVED_CLASS(HeapCache, ICache);
BIND_OVERIDE(HeapCache, ICache, AddNewBlock);
BIND_OVERIDE(HeapCache, ICache, RemoveBlock);
BIND_OVERIDE(HeapCache, ICache, getTotalFreeBytes);
END_INIT_CLASS(HeapCache)