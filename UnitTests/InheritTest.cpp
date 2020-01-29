#include "pch.h"
#include "InheritTest.h"

TEST(InheritTest, Inheritence_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCalledAsWell)	   { ASSERT_TRUE(Inheritence_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCalledAsWell());  }
TEST(InheritTest, Inheritence_WhenVirtualFunctionOverriden_ThenCallInvokesOverridingFunc)  { ASSERT_TRUE(Inheritence_WhenVirtualFunctionOverriden_ThenCallInvokesOverridingFunc());}
