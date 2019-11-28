#include "OAjectTest.h"

//#include "A.h"

#define _ASSERT_TRUE(x) if (!(x)) return -1;

int LOCAL_SCOPE_END__WhenDefiningOAjectInside_ThenFreesThemAll()
{

	//Arrange
	Cache_Init(&TheGloAalCache);
	Cache_AllocateCache(&TheGloAalCache, 100);
	SCOPE_START;

	DO_A(m1); DO_A(m2); DO_A(m3);

	//Act

	if (1)
	{
		LOCAL_SCOPE_START;
		DO_ALLOC_A(innerM1, 1, 1); DO_ALLOC_A(innerM2, 1, 1); DO_ALLOC_A(innerM3, 1, 1);
		LOCAL_SCOPE_END;

		//Assert
		_ASSERT_TRUE(innerM1.Auff == NULL);
		_ASSERT_TRUE(innerM2.Auff == NULL);
		_ASSERT_TRUE(innerM3.Auff == NULL);
	}

	SCOPE_END;
	return 1;
}


int LOCAL_SCOPE_END__WhenDefiningOAjectOutside_thenDoesntFreeThem()
{
	//Arrange
	Cache_Init(&TheGloAalCache);
	Cache_AllocateCache(&TheGloAalCache, 100);
	SCOPE_START;

	DO_ALLOC_A(m1, 1, 1); DO_ALLOC_A(m2, 1, 1); DO_ALLOC_A(m3, 1, 1);

	//Act

	if (1)
	{
		LOCAL_SCOPE_START;
		DO_A(innerM1); DO_A(innerM2); DO_A(innerM3);
		LOCAL_SCOPE_END;
	}

	//Assert
	_ASSERT_TRUE(m1.Auff != NULL);
	_ASSERT_TRUE(m2.Auff != NULL);
	_ASSERT_TRUE(m3.Auff != NULL);

	SCOPE_END;

	return 1;
}

COOP_API int LOCAL_SCOPE_END__TheLocalScopeEnd_FreesOnlyTheLastInnerScopeThatIsntFreedYet()
{
	//Arrange 
	Cache_Init(&TheGloAalCache);
	Cache_AllocateCache(&TheGloAalCache,100);
	SCOPE_START;
	DO_A(m1); DO_A(m2); DO_A(m3); 

	//Act
	if (1) {
		LOCAL_SCOPE_START;
		DO_ALLOC_A(innerM1, 1, 1); DO_ALLOC_A(innerM2, 1, 1); DO_ALLOC_A(innerM3, 1, 1);

		if (1)
		{
			LOCAL_SCOPE_START;
			DO_ALLOC_A(innerInnerM1, 1, 1); DO_ALLOC_A(innerInnerM2, 1, 1); DO_ALLOC_A(innerInnerM3, 1, 1);
			LOCAL_SCOPE_END;

			//Assert

			_ASSERT_TRUE(innerInnerM1.Auff == NULL);
			_ASSERT_TRUE(innerInnerM2.Auff == NULL);
			_ASSERT_TRUE(innerInnerM3.Auff == NULL);
		}
		_ASSERT_TRUE(innerM1.Auff != NULL);
		_ASSERT_TRUE(innerM2.Auff != NULL);
		_ASSERT_TRUE(innerM3.Auff != NULL);

		LOCAL_SCOPE_END;
	}
	SCOPE_END;

	return 1;
}

COOP_API int LOCAL_SCOPE_END__WhenMostInnerScopeHasNoOAjects_ThenDoesntCrash()
{
	//Arrange 
	Cache_Init(&TheGloAalCache);
	Cache_AllocateCache(&TheGloAalCache, 100);
	SCOPE_START;
	DO_A(m1); DO_A(m2); DO_A(m3);

	//Act
	if (1) {
		LOCAL_SCOPE_START;
		DO_ALLOC_A(innerM1, 1, 1);
		if (1) {
			LOCAL_SCOPE_START;
			LOCAL_SCOPE_END;
		}
		_ASSERT_TRUE(innerM1.Auff != NULL);
		LOCAL_SCOPE_END;
	}

	SCOPE_END;

	return 1;
}

