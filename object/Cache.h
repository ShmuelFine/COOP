#ifndef __CACHE__H_
#define __CACHE__H_

#include <stdbool.h>

#include "iCache.h"
#include "object.h"
#include "ExportDefs.h"


#define MAX_NUM_BLOCKS 100
//
//typedef struct {
//	char* buffer;
//	int size;
//	Block allBlocks[MAX_NUM_BLOCKS + 2];
//	Block* allBlockPointers;
//	bool IsBlockUsed[MAX_NUM_BLOCKS + 2];
//	int nextFreeBlock;
//}Cache;
//
//#ifdef __cplusplus
//extern "C" {
//#endif
//	COOP_API void Cache_Init(Cache* c);
//	COOP_API void Cache_Destroy(Cache* c);
//	COOP_API void Cache_AllocateCache(Cache* c, int newSize);
//	COOP_API void Cache_AllocateCacheFromExisingBuf(Cache* c, char* cacheMemroy, int newSize);
//
//
//	COOP_API Block* Cache_getAvailableBlock(Cache* c);
//	//COOP_API Block* Cache_FindBlockByName(Cache* c, const char* name);
//	//COOP_API Block* Cache_Fetch(Cache* c, const char* block_name, int block_size);
//	//COOP_API Block* Cache_Fetch_Assert(Cache* c, const char* block_name, int block_size);
//	COOP_API void Cache_DeleteBlock(Cache* c, Block* toDelete);
//	COOP_API Block* Cache_FindBlockByBuffAddress(Cache *c, void* buff);
//	COOP_API Block* Cache_AddNewBlock(Cache* c, int block_size);
//	COOP_API void Cache_RemoveBlock(Cache* c, Block* toDelete);
//	//COOP_API void Cache_RemoveBlockByName(Cache* c, const char* block_name);
//	COOP_API unsigned long Cache_GetAllocAmount(Cache* c);
//
//	COOP_API void external_symbol();
//
//#ifdef __cplusplus
//}
//#endif
       



#ifdef __cplusplus
extern "C" {
#endif

DEF_DERIVED_CLASS(InMemoryCache,iCache)

char* buffer;
int size;
Block allBlocks[MAX_NUM_BLOCKS + 2];
Block* allBlockPointers;
bool IsBlockUsed[MAX_NUM_BLOCKS + 2];
int nextFreeBlock;
int numBlocks;
END_DEF_DERIVED(InMemoryCache);

DERIVED_FUNCTIONS(InMemoryCache, iCache, int size)
OVERIDE_FUNCTION_PTR(AddNewBlock, int block_size, void** returned);
OVERIDE_FUNCTION_PTR(RemoveBlock, void* toDelete);
END_FUNCTIONS(InMemoryCache);



	FUNCTION_H(InMemoryCache, AddNewBlock, int block_size,void ** returned);
	FUNCTION_H(InMemoryCache, RemoveBlock, void* toDelete);

	COOP_API void Cache_InitCache(InMemoryCache* c);
	COOP_API void Cache_Destroy(InMemoryCache* c);
	COOP_API void Cache_AllocateCache(InMemoryCache* c, int newSize);
	COOP_API void Cache_AllocateCacheFromExisingBuf(InMemoryCache* c, char* cacheMemroy, int newSize);


	COOP_API Block* Cache_getAvailableBlock(InMemoryCache* c);
	COOP_API void Cache_DeleteBlock(InMemoryCache* c, Block* toDelete);
	COOP_API Block* Cache_FindBlockByBuffAddress(InMemoryCache* c, void* buff);
	COOP_API Block* Cache_AddNewBlock(InMemoryCache* c, int block_size);
	COOP_API void Cache_RemoveBlock(InMemoryCache* c, Block* toDelete);
	COOP_API unsigned long Cache_GetAllocAmount(InMemoryCache* c);

	COOP_API void external_symbol();

	DEF_INIT_CLASS(InMemoryCache);

#ifdef __cplusplus
}
#endif

Block* Cache_allocateBlock(InMemoryCache* c, int block_size, char* pos_in_Cache_buff);

Block* Cache_FindAvailableInterval(InMemoryCache* c, int dstSizeInBytes);

Block* _Cache_AddNewBlock(InMemoryCache* c, int size);

#endif