#include "test_defs.h"
#include "../COOP/COOP.h"
#include "DerivedClassExample.h"
//#include "../COOP_C_TestProxy/DerivedClassExample.h"

DEF_TEST_SUITE(InheritanceTest);
ADD_TEST(InheritanceTest, WhenCallingCtorOfDrivedClass_BaseClassCtorIsCalledAsWell);
ADD_TEST(InheritanceTest, WhenVirtualFunctionOverriden_ThenCallInvokesOverridingFunc);
ADD_TEST(InheritanceTest, WhenCallingNonVirtualFunciton_Sanity);
END_TEST_SUITE(InheritanceTest);