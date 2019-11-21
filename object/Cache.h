#pragma once
#include <stdbool.h>

#include "ExportDefs.h"

typedef struct block_s {
	const char* name;
	char* buff;
	int size;
	struct block_s* next;
	bool isSealed;
	bool isActive;
}Block;

#define MAX_NUM_BLOCKS 1000

typedef struct {
	char* buffer;
	int size;
	Block allBlocks[MAX_NUM_BLOCKS + 2];
	Block* allBlockPointers;
	bool IsBlockUsed[MAX_NUM_BLOCKS + 2];
	int nextFreeBlock;
}Cache;

GMED_API void Cache_Init(Cache* c);
GMED_API void Cache_Destroy(Cache* c);
GMED_API void Cache_AllocateCache(Cache* c, int newSize);
GMED_API void Cache_AllocateCacheFromExisingBuf(Cache* c, char * cacheMemroy, int newSize);


GMED_API Block* Cache_getAvailableBlock(Cache* c);
GMED_API Block* Cache_FindBlockByName(Cache* c, const char* name);
GMED_API Block* Cache_Fetch(Cache* c, const char* block_name, int block_size);
GMED_API Block* Cache_Fetch_Assert(Cache* c, const char* block_name, int block_size);
GMED_API void Cache_DeleteBlock(Cache* c, Block* toDelete);
GMED_API void Cache_RemoveBlock(Cache* c, Block* toDelete);
GMED_API void Cache_RemoveBlockByName(Cache* c, const char* block_name);
GMED_API unsigned long Cache_GetAllocAmount(Cache* c);