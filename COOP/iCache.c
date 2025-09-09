

#include "ICache.h"

DEF_CTOR(ICache)
END_CTOR

DEF_DTOR(ICache)
END_DTOR

MEM_FUN_IMPL(ICache, AddNewBlock, MEM_SIZE_T block_size, void** returned)
END_FUN

MEM_FUN_IMPL(ICache, RemoveBlock, void* toDelete)
END_FUN

MEM_FUN_IMPL(ICache, getTotalFreeBytes, MEM_SIZE_T* out_count)
END_FUN

MEM_FUN_IMPL(ICache, reset)
END_FUN

INIT_CLASS(ICache)
BIND(ICache, AddNewBlock);
BIND(ICache, RemoveBlock);
BIND(ICache, getTotalFreeBytes);
BIND(ICache, reset);
END_INIT_CLASS(ICache)