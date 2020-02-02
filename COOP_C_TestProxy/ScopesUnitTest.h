
#include "ProxyExportDefs.h"
#include "COOP.h"

#ifdef __cplusplus
extern "C" {
#endif

	COOP_API FUN_DECL(SCOPE_END__WhenObjectsDefinedInsideScope_ThenAllGetFreed);
	COOP_API FUN_DECL(LOCAL_SCOPE__WhenObjectsDefinedInside_InnerScope_ThenAllGetFreed);
	COOP_API FUN_DECL(LOCAL_SCOPE__DoesNotFreeUnrelatedObjects);
	COOP_API FUN_DECL(LOCAL_SCOPE__WhenMultipleNestedScopesExist_ThenFreesOnlyTheInnerMost);
	COOP_API FUN_DECL(LOCAL_SCOPE__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash);
	COOP_API FUN_DECL(LOCAL_SCOPE__LoopScopeSanityTest);

#ifdef __cplusplus
}
#endif