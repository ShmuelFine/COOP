
#include "ProxyExportDefs.h"
#include "COOP.h"

#ifdef __cplusplus
extern "C" {
#endif

	COOP_API DECL_FUN(SCOPE_END__WhenObjectsDefinedInsideScope_ThenAllGetFreed);
	COOP_API DECL_FUN(LOCAL_SCOPE_END__WhenObjectsDefinedInside_InnerScope_ThenAllGetFreed);
	COOP_API DECL_FUN(LOCAL_SCOPE_END__DoesNotFreeUnrelatedObjects);
	COOP_API DECL_FUN(LOCAL_SCOPE_END__WhenMultipleNestedScopesExist_ThenFreesOnlyTheInnerMost);
	COOP_API DECL_FUN(LOCAL_SCOPE_END__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash);

#ifdef __cplusplus
}
#endif