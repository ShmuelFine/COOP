#include "MemManagementUnitTest.h"


TEST_FUN_IMPL(MemoryManagementTest, NEW_AllocatesSufficientMemory)
{
	//Arrange
	init_global_memory(sizeof(int) * 40, LIMITED_SIZE_MEMORY);

	//Act
	int* buff = NULL;
	ALLOC_ARRAY(buff, int, 20);
	ASSERT_NOT_NULL(buff);

	// Assert:
	// Make sure we can access all our memory:
	FOR (int i = 0; i < 20; i++)
		buff[i] = i;
	END_LOOP;

}END_FUN

TEST_FUN_IMPL(MemoryManagementTest, Delete_WhenDeletingAllocMemory_ThenItFreesTheResource)
{
	//Arrange
	init_global_memory(sizeof(int) * 20, LIMITED_SIZE_MEMORY);
	// MD = 3 ints
	// 2xMD for anchor and suffix
	// 1xMD for new block
	// max size of new block = 20 ints - 3xMDs = (20 - 3*3) ints
	//Act
	int* buff;
	ALLOC(buff, int[11]);
	FREE(buff);

	//Assert
	ALLOC(buff, int[11]);
	// if it throws - test failed

}END_FUN


INIT_TEST_SUITE(MemoryManagementTest)
BIND_TEST(MemoryManagementTest, NEW_AllocatesSufficientMemory);
BIND_TEST(MemoryManagementTest, Delete_WhenDeletingAllocMemory_ThenItFreesTheResource);
END_INIT_TEST_SUITE(MemoryManagementTest)