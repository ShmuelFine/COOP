#ifndef __I_CACHE__H_
#define __I_CACHE__H_

#include "object.h"

typedef struct block_s {
	char* buff;
	int size;
	struct block_s* next;
}Block;

DEF_CLASS(iCache);

END_DEF(iCache);

FUNCTIONS(iCache);
MEM_FUN_DECL(iCache, AddNewBlock, int block_size, void ** returned);
MEM_FUN_DECL(iCache, RemoveBlock, void* toDelete);
MEM_FUN_DECL(iCache, GetAmountAllocatedMemory);
END_FUNCTIONS(iCache);

#endif