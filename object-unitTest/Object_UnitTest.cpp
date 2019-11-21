#include "pch.h"
#include "LibLoadUtils.h"
#include "../COOP/ObjectTest.h"
#include "../Gmed/Mat.h"	

class ObjectTest : public ::testing::Test
{
public:
	HMODULE gmedLib;
	DEF_PTR_TO_FUNC(LOCAL_SCOPE_END__WhenDefiningObjectInside_ThenFreesThemAll);
	DEF_PTR_TO_FUNC(LOCAL_SCOPE_END__WhenDefiningObjectOutside_thenDoesntFreeThem);
	DEF_PTR_TO_FUNC(Mat_init);
	DEF_PTR_TO_FUNC(LOCAL_SCOPE_END__TheLocalScopeEnd_FreesOnlyTheLastInnerScopeThatIsntFreedYet);
	DEF_PTR_TO_FUNC(LOCAL_SCOPE_END__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash);

	virtual void SetUp()
	{
		gmedLib = LoadLibraryA("Gmed.dll");
		LOAD_FUNC(Mat_init);
		Mat_init_ptr();
		LOAD_FUNC(LOCAL_SCOPE_END__WhenDefiningObjectInside_ThenFreesThemAll);
		LOAD_FUNC(LOCAL_SCOPE_END__WhenDefiningObjectOutside_thenDoesntFreeThem);
		LOAD_FUNC(LOCAL_SCOPE_END__TheLocalScopeEnd_FreesOnlyTheLastInnerScopeThatIsntFreedYet);
		LOAD_FUNC(LOCAL_SCOPE_END__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash);
	}

	virtual void TearDown()
	{

	}
};

TEST_F(ObjectTest, LOCAL_SCOPE_END__WhenDefiningObjectInside_ThenFreesThemAll)
{
	ASSERT_EQ(LOCAL_SCOPE_END__WhenDefiningObjectInside_ThenFreesThemAll_ptr(), 1);
}

TEST_F(ObjectTest, LOCAL_SCOPE_END__WhenDefiningObjectOutside_thenDoesntFreeThem)
{
	ASSERT_EQ(LOCAL_SCOPE_END__WhenDefiningObjectOutside_thenDoesntFreeThem_ptr(), 1);
}

TEST_F(ObjectTest, LOCAL_SCOPE_END__TheLocalScopeEnd_FreesOnlyTheLastInnerScopeThatIsntFreedYet)
{
	ASSERT_EQ(LOCAL_SCOPE_END__TheLocalScopeEnd_FreesOnlyTheLastInnerScopeThatIsntFreedYet_ptr(), 1);
}

TEST_F(ObjectTest, LOCAL_SCOPE_END__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash)
{
	ASSERT_EQ(LOCAL_SCOPE_END__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash_ptr(), 1);
}