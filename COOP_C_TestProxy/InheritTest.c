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
	//expectedLoc = (mat._base.width * 1 + 2) * mat.step;
	(*(cube.vTable)).GetVolume(&actualLoc);

	CALL(cube.vTable->GetVolume, , 1, 2, &actualLoc);

	//Assert
	ASSERT_EQ(expectedLoc, actualLoc);

	SCOPE_END;

}