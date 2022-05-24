#include "runTest.h"
int main() {
	INIT_TESTS();
	RUN_TEST(is_the_test_platform_run_as_well);
	RUN_TEST(Class_CallSimpleFunction);
	RUN_TEST(Rectangle_SanityTest);
	RUN_TEST(Inheritence_WhenCallingNonVirtualFunciton_Sanity);
	RUN_TEST(Inheritence_WhenVirtualFunctionOverriden_ThenCallInvokesOverridingFunc);
	RUN_TEST(Inheritence_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCalledAsWell);

}
