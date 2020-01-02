#pragma once
#include <stdbool.h>

#include "ExportDefs.h"

typedef struct block_s {
	char* buff;
	int size;
	struct block_s* next;
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

#ifdef __cplusplus
extern "C" {
#endif
	COOP_API void Cache_Init(Cache* c);
	COOP_API void Cache_Destroy(Cache* c);
	COOP_API void Cache_AllocateCache(Cache* c, int newSize);
	COOP_API void Cache_AllocateCacheFromExisingBuf(Cache* c, char* cacheMemroy, int newSize);


	COOP_API Block* Cache_getAvailableBlock(Cache* c);
	//COOP_API Block* Cache_FindBlockByName(Cache* c, const char* name);
	//COOP_API Block* Cache_Fetch(Cache* c, const char* block_name, int block_size);
	//COOP_API Block* Cache_Fetch_Assert(Cache* c, const char* block_name, int block_size);
	COOP_API void Cache_DeleteBlock(Cache* c, Block* toDelete);
	COOP_API Block* Cache_FindBlockByBuffAddress(Cache *c, void* buff);
	COOP_API Block* Cache_AddNewBlock(Cache* c, int block_size);
	COOP_API void Cache_RemoveBlock(Cache* c, Block* toDelete);
	//COOP_API void Cache_RemoveBlockByName(Cache* c, const char* block_name);
	COOP_API unsigned long Cache_GetAllocAmount(Cache* c);

#ifdef __cplusplus
}
#endif