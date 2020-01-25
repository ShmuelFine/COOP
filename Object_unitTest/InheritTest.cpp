#include "pch.h"
#include "LibLoadUtils.h"
#include "../object/InheritTest.h"


class InheritTest : public ::testing::Test
{
public:

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{

	}
};

TEST_F(InheritTest, Ctor_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCallen)
{
	ASSERT_TRUE(TEST_FAIL != Ctor_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCallen());
}

TEST_F(InheritTest, VTable_WhenDeriving_OverriddeesVTablePointer)
{
	ASSERT_TRUE(TEST_FAIL != VTable_WhenDeriving_OverriddeesVTablePointer());
}

TEST_F(InheritTest, VTable_WhenDriving_CanCallNoneOverridedBaseFunctionsViaOwnVTable)
{
	ASSERT_TRUE(TEST_FAIL != VTable_WhenDriving_CanCallNoneOverridedBaseFunctionsViaOwnVTable());
}

TEST_F(InheritTest, VTable_WhenDriving_NotOverridingTheBaseMembers)
{
	ASSERT_TRUE(TEST_FAIL != VTable_WhenDriving_NotOverridingTheBaseMembers());
}

TEST_F(InheritTest, Overridding_WhenCallingAFunction_AlwaysCallsTheOvveridden)
{
	ASSERT_TRUE(TEST_FAIL != Overridding_WhenCallingAFunction_AlwaysCallsTheOvveridden());
}
