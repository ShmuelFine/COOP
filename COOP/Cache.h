#ifndef __CACHE__H_
#define __CACHE__H_

#include <stdbool.h>

#include "iCache.h"
#include "object.h"
#include "ExportDefs.h"

DEF_DERIVED_CLASS(InMemoryCache, iCache);
char* buffer;
int size;
END_DEF_DERIVED(InMemoryCache);

DERIVED_FUNCTIONS(InMemoryCache, iCache, int size)
FUN_OVERRIDE(iCache, AddNewBlock, int block_size, void** returned);
FUN_OVERRIDE(iCache, RemoveBlock, void* toDelete);
END_DERIVED_FUNCTIONS(InMemoryCache);


#endif