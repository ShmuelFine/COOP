#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("InheritTest", "[Inherit]")
{
	SECTION("Inheritence_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCalledAsWell")
	{
		CHECK_FALSE(Inheritence_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCalledAsWell());
	}
	SECTION("Inheritence_WhenVirtualFunctionOverriden_ThenCallInvokesOverridingFunc")
	{
		CHECK_FALSE(Inheritence_WhenVirtualFunctionOverriden_ThenCallInvokesOverridingFunc());
	}
	SECTION("Inheritence_WhenCallingNonVirtualFunciton_Sanity")
	{
		CHECK_FALSE(Inheritence_WhenCallingNonVirtualFunciton_Sanity());
	}
}