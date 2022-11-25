#ifndef __HEAP_CACHE__H_
#define __HEAP_CACHE__H_

#include "Object.h"
#include "ICache.h"

DEF_DERIVED_CLASS(HeapCache, ICache);
END_DEF_DERIVED(HeapCache);

DERIVED_FUNCTIONS(HeapCache, ICache);
FUN_OVERRIDE(ICache, AddNewBlock, MEM_SIZE_T block_size, void** returned);
FUN_OVERRIDE(ICache, RemoveBlock, void* toDelete);
END_DERIVED_FUNCTIONS(HeapCache);

#endif

