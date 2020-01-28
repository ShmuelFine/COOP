#include "HeapCache.h"
#include <stdlib.h>


DEF_DERIVED_CTOR(HeapCache, iCache) SUPER ME
END_DERIVED_CTOR

DEF_DERIVED_DTOR(HeapCache, iCache)
END_DERIVED_DTOR


FUNCTION_IMPL(HeapCache, AddNewBlock, int block_size, void** returned)
{
	*returned = (char*)malloc(sizeof(char) * block_size);
}
END_FUNCTION_IMPL

FUNCTION_IMPL(HeapCache, RemoveBlock, void* toDelete)
{
	free(toDelete);
}
END_FUNCTION_IMPL

INIT_DERIVED_CLASS(HeapCache, iCache);
BIND_OVERIDE(HeapCache, iCache, AddNewBlock);
BIND_OVERIDE(HeapCache, iCache, RemoveBlock);
END_INIT_CLASS




//#include "HeapCache.h"
//
//DEF_DERIVED_CTOR(HeapCache, iCache, int size) SUPER ME
//{
//	HeapCache_InitCache(_this);
//	HeapCache_AllocateCache(_this, size);
//}
//END_DERIVED_CTOR
//
//DEF_DERIVED_DTOR(HeapCache, iCache)
//{
//	HeapCache_Destroy(_this);
//}
//END_DERIVED_DTOR
//
//FUNCTION_IMPL(HeapCache, AddNewBlock, int blockSize, Block** returned)
//{
//
//}
//END_FUNCTION_IMPL
//
//void HeapCache_InitCache(HeapCache* c)
//{
//	c->size = 0;
//	c->buffer = NULL;
//	c->nextFreeBlock = 0;
//}
//
//void HeapCache_AllocateCache(HeapCache* c, int newSize)
//{
//	if (c->size == newSize)
//		return;
//
//	HeapCache_AllocateCacheFromExisingBuf(c, (char*)malloc(sizeof(char) * newSize), newSize);
//}
//
//void HeapCache_AllocateCacheFromExisingBuf(HeapCache* c, char* cacheMemroy, int newSize)
//{
//	HeapCache_Destroy(c);
//
//	c->buffer = cacheMemroy;
//	c->size = newSize;
//
//	c->nextFreeBlock = 0;
//}
//
//bool HeapCache_isEmpty(HeapCache* c)
//{
//	return c->size == 0;
//}
//
//void HeapCache_Destroy(HeapCache* c)
//{
//	if (!HeapCache_isEmpty(c))
//	{
//		free(c->buffer);
//	}
//}
//
//int HeapCache_GetRight(HeapCache &c,int index)
