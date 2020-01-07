#include "ObjectTest.h"
#include "Cache.h"
#include "Globals.h"
#include "object.h"
#include "ScopeTest.h"

#define _ASSERT_TRUE(x) if (x) return 1;return -1;
#define _ASSERT_FALSE(x) if (!(x)) return -1;

int LOCAL_SCOPE_END__WhenDefiningObjectInside_ThenFreesThemAll()
{
	SCOPE_START;
	//Arrange
	CreateGlobalCache(1000, "GlobalCache", IN_MEMORY_CACHE_);

	CREATE_OBJECT(ScopeTest, s1, 10);
	CREATE_OBJECT(ScopeTest, s2, 10);
	CREATE_OBJECT(ScopeTest, s3, 10);

	//Act

	if (1)
	{
		LOCAL_SCOPE_START;
		CREATE_OBJECT(ScopeTest, innerS1, 10);
		CREATE_OBJECT(ScopeTest, innerS2, 10);
		CREATE_OBJECT(ScopeTest, innerS3, 10);
		LOCAL_SCOPE_END;

		//Assert
		_ASSERT_FALSE(innerS1.buff != NULL);
		_ASSERT_FALSE(innerS2.buff != NULL);
		_ASSERT_FALSE(innerS3.buff != NULL);
	}

	SCOPE_END;
	return 1;
}


int LOCAL_SCOPE_END__WhenDefiningObjectOutside_thenDoesntFreeThem()
{
	SCOPE_START;
	//Arrange
	CreateGlobalCache(100, "GlobalCahce", IN_MEMORY_CACHE_);



	CREATE_OBJECT(ScopeTest, s1, 10);
	CREATE_OBJECT(ScopeTest, s2, 10);
	CREATE_OBJECT(ScopeTest, s3, 10);

	//Act

	if (1)
	{
		LOCAL_SCOPE_START;
		CREATE_OBJECT(ScopeTest, innerS1, 10);
		CREATE_OBJECT(ScopeTest, innerS2, 10);
		CREATE_OBJECT(ScopeTest, innerS3, 10);
		LOCAL_SCOPE_END;
	}

	//Assert
	_ASSERT_FALSE(s1.buff == NULL);
	_ASSERT_FALSE(s2.buff == NULL);
	_ASSERT_FALSE(s3.buff == NULL);

	SCOPE_END;

	return 1;
}

int LOCAL_SCOPE_END__TheLocalScopeEnd_FreesOnlyTheLastInnerScopeThatIsntFreedYet()
{
	SCOPE_START;
	//Arrange 
	CreateGlobalCache(1000, "GlobalCache", IN_MEMORY_CACHE_);


	CREATE_OBJECT(ScopeTest, s1, 10);
	CREATE_OBJECT(ScopeTest, s2, 10);
	CREATE_OBJECT(ScopeTest, s3, 10);

	//Act
	if (1) {
		LOCAL_SCOPE_START;
		CREATE_OBJECT(ScopeTest, innerS1, 10);
		CREATE_OBJECT(ScopeTest, innerS2, 10);
		CREATE_OBJECT(ScopeTest, innerS3, 10);

		if (1)
		{
			LOCAL_SCOPE_START;
			CREATE_OBJECT(ScopeTest, innerInnerS1, 10);
			CREATE_OBJECT(ScopeTest, innerInnerS2, 10);
			CREATE_OBJECT(ScopeTest, innerInnerS3, 10);
			LOCAL_SCOPE_END;

			//Assert

			_ASSERT_FALSE(innerInnerS1.buff != NULL);
			_ASSERT_FALSE(innerInnerS2.buff != NULL);
			_ASSERT_FALSE(innerInnerS3.buff != NULL);
		}
		_ASSERT_FALSE(innerS1.buff == NULL);
		_ASSERT_FALSE(innerS2.buff == NULL);
		_ASSERT_FALSE(innerS3.buff == NULL);

		LOCAL_SCOPE_END;
	}
	SCOPE_END;

	return 1;
}

int LOCAL_SCOPE_END__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash()
{
	SCOPE_START;
	//Arrange 
	CreateGlobalCache(1000, "GlobalCache", IN_MEMORY_CACHE_);


	CREATE_OBJECT(ScopeTest, s1, 10);
	CREATE_OBJECT(ScopeTest, s2, 10);
	CREATE_OBJECT(ScopeTest, s3, 10);

	//Act
	if (1) {
		LOCAL_SCOPE_START;
		CREATE_OBJECT(ScopeTest, innerS1, 10);	
		if (1) {
			LOCAL_SCOPE_START;
			LOCAL_SCOPE_END;
		}
		_ASSERT_FALSE(innerS1.buff == NULL);
		LOCAL_SCOPE_END;
	}

	SCOPE_END;

	return 1;
}

#define AS(T, x) ((T)x)

int New_WhenNew_ThenReturnesPointerInCache()
{
	//Arrange
	CreateGlobalCache(1000, "GlobalCache", IN_MEMORY_CACHE_);
	//Act
	//int* buff = NEW(int, 20);
	int* buff;
	//NEW(buff, int, 20);
	Block* returned; 
	CALL(AddNewBlock, *TheGlobalCache, sizeof(int) * 20, &returned); 
	buff = (int*)returned->buff;

	//Assert
	_ASSERT_FALSE(AS(InMemoryCache *, TheGlobalCache)->buffer > 
		(char*)(AS(InMemoryCache*, TheGlobalCache)->buffer + AS(InMemoryCache* ,TheGlobalCache)->size - (char*)buff));

	return 1;

}

int New_WhenNew_ConstructsBlockWithRightSize()
{
	//Arrange
	CreateGlobalCache(1000, "GlobalCache", IN_MEMORY_CACHE_);

	//Act
	int* buff;
	NEW(buff,int, 20);

	//Assert
	_ASSERT_FALSE(AS(InMemoryCache*, TheGlobalCache)->allBlockPointers->next->buff !=
		(char*)buff && AS(InMemoryCache*, TheGlobalCache)->allBlockPointers->next->size != 20);

	return 1;
}

int Delete_WhenDelete_PointerPointToNull()
{
	//Arrange
	CreateGlobalCache(1000, "GlobalCache", IN_MEMORY_CACHE_);

	//Act
	int* buff;
	NEW(buff, int, 20);

	//Assert
	_ASSERT_FALSE(buff != NULL);

	return 1;
}

int Delete_WhenDelete_ThenDeletesTheBlock()
{
	//Arrange
	CreateGlobalCache(1000, "GlobalCache", IN_MEMORY_CACHE_);

	//Act
	int* buff;
	NEW(buff, int, 20);
	Block* b = Cache_FindBlockByBuffAddress(AS(InMemoryCache*,TheGlobalCache), buff);
	DELETE(buff);
	int myIdx = (int)((char*)b - (char*)AS(InMemoryCache*,TheGlobalCache)->allBlocks) / sizeof(Block);

	//Assert
	_ASSERT_FALSE(AS(InMemoryCache*, TheGlobalCache)->IsBlockUsed[myIdx] != false);

	return 1;
}

int NEW_WhenAllocDifferentTypeThenChar_AllocatesTheRightSize()
{
	//Arrange
	CreateGlobalCache(1000, "GlobalCache", IN_MEMORY_CACHE_);
	int* ibuff;
	long* lBuff;
	double* dBuff;
	float* fBuff;

	int sizeInt, sizeLong, sizeDouble, sizeFloat;
	//Act
	NEW(ibuff, int, 10);
	NEW(lBuff, long, 10);
	NEW(dBuff, double, 10);
	NEW(fBuff, float, 10);

	sizeInt = ((InMemoryCache*)TheGlobalCache)->allBlocks[2].size;
	sizeLong = ((InMemoryCache*)TheGlobalCache)->allBlocks[3].size;
	sizeDouble = ((InMemoryCache*)TheGlobalCache)->allBlocks[4].size;
	sizeFloat = ((InMemoryCache*)TheGlobalCache)->allBlocks[5].size;

	//Assert
	_ASSERT_FALSE( sizeof(int) * 10  != sizeInt &&
				  sizeof(long) * 10  != sizeLong &&
				  sizeof(double) * 10  != sizeDouble &&
				  sizeof(float) * 10  != sizeFloat );

	return 1;
}
