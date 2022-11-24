#ifndef __COOP_TESTS__INHERITANCE_TEST__H__
#define __COOP_TESTS__INHERITANCE_TEST__H__

#include "test_defs.h"
#include "../COOP/COOP.h"
#include "DerivedClassExample.h"

DEF_TEST_SUITE(InheritanceTest);
ADD_TEST(InheritanceTest, WhenCallingCtorOfDrivedClass_BaseClassCtorIsCalledAsWell);
ADD_TEST(InheritanceTest, WhenVirtualFunctionOverriden_ThenCallInvokesOverridingFunc);
ADD_TEST(InheritanceTest, WhenCallingNonVirtualFunciton_Sanity);
END_TEST_SUITE(InheritanceTest);

#endif