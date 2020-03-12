#include "ClassUnitTest.h"
#include "BaseClassExample.h"
#include "vector.h"

FUN_IMPL(vector_push_back_SanityTest)
{
	// Arrange
	CREATE(vector, v1));

	CREATE(MemoryManager, memManager), sizeof(int) * 2, HEAP_BASED_MEMORY);

	// Act
	int retVal = 0;
	FUN(&v1, push_back), 1 CALL
	FUN(&v1, at), 0, & retVal CALL

	// Assert
	ASSERT(retVal == 1);

}END_FUN

FUN_IMPL(vector_at_throws_when_idx_is_outOfRange)
{
	// Arrange
	CREATE(vector, v1));

	// Act
	int retVal = 0;

	FUN(&v1, at), 6, & retVal CALL

	// Assert-TODO: how to check if it throws
	ASSERT(retVal == 1);

}END_FUN