

#include "iCache.h"



DEF_CTOR(iCache)
END_CTOR

DEF_DTOR(iCache)
END_DTOR

FUNCTION_IMPL(iCache, AddNewBlock, int block_size, void** returned)
END_FUNCTION_IMPL

FUNCTION_IMPL(iCache, RemoveBlock, void* toDelete)
END_FUNCTION_IMPL

INIT_CLASS(iCache)
BASE_BIND(iCache, AddNewBlock);
BASE_BIND(iCache, RemoveBlock);
END_INIT_CLASS