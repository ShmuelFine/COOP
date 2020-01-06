//#include "pch.h"
//
//
//#ifdef _DEBUG
//	#pragma comment(lib, "object_d.lib")
//#else
//	#pragma comment(lib, "object.lib")
//#endif
//
//#include "..\object\Cache.h"
//
//class CacheTest : public ::testing::Test
//{
//public:
//
//	virtual void SetUp()
//	{
//	}
//
//	virtual void TearDown()
//	{
//
//	}
//};
//
//TEST_F(CacheTest, Fetch_WhenThereIsNotEnoughSpace_ThenReturnsNullPointer)
//{
//	// Arrange
//	int CacheSize = 10;
//	Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);
//
//	int allocSize = 100;
//	Block* expected = NULL;
//
//	// Act
//	Block* actual = Cache_Fetch(&c, "asdf", allocSize);
//
//	// Assert
//	EXPECT_EQ(expected, actual);
//}
//
//void EXPECT_BLOCKS_EQ(Block* expected, Block* actual)
//{
//	if (expected == NULL && actual == NULL)
//		return;
//
//	ASSERT_FALSE(
//		(expected == NULL && actual != NULL) ||
//		(expected != NULL && actual == NULL));
//
//	EXPECT_EQ(std::string(expected->name), std::string(actual->name));
//	EXPECT_EQ(expected->buff, actual->buff);
//	EXPECT_EQ(expected->next, actual->next);
//	EXPECT_EQ(expected->size, actual->size);
//}
//
//TEST_F(CacheTest, Fetch_WhenThereIsSufficientSpace_ThenReturnsValidPointer)
//{
//	// Arrange
//	int CacheSize = 100;
//	Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);
//
//	int allocSize = 10;
//	Block expected;
//	expected.name = "test";
//	expected.buff = c.buffer;
//	expected.next = c.allBlocks->next;
//	expected.size = allocSize;
//
//	// Act
//	Block* actual = Cache_Fetch(&c, "test", allocSize);
//
//	// Assert
//	EXPECT_BLOCKS_EQ(&expected, actual);
//}
//
//TEST_F(CacheTest, Fetch_WhenTheBlockExists_ThenReturnsIt)
//{
//	// Arrange
//	int CacheSize = 100;
//	Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);
//
//	int allocSize = 10;
//	Cache_Fetch(&c, "test", allocSize);
//	Block* expected = Cache_Fetch(&c, "ff", allocSize);
//
//	// Act
//	Block* actual = Cache_Fetch(&c, "ff", allocSize);
//	// Assert
//	EXPECT_EQ(expected, actual);
//}
//
//TEST_F(CacheTest, Fetch_WhenTheBlockExistsWithDifferentSize_ThenReturnsNewOneWithRequiredSize)
//{
//	// Arrange
//	int CacheSize = 100;
//	Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);
//
//	int allocSize = 10;
//	Cache_Fetch(&c, "test", allocSize);
//	Block* original = Cache_Fetch(&c, "modified", allocSize);
//	//Cache_Fetch(&c, "upperLimitToAvoidDoubleAllocInSamePlace", allocSize);
//
//	// Act
//	Block* actual = Cache_Fetch(&c, "modified", allocSize * 2);
//
//	// Assert
//	EXPECT_TRUE(original != actual);
//	EXPECT_EQ(std::string("modified"), std::string(actual->name));
//	EXPECT_EQ(actual->size, allocSize * 2);
//}
//
//void EXPECT_BLOCKS_NAMES_ORDER(Cache* c, const char* expected_names[], int namesAmount)
//{
//	Block* curr = c->allBlocks->next;
//	for (int i = 0;
//		i < namesAmount && curr->next != NULL;
//		i++, curr = curr->next)
//	{
//		ASSERT_EQ(std::string(curr->name), std::string(expected_names[i]));
//	}
//}
//
//
//TEST_F(CacheTest, Fetch_WhenInsertingToMiddleOfBuffer_DoesWell)
//{
//	// Arrange
//	int CacheSize = 1000;
//	Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);
//
//	// Act
//	Cache_Fetch(&c, "10", 10);
//	Cache_Fetch(&c, "20", 20);
//	Cache_Fetch(&c, "30", 30);
//	Cache_Fetch(&c, "40", 40);
//	Cache_Fetch(&c, "30", 10);
//	Cache_Fetch(&c, "inter_A", 10);
//	Cache_Fetch(&c, "inter_B", 10);
//
//	const char* expected_names[] = { "10", "20", "30", "inter_A", "inter_B", "40" };
//
//	// Assert
//	EXPECT_BLOCKS_NAMES_ORDER(&c, expected_names, sizeof(expected_names) / sizeof(const char*));
//}
//
//TEST_F(CacheTest, Fetch_WhenSumOfBlockSizesCoversAllCacheBuffer_ThenActuallyAllocatesThemCorrectly)
//{
//	// Arrange
//	int CacheSize = 10;
//	Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);
//
//	// Act
//	Cache_Fetch(&c, "1", 2);
//	Cache_Fetch(&c, "2", 2);
//	Cache_Fetch(&c, "3", 2);
//	Cache_Fetch(&c, "4", 2);
//	Cache_Fetch(&c, "5", 2);
//
//	const char* expected_names[] = { "1", "2", "3", "4", "5" };
//
//	// Assert
//	EXPECT_BLOCKS_NAMES_ORDER(&c, expected_names, sizeof(expected_names) / sizeof(const char*));
//}
//
//TEST_F(CacheTest, Fetch_WhenSumOfBlockSizesExceedsCacheBuffer_ThenReturnNullOnTheExtra)
//{
//	// Arrange
//	int CacheSize = 11;
//	Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);
//
//	// Act
//	Cache_Fetch(&c, "1", 2);
//	Cache_Fetch(&c, "2", 2);
//	Cache_Fetch(&c, "3", 2);
//	Cache_Fetch(&c, "4", 2);
//	Cache_Fetch(&c, "5", 2);
//	Block* extraBlock = Cache_Fetch(&c, "6", 2);
//
//	const char* expected_names[] = { "1", "2", "3", "4", "5" };
//
//	// Assert
//	EXPECT_BLOCKS_NAMES_ORDER(&c, expected_names, sizeof(expected_names) / sizeof(const char*));
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
//	Cache_Fetch(&c, "1", 2);
//	Cache_Fetch(&c, "2", 2);
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
//	Cache_Fetch(&c, "1", 2);
//	auto block_2 = Cache_Fetch(&c, "2", 2);
//	Cache_Fetch(&c, "3", 2);
//	Cache_RemoveBlock(&c, block_2);
//	auto sum = Cache_GetAllocAmount(&c);
//
//	// Assert
//	ASSERT_EQ(sum, 4);
//}
//
//
//
//TEST_F(CacheTest, Remove_WhenBlockIsSealed_ThenDoesntDeleteIt)
//{
//	// Arrange
//	int CacheSize = 10;
//	Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);
//
//	auto block = Cache_Fetch(&c, "1", 2);
//	memcpy(block->buff, "12", 2);
//	block->isSealed = true;
//
//	Cache_RemoveBlock(&c, block);
//	auto sum = Cache_GetAllocAmount(&c);
//
//	// Act
//	auto actual = Cache_Fetch(&c, "1", 2);
//
//	// Assert
//	 
//	ASSERT_EQ(block->buff, actual->buff);
//}
//
//TEST_F(CacheTest, Fatch_WhenNewBlock_NextLocatesNextFreeBlockToFirstAvailablePosition)
//{
//	int CacheSize = 100;
//	Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);
//
//	int posOfBlock = c.nextFreeBlock;
//	auto block = Cache_Fetch(&c, "one", 10);
//
//	ASSERT_EQ(c.nextFreeBlock, posOfBlock + 1);
//}
//
//TEST_F(CacheTest, Fatch_WhenRemovingBlockBeforeNextFreeBlock_NextFreeBlockChangesToPlaceOfRemovedBlock)
//{
//	// Arrange
//	int CacheSize = 100;
//	Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);
//
//	auto block1 = Cache_Fetch(&c, "one", 10);
//	int posOfBlock2 = c.nextFreeBlock;
//	auto block2 = Cache_Fetch(&c, "tow", 10);
//	auto block3 = Cache_Fetch(&c, "three", 10);
//
//	Cache_DeleteBlock(&c, block2);
//
//	ASSERT_EQ(c.nextFreeBlock, posOfBlock2);
//}
//
//TEST_F(CacheTest, Fatch_WhenRemovingBlockAfterNextFreeBlock_NextFreeBlockDoesntChange)
//{
//	// Arrange
//	int CacheSize = 100;
//	Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);
//
//	auto block1 = Cache_Fetch(&c, "one", 10);
//	auto block2 = Cache_Fetch(&c, "two", 10);
//	auto block3 = Cache_Fetch(&c, "three", 10);
//	int posOfBlock4 = c.nextFreeBlock;
//	auto block4 = Cache_Fetch(&c, "four", 10);
//	auto block5 = Cache_Fetch(&c, "five", 10);
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
//	std::vector<std::string> blockNames;
//	for (int i = 0; i < 1000; i++)
//	{
//		blockNames.push_back(std::to_string(i));
//		Cache_Fetch(&c, blockNames.back().c_str(), 1);
//	}
//	ASSERT_TRUE(NULL == Cache_Fetch(&c, "THE NULL", 1));
//}