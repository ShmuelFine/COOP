//
//#ifdef _cplusplus
//extern "C" {
//#endif
//
//#include "object.h"
//
//	#define MAX_NUM_BLOCKS 1000
//
//	DEF_CLASS(Cache)
//	char* buffer;
//	int size;
//	Block allBlocks[MAX_NUM_BLOCKS + 2];
//	Block* allBlockPointers;
//	bool IsBlockUsed[MAX_NUM_BLOCKS + 2];
//	int nextFreeBlock;
//	END_DEF(Cache)
//
//
//	FUNCTIONS(Cache, int size)
//
//		BASE_FUNCTION_PTR(Cache, AddNewBlock, int block_size, Block* returned)
//		BASE_FUNCTION_PTR(Cache, RemoveBlock, Block* toDelete)
//
//	END_FUNCTIONS(Cache)
//
//
//
//	BASE_FUNCTION_H(Cache, AddNewBlock, int block_size, Block* returned);
//	BASE_FUNCTION_H(Cache, RemoveBlock, Block* toDelete);
//
//	//FUNCTION_H(Cache, InitCache);
//	//FUNCTION_H(Cache, Destroy);
//	//FUNCTION_H(Cache, AllocateCache,int size);
//	//FUNCTION_H(Cache, AllocateCacheFromExisingBuf, char* cacheMemroy, int newSize);
//	//
//	//FUNCTION_H(Cache, getAvailableBlock, Block* returned);
//	//FUNCTION_H(Cache, DeleteBlock, Block* toDelete);
//	//FUNCTION_H(Cache, GetAllocAmount, unsigned long * returned);
//	//FUNCTION_H(Cache, FindBlockByBuffAddress, void* buff,Block *returned);
//
//	COOP_API void Cache_InitCache(Cache* c);
//	COOP_API void Cache_Destroy(Cache* c);
//	COOP_API void Cache_AllocateCache(Cache* c, int newSize);
//	COOP_API void Cache_AllocateCacheFromExisingBuf(Cache* c, char* cacheMemroy, int newSize);
//
//
//	COOP_API Block* Cache_getAvailableBlock(Cache* c);
//	COOP_API void Cache_DeleteBlock(Cache* c, Block* toDelete);
//	COOP_API Block* Cache_FindBlockByBuffAddress(Cache* c, void* buff);
//	COOP_API unsigned long Cache_GetAllocAmount(Cache* c);
//	COOP_API Block* Cache_allocateBlock(Cache* c, int block_size, char* pos_in_Cache_buff);
//	COOP_API Block* Cache_FindAvailableInterval(Cache* c, int dstSizeInBytes);
//
//
//	DEF_INIT_CLASS(Cache);
//
//	COOP_API void external_symbol();
//
//#ifdef _cplusplus
//}
//#endif