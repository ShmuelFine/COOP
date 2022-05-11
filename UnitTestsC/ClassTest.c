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
	//act 


	//assert
	return TEST_PASS;

}
int Class_CallSimpleFunction_SanityTest() 
{	
		// Arrange
		CREATE(BaseClassExample, B), 10, 3);
		// Act
		int retVal = 0;
		FUN(&B, GetVolume), & retVal CALL
			// Assert
		TEST_ASSERT(retVal == 10 * 3);
}


int main() {
	INIT_TESTS();
	RUN_TEST(is_the_test_platform_run_as_well);
	
}

