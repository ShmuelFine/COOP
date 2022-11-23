#include "vectorIteratorUnitTest.h"

FUN_IMPL(vectorIterator_getContentsOf_Sanity)
{
	CREATE(vector, v1) CALL;

	init_global_memory(sizeof(int) * 1, HEAP_BASED_MEMORY);

	FUN(&v1, push_back), 3 CALL;

	CREATE(vectorIterator, vecIt), & v1, 0 CALL;

	int retVal = 0;
	FUN(&vecIt, getContentsOf), & retVal CALL;

	TEST_ASSERT(retVal == 3);


}END_FUN
FUN_IMPL(vectorIterator_increment_Sanity)
{
	CREATE(vector, v1) CALL;

	init_global_memory(sizeof(int) * 10, HEAP_BASED_MEMORY);

	FUN(&v1, push_back), 3 CALL;
	FUN(&v1, push_back), 4 CALL;
	FUN(&v1, push_back), 5 CALL;

	CREATE(vectorIterator, vecIt), & v1, 0 CALL;

	CREATE(vectorIterator, vecIt1), & v1, 1 CALL;

	int retVal = 0;
	int retVal1 = 0;
	int retVal2 = 0;
	bool eq = false;

	FUN(&vecIt, getContentsOf), & retVal CALL;

	FUN(&vecIt, increment) CALL;
	FUN(&vecIt, getContentsOf), & retVal1 CALL;
	FUN(&vecIt, equals), vecIt1, & eq CALL;

	FUN(&vecIt, increment) CALL;
	FUN(&vecIt, getContentsOf), & retVal2 CALL;

	TEST_ASSERT(retVal == 3);
	TEST_ASSERT(retVal1 == 4);
	TEST_ASSERT(retVal2 == 5);
	TEST_ASSERT(eq == true);

}END_FUN
