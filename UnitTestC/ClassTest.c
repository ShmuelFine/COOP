#include "ClassTest.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


int is_the_test_platform_run_as_well()
{
	//arrange
	int a = 10;
	int b = 11;
	TEST_ASSERT(a != b);

	//assert
	return TEST_PASS;

}

FUN_IMPL(Class_CallSimpleFunction) {
	// Arrange
	CREATE(BaseClassExample, B), 10, 3 CALL;
	// Act
	int retVal = 0;
	FUN(&B, GetVolume), & retVal CALL
		// Assert
		TEST_ASSERT(retVal == 10 * 3);
	
}END_FUN;

FUN_IMPL(Rectangle_SanityTest)
{
	// Arrange
	CREATE(Rectangle, Rect), 10, 3 CALL;

	// Act
	int retVal = 0;
	FUN(&Rect, GetArea), & retVal CALL

	// Assert
		TEST_ASSERT(retVal == 10 * 3);
	


}END_FUN


