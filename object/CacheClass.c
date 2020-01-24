//#include "CacheClass.h"
//
//DEF_CTOR(Cache, int size)
//{
//	Cache_InitCache(_this);
//	Cache_AllocateCache(_this, size);
//}
//END_CTOR
//
//DEF_DTOR(Cache)
//{
//	Cache_Destroy(_this);
//}
//END_DTOR
//
//FUNCTION_IMPL(Cache, AddNewBlock, int block_size, Block* returned)
//{
//	Block* lowerBound = Cache_FindAvailableInterval(_this, block_size);
//	if (!lowerBound)
//	{
//		returned = NULL;
//		return;
//	}
//
//	char* block_buff_pos = lowerBound->buff + lowerBound->size;
//
//	Block* newBlock = Cache_allocateBlock(_this, block_size, block_buff_pos);
//	if (!newBlock)
//	{
//		returned = NULL;
//		return;
//	}
//	newBlock->next = lowerBound->next;
//	lowerBound->next = newBlock;
//
//	returned = newBlock;
//}
//END_FUNCTION_IMPL
//
//FUNCTION_IMPL(Cache, RemoveBlock, Block* toDelete)
//{
//	Cache_DeleteBlock(_this, toDelete);
//}
//END_FUNCTION_IMPL
//
//
//INIT_CLASS(Cache)
//BASE_BIND(Cache, AddNewBlock);
//BASE_BIND(Cache, RemoveBlock);
//END_INIT_CLASS
//
//void Cache_InitCache(Cache* c)
//{
//	c->size = 0;
//	c->buffer = NULL;
//	for (int i = 2; i < MAX_NUM_BLOCKS; i++)
//	{
//		c->IsBlockUsed[i] = false;
//	}
//}
//
//bool Cache_isEmpty(Cache* c)
//{
//	return c->size == 0;
//}
//
//
//
//void Cache_Destroy(Cache* c)
//{
//	if (!Cache_isEmpty(c))
//	{
//		free(c->buffer);
//	}
//}
//
//Block* Cache_getAvailableBlock(Cache* c)
//{
//	if (c->nextFreeBlock == MAX_NUM_BLOCKS)
//		return NULL;
//
//	Block* tempBlock = &(c->allBlocks[c->nextFreeBlock]);
//	c->IsBlockUsed[c->nextFreeBlock] = true;
//
//	while (c->nextFreeBlock < MAX_NUM_BLOCKS && c->IsBlockUsed[c->nextFreeBlock] == true)
//		c->nextFreeBlock++;
//
//	return tempBlock;
//}
//
//Block* Cache_allocateBlock(Cache* c, int block_size, char* pos_in_Cache_buff)
//{
//	Block* tempBlock = Cache_getAvailableBlock(c);
//	if (!tempBlock)
//		return NULL;
//	tempBlock->size = block_size;
//	tempBlock->buff = pos_in_Cache_buff;
//	tempBlock->next = NULL;
//	return tempBlock;
//}
//
//
//void Cache_AllocateCacheFromExisingBuf(Cache* c, char* cacheMemroy, int newSize)
//{
//	Cache_Destroy(c);
//
//	c->buffer = cacheMemroy;
//	c->size = newSize;
//
//	c->allBlocks[0].buff = c->buffer;
//	c->allBlocks[0].size = 0;
//	c->IsBlockUsed[0] = true;
//
//	c->allBlocks[1].buff = c->buffer + c->size;
//	c->allBlocks[1].size = 0;
//	c->allBlocks[1].next = NULL;
//	c->IsBlockUsed[1] = true;
//
//	c->allBlockPointers = &(c->allBlocks[0]);
//	c->allBlockPointers->next = &(c->allBlocks[1]);
//	c->nextFreeBlock = 2;
//}
//
//void Cache_AllocateCache(Cache* c, int newSize)
//{
//	if (c->size == newSize)
//		return;
//
//	Cache_AllocateCacheFromExisingBuf(c, (char*)malloc(sizeof(char) * newSize), newSize);
//}
//
//
//Block* Cache_FindAvailableInterval(Cache* c, int dstSizeInBytes)
//{
//	for (Block* it = c->allBlockPointers; it->next != NULL; it = it->next)
//	{
//		if (it->next->buff - (it->buff + it->size) >= dstSizeInBytes)
//			return it;
//	}
//
//	return NULL;
//}
//
//
//
//void Cache_DeleteBlock(Cache* c, Block* toDelete)
//{
//	if (!toDelete)
//		return;
//
//	Block* prev = NULL;
//	for (Block* it = c->allBlocks; it->next != NULL; it = it->next)
//	{
//		if (it->next == toDelete)
//		{
//			toDelete = it->next;
//			prev = it;
//			break;
//		}
//	}
//
//	if (prev)
//		prev->next = toDelete->next;
//
//	int myIdx = (int)((char*)toDelete - (char*)c->allBlocks) / sizeof(Block);
//	c->IsBlockUsed[myIdx] = false;
//	if (myIdx < c->nextFreeBlock)
//		c->nextFreeBlock = myIdx;
//
//}
//
//Block* Cache_FindBlockByBuffAddress(Cache* c, void* buff)
//{
//	Block* it = c->allBlockPointers->next;
//	while (it->next)
//	{
//		if (it->buff == buff)
//			return it;
//		it = it->next;
//	}
//	return NULL;
//}
//
//unsigned long Cache_GetAllocAmount(Cache* c)
//{
//	unsigned long sum = 0;
//	for (Block* it = c->allBlockPointers; it->next != NULL; it = it->next)
//	{
//		sum += (unsigned long)it->size;
//	}
//
//	return sum;
//}
//
//void external_symbol()
//{
//	int t = 5;
//}