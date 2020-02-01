#include "pch.h"


#include "ScopesUnitTest.h"

TEST(Object, SCOPE_END__WhenObjectsDefinedInsideScope_ThenAllGetFreed)
{
	ASSERT_FALSE(SCOPE_END__WhenObjectsDefinedInsideScope_ThenAllGetFreed());
}

TEST(Object, LOCAL_SCOPE_END__WhenObjectsDefinedInside_InnerScope_ThenAllGetFreed)
{
	ASSERT_FALSE(LOCAL_SCOPE_END__WhenObjectsDefinedInside_InnerScope_ThenAllGetFreed());
}

TEST(Object, LOCAL_SCOPE_END__DoesNotFreeUnrelatedObjects)
{
	ASSERT_FALSE(LOCAL_SCOPE_END__DoesNotFreeUnrelatedObjects());
}

TEST(Object, LOCAL_SCOPE_END__WhenMultipleNestedScopesExist_ThenFreesOnlyTheInnerMost)
{
	ASSERT_FALSE(LOCAL_SCOPE_END__WhenMultipleNestedScopesExist_ThenFreesOnlyTheInnerMost());
}

TEST(Object, LOCAL_SCOPE_END__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash)
{
	ASSERT_FALSE(LOCAL_SCOPE_END__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash());
}