#ifndef __I_CACHE__H_
#define __I_CACHE__H_

#include "object.h"

typedef int MEM_SIZE_T;

DEF_CLASS(iCache);
END_DEF(iCache);

FUNCTIONS(iCache);
MEM_FUN_DECL(iCache, AddNewBlock, int block_size, void ** returned);
MEM_FUN_DECL(iCache, RemoveBlock, void* toDelete);
END_FUNCTIONS(iCache);

#endif