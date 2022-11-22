#include "ClassUnitTest.h"
#include "BaseClassExample.h"
#include "Rectangle.h"

FUN_IMPL(Class_CallSimpleFunction_SanityTest)
{
	// Arrange
	CREATE(BaseClassExample, B), 10, 3);
	
	// Act
	int retVal = 0;
	FUN(&B, GetVolume), &retVal CALL
	
	// Assert
	ASSERT(retVal == 10 * 3);

}END_FUN


FUN_IMPL(Rectangle_SanityTest)
{
	// Arrange
	CREATE(Rectangle, Rect), 10, 3);

	// Act
	int retVal = 0;
	FUN(&Rect, GetArea), & retVal CALL;
	
	// Assert
	ASSERT(retVal == 10 * 3);

}END_FUN
