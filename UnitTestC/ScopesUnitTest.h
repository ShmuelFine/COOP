#ifndef __COOP__SCOPES_TEST_H__
#define __COOP__SCOPES_TEST_H__

#include "UnitTestsInfra.h"
#include "COOP.h"
#include "InMemoryCache.h"
#include "DynamicMemoryManagement.h"
#include "ScopeTester.h"

DEF_TEST_SUITE(Infra_ScopesTest);
ADD_TEST(Infra_ScopesTest, SCOPE_END__WhenObjectsDefinedInsideScope_ThenAllGetFreed);
ADD_TEST(Infra_ScopesTest, LOCAL_SCOPE__WhenObjectsDefinedInside_InnerScope_ThenAllGetFreed);
ADD_TEST(Infra_ScopesTest, LOCAL_SCOPE__DoesNotFreeUnrelatedObjects);
ADD_TEST(Infra_ScopesTest, LOCAL_SCOPE__WhenMultipleNestedScopesExist_ThenFreesOnlyTheInnerMost);
ADD_TEST(Infra_ScopesTest, LOCAL_SCOPE__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash);
ADD_TEST(Infra_ScopesTest, LOCAL_SCOPE__LoopScopeSanityTest);
END_TEST_SUITE(Infra_ScopesTest);

#endif