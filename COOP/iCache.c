

#include "iCache.h"

DEF_CTOR(iCache)
END_CTOR

DEF_DTOR(iCache)
END_DTOR

MEM_FUN_IMPL(iCache, AddNewBlock, int block_size, void** returned)
END_FUN

MEM_FUN_IMPL(iCache, RemoveBlock, void* toDelete)
END_FUN

INIT_CLASS(iCache)
BIND(iCache, AddNewBlock);
BIND(iCache, RemoveBlock);
END_INIT_CLASS(iCache)