#include"vectorUnitTest.h"


FUN_IMPL(vector_push_back_SanityTest)
{
	// Arrange
	CREATE(vector, v1));

	CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY);

	// Act
	int retVal = 0;
	int retVal1 = 0;
	int retVal2 = 0;
	int retVal3 = 0;
	int retVal4 = 0;
	FUN(&v1, push_back), 3 CALL
		FUN(&v1, print) CALL
		FUN(&v1, at), 0, & retVal CALL

		FUN(&v1, push_back), 4 CALL
		FUN(&v1, print) CALL
		FUN(&v1, at), 1, & retVal1 CALL

		FUN(&v1, push_back), 5 CALL
		FUN(&v1, print) CALL
		FUN(&v1, at), 2, & retVal2 CALL

		FUN(&v1, push_back), 6 CALL
		FUN(&v1, print) CALL
		FUN(&v1, at), 3, & retVal3 CALL

		FUN(&v1, push_back), 7 CALL
		FUN(&v1, print) CALL
		FUN(&v1, at), 4, & retVal4 CALL

		// Assert
		TEST_ASSERT(retVal == 3);
	TEST_ASSERT(retVal1 == 4);
	TEST_ASSERT(retVal2 == 5);
	TEST_ASSERT(retVal3 == 6);
	TEST_ASSERT(retVal4 == 7);

}END_FUN
FUN_IMPL(vector_at_throws_when_idx_is_outOfRange, int* tester)
{
	CREATE(vector, v1));
	(*tester) = 0;
	char feedback[3] = { 0, 0, 0 };
	int retVal = 0;

	CREATE(ScopeTester, s), feedback + 0);
	if (1)
	{
		SCOPE_START;
		// Cause throwing from within a func:
		FUN(&v1, at), 6, & retVal CALL
		(*tester)++;
		END_SCOPE;
	}
	(*tester)++;

}END_FUN
FUN_IMPL(vector_iteration_SanityTest)
{
	CREATE(vector, v1));

	CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY);

	FUN(&v1, push_back), 3 CALL
		FUN(&v1, push_back), 4 CALL
		FUN(&v1, push_back), 5 CALL
		FUN(&v1, push_back), 6 CALL


		int beginVal;
	CREATE(vectorIterator, vecItBegin), & v1, 2);
	FUN(&v1, begin), & vecItBegin CALL;
	FUN(&vecItBegin, getContentsOf), & beginVal CALL

		CREATE(vectorIterator, vecItEnd), & v1, 2);
		FUN(&v1, end), & vecItEnd CALL;

		bool isAtEnd = false;
		while (!isAtEnd)
		{
			SCOPE_START;
			int intermediateVal = 0;
			FUN(&vecItBegin, getContentsOf), & intermediateVal CALL
				printf("%d ", intermediateVal);

			FUN(&vecItBegin, increment) CALL
				FUN(&vecItBegin, equals), vecItEnd, & isAtEnd CALL;
			END_SCOPE;
		}


		//checks that begin is working properly
		TEST_ASSERT(beginVal == 3);

}END_FUN
