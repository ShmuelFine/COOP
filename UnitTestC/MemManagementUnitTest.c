#include "MemManagementUnitTest.h"


TEST_FUN_IMPL(MemoryManagementTest, NEW_AllocatesSufficientMemory)
{
	//Arrange
	init_global_memory( sizeof(int) * 40, STACK_BASED_MEMORY );

	//Act
	int* buff = NULL;
	NEW(buff, int[20]);
	ASSERT_NOT_NULL(buff);
	
	// Assert:
	// Make sure we can access all our memory:
	for (int i = 0; i < 20; i++)
		buff[i] = i;

}END_FUN
//
//TEST_FUN_IMPL(MemoryManagementTest, New_WhenAllocatingClassInstance_ThenReturnsRightAmountOfMemroy)
//{
//	//Arrange
//	init_global_memory( sizeof(BaseClassExample)*10, STACK_BASED_MEMORY );
//
//	//Act
//	BaseClassExample* m = NULL;
//	NEW(m, BaseClassExample);
//	ASSERT_NOT_NULL(m);
//	// Make sure we can access all our memory:
//	m->width = 32;
//	m->hight = 345;
//
//	//Assert
//	bool canAllocMore = true;
//	TRY
//	{
//		char* otherBuff;
//		NEW(otherBuff, char);
//	}CATCH{
//		canAllocMore = false;
//	}
//	END_TRY;
//
//	NTEST_ASSERT(canAllocMore == false);
//
//	
//}END_FUN

TEST_FUN_IMPL(MemoryManagementTest, Delete_WhenDeletingAllocMemory_ThenItFreesTheResource)
{
	//Arrange
	init_global_memory( sizeof(int) * 20, STACK_BASED_MEMORY );
	// MD = 3 ints
	// 2xMD for anchor and suffix
	// 1xMD for new block
	// max size of new block = 20 ints - 3xMDs = (20 - 3*3) ints
	//Act
	int* buff;
	NEW(buff, int[11]);
	DELETE_OBJ(buff);

	//Assert
	NEW(buff, int[11]);
	// if it throws - test failed

}END_FUN


INIT_TEST_SUITE(MemoryManagementTest)
BIND_TEST(MemoryManagementTest, NEW_AllocatesSufficientMemory);
BIND_TEST(MemoryManagementTest, Delete_WhenDeletingAllocMemory_ThenItFreesTheResource);
END_INIT_TEST_SUITE(MemoryManagementTest)