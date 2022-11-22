#include "test_defs.h"
#include "../COOP/COOP.h"
#include "../COOP/Cache.h"
#include "../COOP/dynamic_memory_management.h"
#include "ScopeTester.h"

//// #include "../COOP_C_TestProxy/ScopeTester.h"

FUN_DECL(SCOPE_END__WhenObjectsDefinedInsideScope_ThenAllGetFreed);
FUN_DECL(LOCAL_SCOPE__WhenObjectsDefinedInside_InnerScope_ThenAllGetFreed);
FUN_DECL(LOCAL_SCOPE__DoesNotFreeUnrelatedObjects);
FUN_DECL(LOCAL_SCOPE__WhenMultipleNestedScopesExist_ThenFreesOnlyTheInnerMost);
FUN_DECL(LOCAL_SCOPE__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash);
FUN_DECL(LOCAL_SCOPE__LoopScopeSanityTest);
FUN_DECL(Shared_ptr__CopyTo__PointsOnTheSameValue);