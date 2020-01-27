#include "ObjectTest.h"
#include "Cache.h"
#include "dynamic_memory_management.h"
#include "object.h"
#include "ScopeTest.h"

#define _ASSERT_TRUE(x) if (x) return 1;return -1;
#define _ASSERT_FALSE(x) if (!(x)) return -1;

#define CREATE_OBJECT4TEST(type, instance_name, ...)		        \
	if (! is_ ##type ##VirtualTable__initialized) type ##_init();   \
	instance_name.vTable=&type ##VTable;							\
	instance_name.vTable->_ctor(&instance_name, __VA_ARGS__);		\
	instance_name._next= NULL;										\
	REGISTER_OBJECT(&instance_name)

int LOCAL_SCOPE_END__WhenDefiningObjectInside_ThenFreesThemAll()
{
	SCOPE_START;

	////SCOPE_START;
	//object _scope_obj_list; 
	//	if (!setjmp(SCOPE_FALLBACK_ADDR[_CurrScope_Idx++]))
	//	{  
	//		_scope_obj_list.vTable = NULL; 
	//		_scope_obj_list._next = NULL;

	//Arrange
	CreateGlobalCache(1000, "GlobalCache", IN_MEMORY_CACHE_);

	CREATE_OBJECT(ScopeTest, s1, 10);
	CREATE_OBJECT(ScopeTest, s2, 10);
	CREATE_OBJECT(ScopeTest, s3, 10);

	//Act

	if (1)
	{
		ScopeTest innerS1, innerS2, innerS3;
		LOCAL_SCOPE_START;
		CREATE_OBJECT4TEST(ScopeTest, innerS1, 10);
		CREATE_OBJECT4TEST(ScopeTest, innerS2, 10);
		CREATE_OBJECT4TEST(ScopeTest, innerS3, 10);
		LOCAL_SCOPE_END;

		//Assert
		_ASSERT_FALSE(innerS1.buff != NULL);
		_ASSERT_FALSE(innerS2.buff != NULL);
		_ASSERT_FALSE(innerS3.buff != NULL);
	}

	DestroyGlobalCache();

	SCOPE_END;
}


int LOCAL_SCOPE_END__WhenDefiningObjectOutside_thenDoesntFreeThem()
{
	//Arrange
	ScopeTest s1, s2, s3;
	SCOPE_START;

	CreateGlobalCache(100, "GlobalCahce", IN_MEMORY_CACHE_);

	

	CREATE_OBJECT4TEST(ScopeTest, s1, 10);
	CREATE_OBJECT4TEST(ScopeTest, s2, 10);
	CREATE_OBJECT4TEST(ScopeTest, s3, 10);

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

	DestroyGlobalCache();

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
			ScopeTest innerInnerS1, innerInnerS2, innerInnerS3;
			LOCAL_SCOPE_START;
			CREATE_OBJECT4TEST(ScopeTest, innerInnerS1, 10);
			CREATE_OBJECT4TEST(ScopeTest, innerInnerS2, 10);
			CREATE_OBJECT4TEST(ScopeTest, innerInnerS3, 10);
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
	DestroyGlobalCache();
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
	DestroyGlobalCache();
	SCOPE_END;

	return 1;
}

#define AS(T, x) ((T)x)

int New_WhenNew_ThenReturnesPointerInCache()
{
	//Arrange
	CreateGlobalCache(1000, "GlobalCache", IN_MEMORY_CACHE_);
	
	//Act
	int* buff;
	Block* returned; 
	CALL(AddNewBlock, *TheGlobalCache, sizeof(int) * 20, &returned); 
	buff = (int*)returned->buff;

	//Assert
	_ASSERT_FALSE(AS(InMemoryCache *, TheGlobalCache)->buffer > 
		(char*)(AS(InMemoryCache*, TheGlobalCache)->buffer + AS(InMemoryCache* ,TheGlobalCache)->size - (char*)buff));

	DestroyGlobalCache();
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
	DestroyGlobalCache();
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

	DestroyGlobalCache();

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
	DELETE_OBJ(buff);
	int myIdx = (int)((char*)b - (char*)AS(InMemoryCache*,TheGlobalCache)->allBlocks) / sizeof(Block);

	//Assert
	_ASSERT_FALSE(AS(InMemoryCache*, TheGlobalCache)->IsBlockUsed[myIdx] != false);

	DestroyGlobalCache();

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

	DestroyGlobalCache();

	return 1;
}
