#include "MemManagementUnitTest.h"
#include "Cache.h"
#include "dynamic_memory_management.h"
#include "COOP.h"
#include "ScopeTester.h"
#include "BaseClassExample.h"
#define AS(T, x) ((T)x)

FUN_IMPL(New_WhenAllocatingBuffer_ThenReturnsRightAmountOfMemroy)
{
	//Arrange

	CREATE(MemoryManager, memManager), sizeof(int) * 20, STACK_BASED_MEMORY);

	//Act
	int * buff = NULL;
	NEW(buff, int[20]);
	
	// Make sure we can access all our memory:
	for (int i = 0; i < 20; i++)
		buff[i] = i;

	//Assert
	bool canAllocMore = true;
	TRY
	{
		char* otherBuff;
		NEW(otherBuff, char);
	}CATCH{
		canAllocMore = false;
	}
	END_TRY;

	ASSERT(canAllocMore == false);

}END_FUN

FUN_IMPL(New_WhenAllocatingClassInstance_ThenReturnsRightAmountOfMemroy)
{
	//Arrange
	SCOPE_START;

	CREATE(MemoryManager, memManager), sizeof(BaseClassExample), STACK_BASED_MEMORY);

	//Act
	BaseClassExample * m = NULL;
	NEW(m, BaseClassExample);

	// Make sure we can access all our memory:
	m->width = 32;
	m->hight = 345;

	//Assert
	bool canAllocMore = true;
	TRY
	{
		char* otherBuff;
		NEW(otherBuff, char);
	}CATCH{
		canAllocMore = false;
	}
	END_TRY;

	ASSERT(canAllocMore == false);

	END_SCOPE;
}END_FUN

FUN_IMPL(Delete_WhenDeletingAllocMemory_ThenItFreesTheResource)
{
	SCOPE_START;

	//Arrange
	CREATE(MemoryManager, memManager), sizeof(int) * 20, STACK_BASED_MEMORY);

	//Act
	int * buff;
	NEW(buff, int[20]);
	DELETE_OBJ(buff);
	
	//Assert
	bool canAllocMore = true;
	TRY
	{
		char* otherBuff;
		NEW(otherBuff, int[20]);
	}CATCH{
		canAllocMore = false;
	}
	END_TRY;


	ASSERT(canAllocMore == true);

	END_SCOPE;
}END_FUN
