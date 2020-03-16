#include "pch.h"


#include "ScopesUnitTest.h"

TEST(Scopes, SCOPE_END__WhenObjectsDefinedInsideScope_ThenAllGetFreed)
{
	ASSERT_FALSE(SCOPE_END__WhenObjectsDefinedInsideScope_ThenAllGetFreed());
}

TEST(Scopes, LOCAL_SCOPE__WhenObjectsDefinedInside_InnerScope_ThenAllGetFreed)
{
	ASSERT_FALSE(LOCAL_SCOPE__WhenObjectsDefinedInside_InnerScope_ThenAllGetFreed());
}

TEST(Scopes, LOCAL_SCOPE__DoesNotFreeUnrelatedObjects)
{
	ASSERT_FALSE(LOCAL_SCOPE__DoesNotFreeUnrelatedObjects());
}

TEST(Scopes, LOCAL_SCOPE__WhenMultipleNestedScopesExist_ThenFreesOnlyTheInnerMost)
{
	ASSERT_FALSE(LOCAL_SCOPE__WhenMultipleNestedScopesExist_ThenFreesOnlyTheInnerMost());
}

TEST(Scopes, LOCAL_SCOPE__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash)
{
	ASSERT_FALSE(LOCAL_SCOPE__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash());
}

TEST(Scopes, LOCAL_SCOPE__LoopScopeSanityTest)
{
	ASSERT_FALSE(LOCAL_SCOPE__LoopScopeSanityTest());
}


TEST(Shared_ptr, Shared_ptr__CopyTo__PointsOnTheSameValue)
{
	ASSERT_FALSE(Shared_ptr__CopyTo__PointsOnTheSameValue());
}