#include "pch.h"


#ifdef _DEBUG
#pragma comment(lib, "object_d.lib")
#else
#pragma comment(lib, "object.lib")
#endif

#include "..\object\Cache.h"

class CacheTest : public ::testing::Test
{
public:

	virtual void SetUp()
	{
		InMemoryCache_init();
	}

	virtual void TearDown()
	{

	}
};

TEST_F(CacheTest, New_WhenThereIsNotEnoughSpace_ThenReturnsNullPointer)
{
	// Arrange
	int CacheSize = 10;
	CREATE_OBJECT(InMemoryCache, c, CacheSize);
	//Cache c; Cache_Init(&c); Cache_AllocateCache(&c, CacheSize);

	int allocSize = 100;
	Block* expected = NULL;

	// Act
	Block* actual=NULL;
	//CALL(Cache, AddNewBlock, c, allocSize, actual);
	//struct AddNewBlock_t_ * f = (struct AddNewBlock_t_*)callFunction((function*)(c).vTable->AddNewBlock);
	//f->func(&(c), allocSize, actual);

	c.vTable->AddNewBlock->func(&c, allocSize, &actual);
	// Assert
	EXPECT_EQ(expected, actual);
}

void EXPECT_BLOCKS_EQ(Block* expected, Block* actual)
{
	if (expected == NULL && actual == NULL)
		return;

	ASSERT_FALSE(
		(expected == NULL && actual != NULL) ||
		(expected != NULL && actual == NULL));

	EXPECT_EQ(expected->buff, actual->buff);
	EXPECT_EQ(expected->next, actual->next);
	EXPECT_EQ(expected->size, actual->size);
}

TEST_F(CacheTest, New_WhenThereIsSufficientSpace_ThenReturnsValidPointer)
{
	// Arrange
	int CacheSize = 100;
	CREATE_OBJECT(InMemoryCache, c, CacheSize);

	int allocSize = 10;
	Block expected;
	expected.buff = c.buffer;
	expected.next = c.allBlocks->next;
	expected.size = allocSize;

	// Act
	Block* actual=NULL;
	c.vTable->AddNewBlock->func(&c, allocSize,&actual);

	// Assert
	EXPECT_BLOCKS_EQ(&expected, actual);
}



TEST_F(CacheTest, Fetch_WhenInsertingToMiddleOfBuffer_DoesWell)
{
	// Arrange
	int CacheSize = 1000;
	CREATE_OBJECT(InMemoryCache, c, CacheSize);

	// Act
	Block *b1, *b2,  *b3,  *b4;
	b1 = b2 = b3 = b4 = NULL;
	c.vTable->AddNewBlock->func(&c, 10, &b1);
	c.vTable->AddNewBlock->func(&c, 30, &b2);
	c.vTable->AddNewBlock->func(&c, 30, &b3);

	c.vTable->RemoveBlock->func(&c, b2);
	c.vTable->AddNewBlock->func(&c, 10, &b4);

	size_t b1idx = b1->buff - c.buffer;
	size_t b3idx = b3->buff - c.buffer;
	size_t b4idx = b4->buff - c.buffer;


	// Assert
	EXPECT_TRUE(b1idx < b4idx < b3idx);
}

TEST_F(CacheTest, Fetch_WhenSumOfBlockSizesCoversAllCacheBuffer_ThenActuallyAllocatesThemCorrectly)
{
	// Arrange
	int CacheSize = 10;
	CREATE_OBJECT(InMemoryCache, c, CacheSize);

	// Act
	Block* b1, * b2, * b3, * b4, * b5;
	b1 = b2 = b3 = b4 = b5 = NULL;
	c.vTable->AddNewBlock->func(&c, 2, &b1);
	c.vTable->AddNewBlock->func(&c, 2, &b2);
	c.vTable->AddNewBlock->func(&c, 2, &b3);
	c.vTable->AddNewBlock->func(&c, 2, &b4);
	c.vTable->AddNewBlock->func(&c, 2, &b5);

	size_t b1idx = b1->buff - c.buffer;
	size_t b2idx = b2->buff - c.buffer;
	size_t b3idx = b3->buff - c.buffer;
	size_t b4idx = b4->buff - c.buffer;
	size_t b5idx = b5->buff - c.buffer;

	// Assert
	EXPECT_TRUE(b1idx < b2idx < b3idx < b4idx < b5idx);
}

TEST_F(CacheTest, Fetch_WhenSumOfBlockSizesExceedsCacheBuffer_ThenReturnNullOnTheExtra)
{
	// Arrange
	int CacheSize = 11;
	CREATE_OBJECT(InMemoryCache, c, CacheSize);

	// Act
	Block* b1, * b2, * b3, * b4, * b5;
	b1 = b2 = b3 = b4 = b5 = NULL;
	c.vTable->AddNewBlock->func(&c, 2, &b1);
	c.vTable->AddNewBlock->func(&c, 2, &b2);
	c.vTable->AddNewBlock->func(&c, 2, &b3);
	c.vTable->AddNewBlock->func(&c, 2, &b4);
	c.vTable->AddNewBlock->func(&c, 2, &b5);

	size_t b1idx = b1->buff - c.buffer;
	size_t b2idx = b2->buff - c.buffer;
	size_t b3idx = b3->buff - c.buffer;
	size_t b4idx = b4->buff - c.buffer;
	size_t b5idx = b5->buff - c.buffer;

	Block* extraBlock=NULL;
	c.vTable->AddNewBlock->func(&c, 2, &extraBlock);


	// Assert
	EXPECT_TRUE(b1idx < b2idx < b3idx < b4idx < b5idx);
	ASSERT_TRUE(extraBlock == NULL);
}

TEST_F(CacheTest, GetAllocAmount_WhenNoAllocationWereDone_ThenReturnsZero)
{
	// Arrange
	int CacheSize = 10;
	CREATE_OBJECT(InMemoryCache, c, CacheSize);

	// Act
	auto sum = Cache_GetAllocAmount(&c);

	// Assert
	ASSERT_EQ(sum, 0);
}

TEST_F(CacheTest, GetAllocAmount_WhenSomeAllocationWereDone_ThenReturnsTheirSum)
{
	// Arrange
	int CacheSize = 10;
	CREATE_OBJECT(InMemoryCache, c, CacheSize);

	// Act
	Block* temp=NULL;
	c.vTable->AddNewBlock->func(&c, 2, &temp);
	c.vTable->AddNewBlock->func(&c, 2, &temp);
	auto sum = Cache_GetAllocAmount(&c);

	// Assert
	ASSERT_EQ(sum, 4);
}

TEST_F(CacheTest, GetAllocAmount_WhenSomeAllocationAndRemovalsWereDone_ThenReturnsTheirSum)
{
	// Arrange
	int CacheSize = 10;
	CREATE_OBJECT(InMemoryCache, c, CacheSize);

	// Act
	Block* block_2=NULL, * temp=NULL;
	c.vTable->AddNewBlock->func(&c, 2, &temp);
	c.vTable->AddNewBlock->func(&c, 2, &block_2);
	c.vTable->AddNewBlock->func(&c, 2, &temp);
	c.vTable->RemoveBlock->func(&c, block_2);
	auto sum = Cache_GetAllocAmount(&c);

	// Assert
	ASSERT_EQ(sum, 4);
}

TEST_F(CacheTest, New_WhenNewBlock_NextLocatesNextFreeBlockToFirstAvailablePosition)
{
	int CacheSize = 100;
	CREATE_OBJECT(InMemoryCache, c, CacheSize);

	int posOfBlock = c.nextFreeBlock;
	Block * block=NULL;
	c.vTable->AddNewBlock->func(&c, 10, &block);

	ASSERT_EQ(c.nextFreeBlock, posOfBlock + 1);
}

TEST_F(CacheTest, New_WhenRemovingBlockBeforeNextFreeBlock_NextFreeBlockChangesToPlaceOfRemovedBlock)
{
	// Arrange
	int CacheSize = 100;
	CREATE_OBJECT(InMemoryCache, c, CacheSize);

	Block* block1,*block2,*block3;
	block1 = block2 = block3 = NULL;
	c.vTable->AddNewBlock->func(&c, 10, &block1);
	int posOfBlock2 = c.nextFreeBlock;
	c.vTable->AddNewBlock->func(&c, 10, &block2);
	c.vTable->AddNewBlock->func(&c, 10, &block3);

	c.vTable->RemoveBlock->func(&c, block2);

	ASSERT_EQ(c.nextFreeBlock, posOfBlock2);
}

TEST_F(CacheTest, New_WhenRemovingBlockAfterNextFreeBlock_NextFreeBlockDoesntChange)
{
	// Arrange
	int CacheSize = 100;
	CREATE_OBJECT(InMemoryCache, c, CacheSize);

	Block* block1, * block2, * block3, * block4, * block5;
	block1 = block2 = block3 = block4 = block5 = NULL;
	c.vTable->AddNewBlock->func(&c, 10, &block1);
	c.vTable->AddNewBlock->func(&c, 10, &block2);
	c.vTable->AddNewBlock->func(&c, 10, &block3);
	int posOfBlock4 = c.nextFreeBlock;
	c.vTable->AddNewBlock->func(&c, 10, &block4);
	c.vTable->AddNewBlock->func(&c, 10, &block5);

	c.vTable->RemoveBlock->func(&c, block2);
	c.vTable->RemoveBlock->func(&c, block4);

	ASSERT_TRUE(c.nextFreeBlock < posOfBlock4);
}

TEST_F(CacheTest, NAME_TBD)
{
	// Arrange
	int CacheSize = 1000;
	CREATE_OBJECT(InMemoryCache, c, CacheSize);
	// Act
	Block* temp=NULL;
	for (int i = 0; i < 1000; i++)
	{
		c.vTable->AddNewBlock->func(&c, 1,&temp);
	}
	c.vTable->AddNewBlock->func(&c, 1, &temp);
	//Assert
	ASSERT_TRUE(NULL == temp);
}

TEST_F(CacheTest, FindBlockByBuff_DoesWell)
{
	// Arrange
	int CacheSize = 1000;
	CREATE_OBJECT(InMemoryCache, c, CacheSize);
	// Act
	Block* expected=NULL;
	c.vTable->AddNewBlock->func(&c, 20, &expected);
	char* buff = expected->buff;
	Block* actual = Cache_FindBlockByBuffAddress(&c, buff);

	//Assert
	EXPECT_EQ(expected, actual);
}