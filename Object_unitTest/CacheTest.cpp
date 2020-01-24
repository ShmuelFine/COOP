#include "pch.h"


#ifdef _DEBUG
#pragma comment(lib, "object_d.lib")
#else
#pragma comment(lib, "object.lib")
#endif

//#include "..\object\iCache.h"
#include "..\object\Globals.h"
#include "..\object\CacheTest.h"


class CacheTest : public ::testing::Test
{
public:

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{

	}
};
// The C CALL syntax does not work for C++, here we need full qualification of the inner struct
#define CACHE_CALL(funcName,...)     \
{\
	struct iCacheVirtualTable_t::funcName ##_t_ * f = (struct iCacheVirtualTable_t::funcName ##_t_*)callFunction((function*)(TheGlobalCache)->vTable->funcName);\
	f->func(TheGlobalCache,__VA_ARGS__);\
}

#define CACHE_NEW(obj,typeToAlloc,size){ void * returned; CACHE_CALL(AddNewBlock,(sizeof(typeToAlloc)*size),&returned);\
obj = (typeToAlloc*)returned;}

#define CACHE_DELETE(buff) CACHE_CALL(RemoveBlock,buff); \
buff = NULL

TEST_F(CacheTest, New_WhenThereIsNotEnoughSpace_ThenReturnsNullPointer)
{
	// Arrange
	int CacheSize = 10;
	CreateGlobalCache(CacheSize, "GlobalCache", IN_MEMORY_CACHE_);

	int allocSize = 100;
	void* expected = NULL;



	// Act
	void* actual;
	CACHE_CALL(AddNewBlock, allocSize, &actual);
	DestroyGlobalCache();
	//// Assert
	EXPECT_EQ(expected, actual);
}

TEST_F(CacheTest, Buffer_IfThereIsSpaceInBuffer_ThenAllowsAddingBlockes)
{
	//Arrange
	CreateGlobalCache(100, "GlobalCache", IN_MEMORY_CACHE_);
		//Act
		int* b1, * b2, * b3, * b4;
		CACHE_NEW(b1, int, 10);

		CACHE_NEW(b2, int, 10);
		CACHE_NEW(b3, int, 5);

		CACHE_NEW(b4, int, 5);
		//Assert
		ASSERT_TRUE(NULL == b4);

		CACHE_DELETE(b2);
		CACHE_NEW(b4, int, 5);

		ASSERT_TRUE(NULL != b4);

		DestroyGlobalCache();
}

TEST_F(CacheTest, Create_CreateGlobalCache_CreatesCorrectCache)
{
	ASSERT_TRUE(Create_CreateGlobalCache_CreatesCorrectCache());
}

TEST_F(CacheTest, Destroy_DeatroyCache_DeatroysGlobalCache)
{
	ASSERT_TRUE(Destroy_DeatroyCache_DeatroysGlobalCache());
}

//void EXPECT_BLOCKS_EQ(Block* expected, Block* actual)
//{
//	if (expected == NULL && actual == NULL)
//		return;
//
//	ASSERT_FALSE(
//		(expected == NULL && actual != NULL) ||
//		(expected != NULL && actual == NULL));
//
//	EXPECT_EQ(expected->buff, actual->buff);
//	EXPECT_EQ(expected->next, actual->next);
//	EXPECT_EQ(expected->size, actual->size);
//}
//
//TEST_F(CacheTest, New_WhenThereIsSufficientSpace_ThenReturnsValidPointer)
//{
//	// Arrange
//	int CacheSize = 100;
//	CreateGlobalCache(CacheSize, "GlobalCache", IN_MEMORY_CACHE_);
//
//	int allocSize = 10;
//	Block expected;
//	expected.buff = c.buffer;
//	expected.next = c.allBlocks->next;
//	expected.size = allocSize;
//
//	// Act
//	Block* actual = Cache_AddNewBlock(&c, allocSize);
//
//	// Assert
//	EXPECT_BLOCKS_EQ(&expected, actual);
//}
//
////void EXPECT_BLOCKS_NAMES_ORDER(Cache* c, const char* expected_names[], int namesAmount)
////{
////	Block* curr = c->allBlocks->next;
////	for (int i = 0;
////		i < namesAmount && curr->next != NULL;
////		i++, curr = curr->next)
////	{
////		ASSERT_EQ(std::string(curr->name), std::string(expected_names[i]));
////	}
////}
//
//
//TEST_F(CacheTest, Fetch_WhenInsertingToMiddleOfBuffer_DoesWell)
//{
//	// Arrange
//	int CacheSize = 1000;
//	Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);
//
//	// Act
//	Block * b1 = Cache_AddNewBlock(&c, 10);
//	Block* b2 = Cache_AddNewBlock(&c, 30);
//	Block* b3 = Cache_AddNewBlock(&c, 30);
//	Cache_DeleteBlock(&c, b2);
//	Block* b4 = Cache_AddNewBlock(&c, 10);
//
//	size_t b1idx = b1->buff - c.buffer;
//	size_t b3idx = b3->buff - c.buffer;
//	size_t b4idx = b4->buff - c.buffer;
//
//	//const char* expected_names[] = { "10", "20", "30", "inter_A", "inter_B", "40" };
//
//	// Assert
//	//EXPECT_BLOCKS_NAMES_ORDER(&c, expected_names, sizeof(expected_names) / sizeof(const char*));
//	EXPECT_TRUE(b1idx < b4idx < b3idx);
//}
//
//TEST_F(CacheTest, Fetch_WhenSumOfBlockSizesCoversAllCacheBuffer_ThenActuallyAllocatesThemCorrectly)
//{
//	// Arrange
//	int CacheSize = 10;
//	Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);
//
//	// Act
//	Block* b1 = Cache_AddNewBlock(&c, 2);
//	Block* b2 = Cache_AddNewBlock(&c, 2);
//	Block* b3 = Cache_AddNewBlock(&c, 2);
//	Block* b4 = Cache_AddNewBlock(&c, 2);
//	Block* b5 = Cache_AddNewBlock(&c, 2);
//
//	size_t b1idx = b1->buff - c.buffer;
//	size_t b2idx = b2->buff - c.buffer;
//	size_t b3idx = b3->buff - c.buffer;
//	size_t b4idx = b4->buff - c.buffer;
//	size_t b5idx = b5->buff - c.buffer;
//
//	// Assert
//	EXPECT_TRUE(b1idx < b2idx < b3idx < b4idx < b5idx);
//}
//
//TEST_F(CacheTest, Fetch_WhenSumOfBlockSizesExceedsCacheBuffer_ThenReturnNullOnTheExtra)
//{
//	// Arrange
//	int CacheSize = 11;
//	Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);
//
//	// Act
//	Block* b1 = Cache_AddNewBlock(&c, 2);
//	Block* b2 = Cache_AddNewBlock(&c, 2);
//	Block* b3 = Cache_AddNewBlock(&c, 2);
//	Block* b4 = Cache_AddNewBlock(&c, 2);
//	Block* b5 = Cache_AddNewBlock(&c, 2);
//
//	size_t b1idx = b1->buff - c.buffer;
//	size_t b2idx = b2->buff - c.buffer;
//	size_t b3idx = b3->buff - c.buffer;
//	size_t b4idx = b4->buff - c.buffer;
//	size_t b5idx = b5->buff - c.buffer;
//
//	Block* extraBlock = Cache_AddNewBlock(&c, 2);
//
//
//	// Assert
//	EXPECT_TRUE(b1idx < b2idx < b3idx < b4idx < b5idx);
//	ASSERT_TRUE(extraBlock == NULL);
//}
//
//TEST_F(CacheTest, GetAllocAmount_WhenNoAllocationWereDone_ThenReturnsZero)
//{
//	// Arrange
//	int CacheSize = 10;
//	Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);
//
//	// Act
//	auto sum = Cache_GetAllocAmount(&c);
//
//	// Assert
//	ASSERT_EQ(sum, 0);
//}
//
//TEST_F(CacheTest, GetAllocAmount_WhenSomeAllocationWereDone_ThenReturnsTheirSum)
//{
//	// Arrange
//	int CacheSize = 10;
//	Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);
//
//	// Act
//	Cache_AddNewBlock(&c, 2);
//	Cache_AddNewBlock(&c, 2);
//	auto sum = Cache_GetAllocAmount(&c);
//
//	// Assert
//	ASSERT_EQ(sum, 4);
//}
//
//TEST_F(CacheTest, GetAllocAmount_WhenSomeAllocationAndRemovalsWereDone_ThenReturnsTheirSum)
//{
//	// Arrange
//	int CacheSize = 10;
//	Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);
//
//	// Act
//	Cache_AddNewBlock(&c, 2);
//	auto block_2 = Cache_AddNewBlock(&c, 2);
//	Cache_AddNewBlock(&c, 2);
//	Cache_RemoveBlock(&c, block_2);
//	auto sum = Cache_GetAllocAmount(&c);
//
//	// Assert
//	ASSERT_EQ(sum, 4);
//}
//
//TEST_F(CacheTest, New_WhenNewBlock_NextLocatesNextFreeBlockToFirstAvailablePosition)
//{
//	int CacheSize = 100;
//	Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);
//
//	int posOfBlock = c.nextFreeBlock;
//	auto block = Cache_AddNewBlock(&c, 10);
//
//	ASSERT_EQ(c.nextFreeBlock, posOfBlock + 1);
//}
//
//TEST_F(CacheTest, New_WhenRemovingBlockBeforeNextFreeBlock_NextFreeBlockChangesToPlaceOfRemovedBlock)
//{
//	// Arrange
//	int CacheSize = 100;
//	Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);
//
//	auto block1 = Cache_AddNewBlock(&c, 10);
//	int posOfBlock2 = c.nextFreeBlock;
//	auto block2 = Cache_AddNewBlock(&c, 10);
//	auto block3 = Cache_AddNewBlock(&c, 10);
//
//	Cache_DeleteBlock(&c, block2);
//
//	ASSERT_EQ(c.nextFreeBlock, posOfBlock2);
//}
//
//TEST_F(CacheTest, New_WhenRemovingBlockAfterNextFreeBlock_NextFreeBlockDoesntChange)
//{
//	// Arrange
//	int CacheSize = 100;
//	Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);
//
//	auto block1 = Cache_AddNewBlock(&c, 10);
//	auto block2 = Cache_AddNewBlock(&c, 10);
//	auto block3 = Cache_AddNewBlock(&c, 10);
//	int posOfBlock4 = c.nextFreeBlock;
//	auto block4 = Cache_AddNewBlock(&c, 10);
//	auto block5 = Cache_AddNewBlock(&c, 10);
//
//	Cache_DeleteBlock(&c, block2);
//	Cache_DeleteBlock(&c, block4);
//
//	ASSERT_TRUE(c.nextFreeBlock < posOfBlock4);
//}
//
//TEST_F(CacheTest, NAME_TBD)
//{
//	// Arrange
//	int CacheSize = 1000;
//	Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);
//	// Act
//	for (int i = 0; i < 1000; i++)
//	{
//		Cache_AddNewBlock(&c, 1);
//	}
//
//	//Assert
//	ASSERT_TRUE(NULL == Cache_AddNewBlock(&c, 1));
//}
//
//TEST_F(CacheTest, FindBlockByBuff_DoesWell)
//{
//	// Arrange
//	int CacheSize = 1000;
//	Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);
//	// Act
//	Block* expected = Cache_AddNewBlock(&c, 20);
//	char* buff = expected->buff;
//	Block* actual = Cache_FindBlockByBuffAddress(&c, buff);
//
//	//Assert
//	EXPECT_EQ(expected,actual);
//}
//
//TEST_F(CacheTest, new_function)
//{
//	external_symbol();
//	ASSERT_TRUE(true);
//}