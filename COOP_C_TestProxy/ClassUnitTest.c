#include "ClassUnitTest.h"
#include "BaseClassExample.h"

int Class_CallSimpleFunction_SanityTest()
{
	// Arrange
	SCOPE_START;
	
	CREATE_OBJECT(BaseClassExample, B), 10, 3);
	
	// Act
	int retVal = 0;
	FUN(GetVolume, &B), &retVal);
	
	// Assert
	ASSERT(retVal == 10 * 3);

	SCOPE_END;
}