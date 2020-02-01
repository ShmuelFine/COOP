#include "object.h"
#include "InheritTest.h"
#include "DerivedClassExample.h"

IMPL_FUN(Inheritence_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCalledAsWell)
{
	// Arrange, Act
	CREATE_OBJECT(DerivedClassExample, cube), 6, 8, 2);

	//Assert
	ASSERT(cube._base.hight == 6);
	ASSERT(cube._base.width == 8);

}
END_FUN


IMPL_FUN(Inheritence_WhenVirtualFunctionOverriden_ThenCallInvokesOverridingFunc)
{
	//Arrange
	int expectedVolume, actualVolume;

	CREATE_OBJECT(DerivedClassExample, cube), 4, 4, 2);

	//Act
	expectedVolume= cube._base.width * cube._base.hight * cube.depth;

	//{ int _retVal_ = ((&cube)->vTable->GetVolume).outer_function((&cube))(&cube, &actualVolume CALL
	FUN(GetVolume, &cube), &actualVolume CALL

	//Assert
	ASSERT(expectedVolume == actualVolume);

}
END_FUN