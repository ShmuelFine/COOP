#include "coop.h"
#include "InheritTest.h"
#include "DerivedClassExample.h"

FUN_IMPL(Inheritence_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCalledAsWell)
{
	// Arrange, Act
	CREATE(DerivedClassExample, cube), 6, 8, 2);

	//Assert
	ASSERT(cube._base.hight == 6);
	ASSERT(cube._base.width == 8);

}
END_FUN


FUN_IMPL(Inheritence_WhenVirtualFunctionOverriden_ThenCallInvokesOverridingFunc)
{
	//Arrange
	int expectedVolume, actualVolume;

	CREATE(DerivedClassExample, cube), 4, 4, 2);

	//Act
	expectedVolume= cube._base.width * cube._base.hight * cube.depth;

	FUN(&cube, GetVolume), &actualVolume CALL

	//Assert
	ASSERT(expectedVolume == actualVolume);

}
END_FUN

FUN_IMPL(Inheritence_WhenCallingNonVirtualFunciton_Sanity)
{
	// Arrange
	CREATE(DerivedClassExample, cube), 4, 5, 6);
	// Act
	int retVal;
	FUN(&cube, JustReturn10), & retVal CALL;
	//Assert
	ASSERT(retVal == 10);

}
END_FUN

