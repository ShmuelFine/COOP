
#include "Cache.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//void Cache_Init(Cache* c)
//{
//	c->size = 0;
//	c->buffer = NULL;
//	for (int i = 2; i < MAX_NUM_BLOCKS; i++)
//	{
//		c->IsBlockUsed[i]=false;
//	}
//}
//
//bool Cache_isEmpty(Cache* c)
//{
//	return c->size == 0;
//}
//
///*recursive cache free*/
////void recursive_free_blocks(Block* Cache_block_node)
////{
////	if (Cache_block_node->next)
////		recursive_free_blocks(Cache_block_node->next);
////
////	//free(Cache_block_node);
////	Cache_block_node = NULL;
////}
//
//void Cache_Destroy(Cache* c)
//{
//	if (!Cache_isEmpty(c))
//	{
//		free(c->buffer);
//		//recursive_free_blocks(c->allBlockPointers);
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
//Block* Cache_allocateBlock(Cache *c, int block_size, char* pos_in_Cache_buff)
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
//	//Cache_Init(c);
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
//
//	//Block* endOfList = Cache_allocateBlock("__END__OF__LIST", 0, c->buffer + c->size);
//	//c->allBlocks = Cache_allocateBlock("__START__OF__LIST", 0, c->buffer);
//	//c->allBlocks->next = endOfList;
//
//
//	//c->allBlocks->isSealed = true;
//	//endOfList->isSealed = true;
//
//}
//
//void Cache_AllocateCache(Cache* c, int newSize)
//{
//	if (c->size == newSize)
//		return;
//	
//	Cache_AllocateCacheFromExisingBuf(c, (char *)malloc(sizeof(char) * newSize), newSize);
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
////Block* Cache_FindBlockByID(Cache* c, void * buff_id)
////{
////	for (Block* it = c->allBlockPointers; it->next != NULL; it = it->next)
////	{
////		if (it->id == buff_id)
////			return it;
////	}
////	return NULL;
////}
//
////void Cache_FreeInactiveBlocks(Cache* c)
////{
////	for (Block* it = c->allBlocks; it->next != NULL; it = it->next)
////	{
////		while (!it->isActive)
////		{
////			Block* next = it->next;
////			Cache_DeleteBlock(c, it);
////		}
////	}
////	
////}
//
//Block* Cache_AddNewBlock(Cache* c, int block_size)
//{
//	Block* lowerBound = Cache_FindAvailableInterval(c, block_size);
//	if (!lowerBound)
//		return NULL;
//
//
//	char* block_buff_pos = lowerBound->buff + lowerBound->size;
//
//	Block* newBlock = Cache_allocateBlock(c, block_size, block_buff_pos);
//	if (!newBlock)
//		return NULL;
//	newBlock->next = lowerBound->next;
//	lowerBound->next = newBlock;
//
//	return newBlock;
//}
//
//void Cache_RemoveBlock(Cache* c, Block* toDelete)
//{
//		Cache_DeleteBlock(c, toDelete);
//}
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
//Block* Cache_FindBlockByBuffAddress(Cache *c, void* buff)
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
///*functions using block names*/
////void Cache_RemoveBlockByName(Cache* c, const char* block_name)
////{
////	Block* toDelete = Cache_FindBlockByName(c, block_name);
////	if (toDelete)
////		Cache_RemoveBlock(c, toDelete);
////}
//
////void Cache_DeleteBlockByName(Cache* c, const char* block_name)
////{
////	Block* toDelete = Cache_FindBlockByName(c, block_name);
////	if (toDelete)
////		Cache_DeleteBlock(c, toDelete);
////}
//
////Block* Cache_Fetch(Cache* c, void* buff_id, int block_size)
////{
////	bool wasExisting = false;
////	Block* existing = Cache_FindBlockByID(c, buff_id);
////	if (existing && existing->size != block_size)
////	{
////		wasExisting = true;
////		Cache_DeleteBlockByName(c, existing->name);
////		Block* UL = Cache_AddNewBlock(c, "upperLimitToAvoidDoubleAllocInSamePlace", 0);
////		existing = NULL;
////	}
////	if (!existing)
////	{
////		existing = Cache_AddNewBlock(c, block_name, block_size);
////		if(wasExisting)
////			Cache_DeleteBlockByName(c, "upperLimitToAvoidDoubleAllocInSamePlace");
////	}
////
////	// In case we've just fetched an existing, inactive, block:
////	if (existing)
////		existing->isActive = true;
////
////	return existing;
////}
//
////Block* Cache_Fetch_Assert(Cache* c, const char* block_name, int block_size)
////{
////	Block* result = Cache_Fetch(c, block_name, block_size);
////	assert(result != NULL);
////	return result;
////}
//
//unsigned long Cache_GetAllocAmount(Cache* c)
//{
//	unsigned long sum = 0;
//	for (Block* it = c->allBlockPointers; it->next != NULL; it = it->next)
//	{
//			sum += (unsigned long)it->size;
//	}
//
//	return sum;
//}
//
//void external_symbol()
//{
//	int t = 5;
//}

DEF_DERIVED_CTOR(InMemoryCache,iCache, int size) SUPER ME
{
	Cache_InitCache(_this);
	Cache_AllocateCache(_this, size);
}
END_DERIVED_CTOR

DEF_DERIVED_DTOR(InMemoryCache,iCache)
{
	Cache_Destroy(_this);
}
END_DERIVED_DTOR

INIT_DERIVED_CLASS(InMemoryCache, iCache);
BIND_OVERIDE(InMemoryCache,iCache, AddNewBlock);
BIND_OVERIDE(InMemoryCache,iCache, RemoveBlock);
END_INIT_CLASS

FUNCTION_IMPL(InMemoryCache,AddNewBlock,int block_size,void ** returned)
		{
	SCOPE_START;
			*returned = NULL;
			Block* lowerBound = Cache_FindAvailableInterval(_this, block_size);
			if (lowerBound)
			{
				char* block_buff_pos = lowerBound->buff + lowerBound->size;

				Block* newBlock = Cache_allocateBlock(_this, block_size, block_buff_pos);
				if (newBlock)
				{
					newBlock->next = lowerBound->next;
					lowerBound->next = newBlock;

					*returned = newBlock->buff;
				}
			}
			SCOPE_END;
		}
END_FUNCTION_IMPL

FUNCTION_IMPL(InMemoryCache, RemoveBlock,void * toDelete)
{
	SCOPE_START;
	Block *block = Cache_FindBlockByBuffAddress(_this, toDelete);
	Cache_DeleteBlock(_this, block);
	SCOPE_END;
}
END_FUNCTION_IMPL

void Cache_InitCache(InMemoryCache* c)
{
	c->size = 0;
	c->buffer = NULL;
	for (int i = 2; i < MAX_NUM_BLOCKS; i++)
	{
		c->IsBlockUsed[i] = false;
	}
}

bool Cache_isEmpty(InMemoryCache* c)
{
	return c->size == 0;
}


void Cache_Destroy(InMemoryCache* c)
{
	if (!Cache_isEmpty(c))
	{
		free(c->buffer);
		//recursive_free_blocks(c->allBlockPointers);
	}
}

Block* Cache_getAvailableBlock(InMemoryCache* c)
{
	if (c->numBlocks == MAX_NUM_BLOCKS)
		return NULL;


	Block* tempBlock = &(c->allBlocks[c->nextFreeBlock]);
	c->IsBlockUsed[c->nextFreeBlock] = true;
	c->numBlocks++;

	if (MAX_NUM_BLOCKS == c->nextFreeBlock)
		c->nextFreeBlock = 0;

	while (c->nextFreeBlock < MAX_NUM_BLOCKS && c->IsBlockUsed[c->nextFreeBlock] == true)
		c->nextFreeBlock++;

	return tempBlock;
}

Block* Cache_allocateBlock(InMemoryCache* c, int block_size, char* pos_in_Cache_buff)
{
	Block* tempBlock = Cache_getAvailableBlock(c);
	if (!tempBlock)
		return NULL;
	tempBlock->size = block_size;
	tempBlock->buff = pos_in_Cache_buff;
	tempBlock->next = NULL;
	return tempBlock;
}


void Cache_AllocateCacheFromExisingBuf(InMemoryCache* c, char* cacheMemroy, int newSize)
{
	Cache_Destroy(c);
	//Cache_Init(c);

	c->buffer = cacheMemroy;
	c->size = newSize;

	c->allBlocks[0].buff = c->buffer;
	c->allBlocks[0].size = 0;
	c->IsBlockUsed[0] = true;

	c->allBlocks[1].buff = c->buffer + c->size;
	c->allBlocks[1].size = 0;
	c->allBlocks[1].next = NULL;
	c->IsBlockUsed[1] = true;

	c->allBlockPointers = &(c->allBlocks[0]);
	c->allBlockPointers->next = &(c->allBlocks[1]);
	c->nextFreeBlock = 2;
	c->numBlocks = 2;

	//Block* endOfList = Cache_allocateBlock("__END__OF__LIST", 0, c->buffer + c->size);
	//c->allBlocks = Cache_allocateBlock("__START__OF__LIST", 0, c->buffer);
	//c->allBlocks->next = endOfList;


	//c->allBlocks->isSealed = true;
	//endOfList->isSealed = true;

}

void Cache_AllocateCache(InMemoryCache* c, int newSize)
{
	if (c->size == newSize)
		return;

	Cache_AllocateCacheFromExisingBuf(c, (char*)malloc(sizeof(char) * newSize), newSize);
}


Block* Cache_FindAvailableInterval(InMemoryCache* c, int dstSizeInBytes)
{
	for (Block* it = c->allBlockPointers; it->next != NULL; it = it->next)
	{
		if (it->next->buff - (it->buff + it->size) >= dstSizeInBytes)
			return it;
	}

	return NULL;
}

Block* _Cache_AddNewBlock(InMemoryCache* c, int size)
{
	Block* lowerBound = Cache_FindAvailableInterval(c, size);
	if (!lowerBound)
		return NULL;

	char* block_buff_pos = lowerBound->buff + lowerBound->size;

	Block* newBlock = Cache_allocateBlock(c, size, block_buff_pos);
	if (!newBlock)
		return NULL;
	newBlock->next = lowerBound->next;
	lowerBound->next = newBlock;

	return newBlock;
}


//Block* Cache_AddNewBlock(InMemoryCache* c, int block_size)
//{
//	Block* lowerBound = Cache_FindAvailableInterval(c, block_size);
//	if (!lowerBound)
//		return NULL;
//
//
//	char* block_buff_pos = lowerBound->buff + lowerBound->size;
//
//	Block* newBlock = Cache_allocateBlock(c, block_size, block_buff_pos);
//	if (!newBlock)
//		return NULL;
//	newBlock->next = lowerBound->next;
//	lowerBound->next = newBlock;
//
//	return newBlock;
//}

//void Cache_RemoveBlock(InMemoryCache* c, Block* toDelete)
//{
//	Cache_DeleteBlock(c, toDelete);
//}

void Cache_DeleteBlock(InMemoryCache* c, Block* toDelete)
{
	if (!toDelete)
		return;

	Block* prev = NULL;
	for (Block* it = c->allBlocks; it->next != NULL; it = it->next)
	{
		if (it->next == toDelete)
		{
			toDelete = it->next;
			prev = it;
			break;
		}
	}

	if (prev)
		prev->next = toDelete->next;

	int myIdx = (int)((char*)toDelete - (char*)c->allBlocks) / sizeof(Block);
	c->IsBlockUsed[myIdx] = false;
	c->numBlocks--;
}

Block* Cache_FindBlockByBuffAddress(InMemoryCache* c, void* buff)
{
	Block* it = c->allBlockPointers->next;
	while (it->next)
	{
		if (it->buff == buff)
			return it;
		it = it->next;
	}
	return NULL;
}



unsigned long Cache_GetAllocAmount(InMemoryCache* c)
{
	unsigned long sum = 0;
	for (Block* it = c->allBlockPointers; it->next != NULL; it = it->next)
	{
		sum += (unsigned long)it->size;
	}

	return sum;
}

void external_symbol()
{
	int t = 5;
}
