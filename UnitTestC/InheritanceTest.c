#include "InheritanceTest.h"


TEST_FUN_IMPL(InheritanceTest, WhenCallingCtorOfDrivedClass_BaseClassCtorIsCalledAsWell)
{
	// Arrange, Act
	CREATE(DerivedClassExample, cube), 6, 8, 2 CALL;

	//Assert
	NTEST_ASSERT(cube._base.hight == 6);
	NTEST_ASSERT(cube._base.width == 8);

}
END_FUN


TEST_FUN_IMPL(InheritanceTest, WhenVirtualFunctionOverriden_ThenCallInvokesOverridingFunc)
{
	//Arrange
	int expectedVolume, actualVolume;

	CREATE(DerivedClassExample, cube), 4, 4, 2 CALL;

	//Act
	expectedVolume = cube._base.width * cube._base.hight * cube.depth;

	FUN(&cube, GetVolume), & actualVolume CALL;

	//Assert
	NTEST_ASSERT(expectedVolume == actualVolume);

}
END_FUN

TEST_FUN_IMPL(InheritanceTest, WhenCallingNonVirtualFunciton_Sanity)
{
	// Arrange
	CREATE(DerivedClassExample, cube), 4, 5, 6 CALL;
	// Act
	int retVal;
	FUN(&cube, JustReturn10), & retVal CALL;
	//Assert
	NTEST_ASSERT(retVal == 10);

}
END_FUN


INIT_TEST_SUITE(InheritanceTest);
BIND_TEST(InheritanceTest, WhenCallingCtorOfDrivedClass_BaseClassCtorIsCalledAsWell);
BIND_TEST(InheritanceTest, WhenVirtualFunctionOverriden_ThenCallInvokesOverridingFunc);
BIND_TEST(InheritanceTest, WhenCallingNonVirtualFunciton_Sanity);
END_INIT_TEST_SUITE(InheritanceTest);