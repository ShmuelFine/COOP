#ifndef __I_CACHE__H_
#define __I_CACHE__H_

#include "Object.h"

typedef unsigned short MEM_SIZE_T;

DEF_CLASS(ICache);
END_DEF(ICache);

FUNCTIONS(ICache);
MEM_FUN_DECL(ICache, AddNewBlock, MEM_SIZE_T block_size, void ** returned);
MEM_FUN_DECL(ICache, RemoveBlock, void* toDelete);
MEM_FUN_DECL(ICache, getTotalFreeBytes, MEM_SIZE_T* out_count);
MEM_FUN_DECL(ICache, reset);
END_FUNCTIONS(ICache);

#endif