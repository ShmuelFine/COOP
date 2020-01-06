#include "ObjectTest.h"
#include "Cache.h"
#include "Globals.h"
#include "object.h"
#include "SuperMat.h"

#define _ASSERT_TRUE(x) if (x) return 1;return -1;

//int LOCAL_SCOPE_END__WhenDefiningObjectInside_ThenFreesThemAll()
//{
//
//	//Arrange
//	Cache_Init(&TheGlobalCache);
//	Cache_AllocateCache(&TheGlobalCache, 100);
//	SCOPE_START;
//
//	//DO_A(m1); DO_A(m2); DO_A(m3);
//
//
//	//Act
//
//	if (1)
//	{
//		LOCAL_SCOPE_START;
//		
//		LOCAL_SCOPE_END;
//
//		//Assert
//		_ASSERT_TRUE(innerM1.buff == NULL);
//		_ASSERT_TRUE(innerM2.buff == NULL);
//		_ASSERT_TRUE(innerM3.buff == NULL);
//	}
//
//	SCOPE_END;
//	return 1;
//}
//
//
//int LOCAL_SCOPE_END__WhenDefiningObjectOutside_thenDoesntFreeThem()
//{
//	//Arrange
//	Cache_Init(&TheGlobalCache);
//	Cache_AllocateCache(&TheGlobalCache, 100);
//	SCOPE_START;
//
//	DO_ALLOC_A(m1, 1, 1); DO_ALLOC_A(m2, 1, 1); DO_ALLOC_A(m3, 1, 1);
//
//	//Act
//
//	if (1)
//	{
//		LOCAL_SCOPE_START;
//		DO_A(innerM1); DO_A(innerM2); DO_A(innerM3);
//		LOCAL_SCOPE_END;
//	}
//
//	//Assert
//	_ASSERT_TRUE(m1.buff != NULL);
//	_ASSERT_TRUE(m2.buff != NULL);
//	_ASSERT_TRUE(m3.buff != NULL);
//
//	SCOPE_END;
//
//	return 1;
//}
//
//COOP_API int LOCAL_SCOPE_END__TheLocalScopeEnd_FreesOnlyTheLastInnerScopeThatIsntFreedYet()
//{
//	//Arrange 
//	Cache_Init(&TheGlobalCache);
//	Cache_AllocateCache(&TheGlobalCache,100);
//	SCOPE_START;
//	DO_A(m1); DO_A(m2); DO_A(m3); 
//
//	//Act
//	if (1) {
//		LOCAL_SCOPE_START;
//		DO_ALLOC_A(innerM1, 1, 1); DO_ALLOC_A(innerM2, 1, 1); DO_ALLOC_A(innerM3, 1, 1);
//
//		if (1)
//		{
//			LOCAL_SCOPE_START;
//			DO_ALLOC_A(innerInnerM1, 1, 1); DO_ALLOC_A(innerInnerM2, 1, 1); DO_ALLOC_A(innerInnerM3, 1, 1);
//			LOCAL_SCOPE_END;
//
//			//Assert
//
//			_ASSERT_TRUE(innerInnerM1.buff == NULL);
//			_ASSERT_TRUE(innerInnerM2.buff == NULL);
//			_ASSERT_TRUE(innerInnerM3.buff == NULL);
//		}
//		_ASSERT_TRUE(innerM1.buff != NULL);
//		_ASSERT_TRUE(innerM2.buff != NULL);
//		_ASSERT_TRUE(innerM3.buff != NULL);
//
//		LOCAL_SCOPE_END;
//	}
//	SCOPE_END;
//
//	return 1;
//}
//
//COOP_API int LOCAL_SCOPE_END__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash()
//{
//	//Arrange 
//	Cache_Init(&TheGlobalCache);
//	Cache_AllocateCache(&TheGlobalCache, 100);
//	SCOPE_START;
//	DO_A(m1); DO_A(m2); DO_A(m3);
//
//	//Act
//	if (1) {
//		LOCAL_SCOPE_START;
//		DO_ALLOC_A(innerM1, 1, 1);
//		if (1) {
//			LOCAL_SCOPE_START;
//			LOCAL_SCOPE_END;
//		}
//		_ASSERT_TRUE(innerM1.buff != NULL);
//		LOCAL_SCOPE_END;
//	}
//
//	SCOPE_END;
//
//	return 1;
//}

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
	_ASSERT_TRUE(AS(InMemoryCache *, TheGlobalCache)->buffer <= 
		(char*)(AS(InMemoryCache*, TheGlobalCache)->buffer + AS(InMemoryCache* ,TheGlobalCache)->size - (char*)buff));

}

int New_WhenNew_ConstructsBlockWithRightSize()
{
	//Arrange
	CreateGlobalCache(1000, "GlobalCache", IN_MEMORY_CACHE_);

	//Act
	int* buff;
	NEW(buff,int, 20);

	//Assert
	_ASSERT_TRUE(AS(InMemoryCache*, TheGlobalCache)->allBlockPointers->next->buff ==
		(char*)buff && AS(InMemoryCache*, TheGlobalCache)->allBlockPointers->next->size==20);
}

int Delete_WhenDelete_PointerPointToNull()
{
	//Arrange
	CreateGlobalCache(1000, "GlobalCache", IN_MEMORY_CACHE_);

	//Act
	int* buff;
	NEW(buff, int, 20);

	//Assert
	_ASSERT_TRUE(buff == NULL);
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
	_ASSERT_TRUE(AS(InMemoryCache*, TheGlobalCache)->IsBlockUsed[myIdx] == false);
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
	_ASSERT_TRUE( sizeof(int) * 10 / sizeof(char) == sizeInt &&
				  sizeof(long) * 10 / sizeof(char) == sizeLong &&
				  sizeof(double) * 10 / sizeof(char) == sizeDouble &&
				  sizeof(float) * 10 / sizeof(char) == sizeFloat );
}
