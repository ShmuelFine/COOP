#ifndef __HEAP_CACHE__H_
#define __HEAP_CACHE__H_

#include "object.h"
#include "iCache.h"

DEF_DERIVED_CLASS(HeapCache, iCache);
END_DEF_DERIVED(HeapCache);

DERIVED_FUNCTIONS(HeapCache, iCache)
FUN_OVERRIDE(iCache, AddNewBlock, int block_size, void** returned);
FUN_OVERRIDE(iCache, RemoveBlock, void* toDelete);
END_DERIVED_FUNCTIONS(HeapCache);

#endif

