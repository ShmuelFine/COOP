#include "pch.h"
#include "LibLoadUtils.h"
#include "../object/InheritTest.h"


class InheritTest : public ::testing::Test
{
public:
	HMODULE gmedLib;
	DEF_PTR_TO_FUNC(Ctor_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCallen);
	DEF_PTR_TO_FUNC(VTable_WhenDeriving_OverriddeesVTablePointer);
	DEF_PTR_TO_FUNC(VTable_WhenDriving_CanCallNoneOverridedBaseFunctionsViaOwnVTable);
	DEF_PTR_TO_FUNC(VTable_WhenDriving_NotOverridingTheBaseMembers);
	DEF_PTR_TO_FUNC(ChainInheritance_WhenDriving_NotOverridingTheBaseMembers);
	DEF_PTR_TO_FUNC(ChainInheritance_WhenDriving_AllBasesAreInited);
	DEF_PTR_TO_FUNC(ChainInheritance_Casting_EnabledCasting2Base);
	DEF_PTR_TO_FUNC(Overridding_WhenCallingAFunction_AlwaysCallsTheOvveridden);

	virtual void SetUp()
	{
		gmedLib = LoadLibraryA("object.dll");
		LOAD_FUNC(Ctor_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCallen);
		LOAD_FUNC(VTable_WhenDeriving_OverriddeesVTablePointer);
		LOAD_FUNC(VTable_WhenDriving_CanCallNoneOverridedBaseFunctionsViaOwnVTable);
		LOAD_FUNC(VTable_WhenDriving_NotOverridingTheBaseMembers);
		LOAD_FUNC(ChainInheritance_WhenDriving_NotOverridingTheBaseMembers);
		LOAD_FUNC(ChainInheritance_WhenDriving_AllBasesAreInited);
		LOAD_FUNC(ChainInheritance_Casting_EnabledCasting2Base);
		LOAD_FUNC(Overridding_WhenCallingAFunction_AlwaysCallsTheOvveridden);
	}

	virtual void TearDown()
	{

	}
};

TEST_F(InheritTest, Ctor_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCallen)
{
	ASSERT_TRUE(Ctor_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCallen_ptr());
}

TEST_F(InheritTest, VTable_WhenDeriving_OverriddeesVTablePointer)
{
	ASSERT_TRUE(VTable_WhenDeriving_OverriddeesVTablePointer_ptr());
}

TEST_F(InheritTest, VTable_WhenDriving_CanCallNoneOverridedBaseFunctionsViaOwnVTable)
{
	ASSERT_TRUE(VTable_WhenDriving_CanCallNoneOverridedBaseFunctionsViaOwnVTable_ptr());
}

TEST_F(InheritTest, VTable_WhenDriving_NotOverridingTheBaseMembers)
{
	ASSERT_TRUE(VTable_WhenDriving_NotOverridingTheBaseMembers_ptr());
}

//TEST_F(InheritTest, ChainInheritance_WhenDriving_NotOverridingTheBaseMembers)
//{
//	ASSERT_TRUE(ChainInheritance_WhenDriving_NotOverridingTheBaseMembers_ptr());
//}
//
//TEST_F(InheritTest, ChainInheritance_WhenDriving_AllBasesAreInited)
//{
//	ASSERT_TRUE(ChainInheritance_WhenDriving_AllBasesAreInited_ptr());
//}
//
//TEST_F(InheritTest, ChainInheritance_Casting_EnabledCasting2Base)
//{
//	ASSERT_TRUE(ChainInheritance_Casting_EnabledCasting2Base_ptr());
//}

TEST_F(InheritTest, Overridding_WhenCallingAFunction_AlwaysCallsTheOvveridden)
{
	ASSERT_TRUE(Overridding_WhenCallingAFunction_AlwaysCallsTheOvveridden_ptr());
}
