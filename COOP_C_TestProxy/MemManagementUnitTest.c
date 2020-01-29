#include "MemManagementUnitTest.h"
#include "Cache.h"
#include "dynamic_memory_management.h"
#include "object.h"
#include "ScopeTester.h"
#include "Mat.h"
#define AS(T, x) ((T)x)

int New_WhenAllocatingBuffer_ThenReturnsRightAmountOfMemroy()
{
	//Arrange
	SCOPE_START;

	CreateGlobalCache(sizeof(int) * 20, IN_MEMORY_CACHE_);

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

	DestroyGlobalCache();

	ASSERT(canAllocMore == false);

	SCOPE_END;
}

int New_WhenAllocatingClassInstance_ThenReturnsRightAmountOfMemroy()
{
	//Arrange
	SCOPE_START;

	CreateGlobalCache(sizeof(Mat), IN_MEMORY_CACHE_);

	//Act
	Mat * m = NULL;
	NEW(m, Mat);

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

	DestroyGlobalCache();

	ASSERT(canAllocMore == false);

	SCOPE_END;
}

int Delete_WhenDeletingAllocMemory_ThenItFreesTheResource()
{
	SCOPE_START;

	//Arrange
	CreateGlobalCache(sizeof(int) * 20, IN_MEMORY_CACHE_);

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

	DestroyGlobalCache();

	ASSERT(canAllocMore == true);

	SCOPE_END;
}