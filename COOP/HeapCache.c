#include "HeapCache.h"
#include <stdlib.h>


DEF_DERIVED_CTOR(HeapCache, iCache) SUPER ME
END_DERIVED_CTOR

DEF_DERIVED_DTOR(HeapCache, iCache)
END_DERIVED_DTOR


FUN_OVERRIDE_IMPL(HeapCache, iCache, AddNewBlock, MEM_SIZE_T block_size, void** returned)
{
	*returned = (char*)malloc(sizeof(char) * block_size);
}
END_FUN

FUN_OVERRIDE_IMPL(HeapCache, iCache, RemoveBlock, void* toDelete)
{
	free(toDelete);
}
END_FUN

INIT_DERIVED_CLASS(HeapCache, iCache);
BIND_OVERIDE(HeapCache, iCache, AddNewBlock);
BIND_OVERIDE(HeapCache, iCache, RemoveBlock);
END_INIT_CLASS(HeapCache)