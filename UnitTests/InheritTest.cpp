#include "pch.h"
#include "InheritTest.h"

TEST(InheritTest, Inheritence_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCalledAsWell)	   { ASSERT_FALSE(Inheritence_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCalledAsWell());  }
TEST(InheritTest, Inheritence_WhenVirtualFunctionOverriden_ThenCallInvokesOverridingFunc) { ASSERT_FALSE(Inheritence_WhenVirtualFunctionOverriden_ThenCallInvokesOverridingFunc()); }
TEST(InheritTest, Inheritence_WhenCallingNonVirtualFunciton_Sanity)  { ASSERT_FALSE(Inheritence_WhenCallingNonVirtualFunciton_Sanity());}
