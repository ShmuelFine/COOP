#include "ClassUnitTest.h"
#include "COOP.h"
#include "Shared_ptr.h"
#include "ScopesUnitTest.h"

FUN_IMPL(check_type)
{
	// Arrange
	CREATE(MemoryManager, memManager), sizeof(int) * 20, STACK_BASED_MEMORY);


	CREATE(Shared_ptr, ptr));
	CREATE(Shared_ptr, ptr2));

	float* i = NULL;
	NEW(i, float);
	*i = 10.5;

	float* j = NULL;
	NEW(j, float);
	*j = 300.3;

	FUN(&ptr, Reset), i CALL;//10.5
	FUN(&ptr2, Reset), j CALL;//300.3

	//int* actualValue = ((int*)(ptr2.px));
	float tst1 = *GET(ptr2, float);
	bool isOK = (300.3 - 0.01 < tst1) && (tst1 < 300.3 + 0.01);
	//IS_IN_RANGE(tst1, 300 + 1, 300 - 1);
	//IS_FLOATS_EQ(F1, F2, tolerance)
	ASSERT(isOK)
	// Act 
	FUN(&ptr, CopyTo), & ptr2 CALL;


	FUN(&ptr2, CopyFrom), & ptr CALL;

	// Assert
	ASSERT(*GET(ptr2, float) == 10.5);

	*i = 12.5;
	ASSERT(*GET(ptr, float) == 12.5);
	ASSERT(*GET(ptr2, float) == 12.5);

	bool out;
	FUN(&ptr, IsEmpty), & out CALL;
	ASSERT(!(out == 0));

	FUN(&ptr, Release) CALL;

}END_FUN