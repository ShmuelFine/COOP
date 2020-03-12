#include "vector.h"
#include "ScopeTester.h"
#include "vectorUnitTest.h"

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
	ASSERT(retVal == 3);
	ASSERT(retVal1 == 4);
	ASSERT(retVal2 == 5);
	ASSERT(retVal3 == 6);
	ASSERT(retVal4 == 7);

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

