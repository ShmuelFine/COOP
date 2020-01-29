#include "object.h"
#include "InheritTest.h"
#include "DerivedClassExample.h"

int Inheritence_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCalledAsWell()
{
	SCOPE_START;
	
	// Arrange, Act
	CREATE_OBJECT(DerivedClassExample, cube), 6, 8, 2);

	//Assert
	ASSERT(cube._base.hight == 6);
	ASSERT(cube._base.width == 8);

	SCOPE_END;
}

int Inheritence_WhenVirtualFunctionOverriden_ThenCallInvokesOverridingFunc()
{
	SCOPE_START;

	//Arrange
	int expectedLoc, actualLoc;

	CREATE_OBJECT(DerivedClassExample, cube), 4, 4, 2);

	//Act
	expectedLoc = cube._base.width * cube._base.hight * cube.depth;

	FUN(GetVolume, &cube), 1, 2, &actualLoc);

	//Assert
	ASSERT(expectedLoc == actualLoc);

	SCOPE_END;

}