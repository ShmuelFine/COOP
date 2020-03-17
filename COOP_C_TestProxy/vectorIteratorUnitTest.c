#include "vectorIteratorUnitTest.h"
#include "vectorIterator.h"

FUN_IMPL(vectorIterator_getContentsOf_Sanity)
{
	CREATE(vector, v1));

	FUN(&v1, push_back), 3 CALL

	CREATE(vectorIterator, vecIt), &v1, 0);

	int retVal = 0;
	FUN(&vecIt, getContentsOf), & retVal CALL

	ASSERT(retVal == 3);
	

}END_FUN

FUN_IMPL(vectorIterator_increment_Sanity)
{
	CREATE(vector, v1));

	FUN(&v1, push_back), 3 CALL
	FUN(&v1, push_back), 4 CALL
	FUN(&v1, push_back), 5 CALL
	
	CREATE(vectorIterator, vecIt), & v1, 0);

	int retVal = 0;
	int retVal1 = 0;
	int retVal2 = 0;

	FUN(&vecIt, getContentsOf), & retVal CALL

	FUN(&vecIt, increment) CALL
	FUN(&vecIt, getContentsOf), & retVal1 CALL

	FUN(&vecIt, increment) CALL
	FUN(&vecIt, getContentsOf), & retVal2 CALL

	ASSERT(retVal == 3);
	ASSERT(retVal1 == 4);
	ASSERT(retVal2 == 5);

}END_FUN