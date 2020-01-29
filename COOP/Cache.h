#ifndef __CACHE__H_
#define __CACHE__H_

#include <stdbool.h>

#include "iCache.h"
#include "object.h"
#include "ExportDefs.h"


#define MAX_NUM_BLOCKS 100

#ifdef __cplusplus
extern "C" {
#endif

	DEF_DERIVED_CLASS(InMemoryCache, iCache);

	char* buffer;
	int size;
	Block allBlocks[MAX_NUM_BLOCKS + 2];
	Block* allBlockPointers;
	bool IsBlockUsed[MAX_NUM_BLOCKS + 2];
	int nextFreeBlock;
	int numBlocks;
	END_DEF_DERIVED(InMemoryCache);

	DERIVED_FUNCTIONS(InMemoryCache, iCache, int size)
	OVERIDE_FUNCTION_PTR(iCache, AddNewBlock, int block_size, void** returned);
	OVERIDE_FUNCTION_PTR(iCache, RemoveBlock, void* toDelete);
	END_DERIVED_FUNCTIONS(InMemoryCache);

	FUNCTION_H(InMemoryCache, AddNewBlock, int block_size, void** returned);
	FUNCTION_H(InMemoryCache, RemoveBlock, void* toDelete);

	DEF_INIT_CLASS(InMemoryCache);

	// Aux. functions that are not member functions:
	void Cache_InitCache(InMemoryCache* c);
	void Cache_Destroy(InMemoryCache* c);
	void Cache_AllocateCache(InMemoryCache* c, int newSize);
	void Cache_AllocateCacheFromExisingBuf(InMemoryCache* c, char* cacheMemroy, int newSize);

	Block* Cache_getAvailableBlock(InMemoryCache* c);
	void Cache_DeleteBlock(InMemoryCache* c, Block* toDelete);
	Block* Cache_FindBlockByBuffAddress(InMemoryCache* c, void* buff);
	Block* Cache_AddNewBlock(InMemoryCache* c, int block_size);
	void Cache_RemoveBlock(InMemoryCache* c, Block* toDelete);
	unsigned long Cache_GetAllocAmount(InMemoryCache* c);

	void external_symbol();

	Block* Cache_allocateBlock(InMemoryCache* c, int block_size, char* pos_in_Cache_buff);
	Block* Cache_FindAvailableInterval(InMemoryCache* c, int dstSizeInBytes);
	Block* _Cache_AddNewBlock(InMemoryCache* c, int size);

#ifdef __cplusplus
}
#endif

#endif