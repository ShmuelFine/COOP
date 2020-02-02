#ifndef __HEAP_CACHE__H_
#define __HEAP_CACHE__H_

#include "object.h"
#include "iCache.h"

DEF_DERIVED_CLASS(HeapCache, iCache);
END_DEF_DERIVED(HeapCache);

DERIVED_FUNCTIONS(HeapCache, iCache)
FUN_OVERRIDE(iCache, AddNewBlock, int block_size, void** returned);
FUN_OVERRIDE(iCache, RemoveBlock, void* toDelete);
END_DERIVED_FUNCTIONS(HeapCache);

//#ifndef __I_CACHE__H_
//#include "iCache.h"
//#endif
//
//#define MAX_NUM_BLOCKS 1000
//
//DEF_DERIVED_CLASS(HeapCache, iCache) ;
//char* buffer;
//int size;
//Block allBlocks[MAX_NUM_BLOCKS];
//int nextFreeBlock;
//END_DEF_DERIVED(HeapCache);
//
//DERIVED_FUNCTIONS(HeapCache, iCache, int size)
//FUN_OVERRIDE(iCache, AddNewBlock, int blockSize, Block** returned);
//FUN_OVERRIDE(iCache, RemoveBlock, Block* toDelete);
//END_FUNCTIONS(HeapCache);
//
//void HeapCache_InitCache(HeapCache* c);
//
//void HeapCache_AllocateCache(HeapCache* c, int newSize);
//
//void HeapCache_AllocateCacheFromExisingBuf(HeapCache* c, char* cacheMemroy, int newSize);
//
//bool HeapCache_isEmpty(HeapCache* c);
//
//void HeapCache_Destroy(HeapCache* c);

#endif

