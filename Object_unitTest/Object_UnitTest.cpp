#include "pch.h"


#include "../COOP_C_TestProxy/ObjectTest.h"


TEST(Object, LOCAL_SCOPE_END__WhenDefiningObjectInside_ThenFreesThemAll)
{
	ASSERT_TRUE(LOCAL_SCOPE_END__WhenDefiningObjectInside_ThenFreesThemAll());
}

TEST(Object, LOCAL_SCOPE_END__WhenDefiningObjectOutside_thenDoesntFreeThem)
{
	ASSERT_TRUE(LOCAL_SCOPE_END__WhenDefiningObjectOutside_thenDoesntFreeThem());
}

TEST(Object, LOCAL_SCOPE_END__TheLocalScopeEnd_FreesOnlyTheLastInnerScopeThatIsntFreedYet)
{
	ASSERT_TRUE(LOCAL_SCOPE_END__TheLocalScopeEnd_FreesOnlyTheLastInnerScopeThatIsntFreedYet());
}

TEST(Object, LOCAL_SCOPE_END__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash)
{
	ASSERT_TRUE(LOCAL_SCOPE_END__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash());
}

TEST(Object, New_WhenNew_ThenReturnesPointerInCache)
{
	ASSERT_TRUE(New_WhenNew_ThenReturnesPointerInCache());
}

TEST(Object, New_WhenNew_ConstructsBlockWithRightSize)
{
	ASSERT_TRUE(New_WhenNew_ConstructsBlockWithRightSize());
}

TEST(object, Delete_WhenDelete_PointerPointToNull)
{
	ASSERT_TRUE(Delete_WhenDelete_PointerPointToNull());
}

TEST(object, NEW_WhenAllocDifferentTypeThenChar_AllocatesTheRightSize)
{
	ASSERT_TRUE(NEW_WhenAllocDifferentTypeThenChar_AllocatesTheRightSize());
}

TEST(object, Delete_WhenDelete_ThenDeletesTheBlock)
{
	ASSERT_TRUE(Delete_WhenDelete_ThenDeletesTheBlock());
}

