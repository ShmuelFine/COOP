#ifndef __IN_MEMORY_CACHE__ICACHE_H_
#define __IN_MEMORY_CACHE__ICACHE_H_

#include <stdbool.h>

#include "ICache.h"
#include "Object.h"
#include "ExportDefs.h"

DEF_DERIVED_CLASS(InMemoryCache, ICache);
char* buffer;
MEM_SIZE_T size;
END_DEF_DERIVED(InMemoryCache);

DERIVED_FUNCTIONS(InMemoryCache, ICache, MEM_SIZE_T size);
FUN_OVERRIDE(ICache, AddNewBlock, MEM_SIZE_T block_size, void** returned);
FUN_OVERRIDE(ICache, RemoveBlock, void* toDelete);
FUN_OVERRIDE(ICache, getTotalFreeBytes, MEM_SIZE_T* out_count);
MEM_FUN_DECL(InMemoryCache, print_block, MEM_SIZE_T offset);
MEM_FUN_DECL(InMemoryCache, print_all);
END_DERIVED_FUNCTIONS(InMemoryCache);


#endif