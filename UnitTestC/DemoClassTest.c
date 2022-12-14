#include "DemoClassTest.h"
#include "BaseClassExample.h"
#include "Rectangle.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

TEST_FUN_IMPL(DemoClassTest, Class_CallSimpleFunction) {
	// Arrange
	CREATE(BaseClassExample, B), 10, 3 CALL;
	// Act
	int retVal = 0;
	MFUN(&B, GetVolume), & retVal CALL;
	// Assert
	NTEST_ASSERT(retVal == 10 * 3);

}END_FUN;

TEST_FUN_IMPL(DemoClassTest, Rectangle_SanityTest)
{
	// Arrange
	CREATE(Rectangle, Rect), 10, 3 CALL;

	// Act
	int retVal = 0;
	MFUN(&Rect, GetArea), & retVal CALL;

	// Assert
	NTEST_ASSERT(retVal == 10 * 3);

}END_FUN

INIT_TEST_SUITE(DemoClassTest)
BIND_TEST(DemoClassTest, Class_CallSimpleFunction)
BIND_TEST(DemoClassTest, Rectangle_SanityTest)
END_INIT_TEST_SUITE(DemoClassTest)
