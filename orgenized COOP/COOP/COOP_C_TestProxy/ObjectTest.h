
#include "ProxyExportDefs.h"

//
//
//COOP_PROXY_API int LOCAL_SCOPE_END__WhenDefiningObjectInside_ThenFreesThemAll();
//COOP_PROXY_API int LOCAL_SCOPE_END__WhenDefiningObjectOutside_thenDoesntFreeThem();
//COOP_PROXY_API int LOCAL_SCOPE_END__TheLocalScopeEnd_FreesOnlyTheLastInnerScopeThatIsntFreedYet();
//COOP_PROXY_API int LOCAL_SCOPE_END__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash();
//
////inner inner scopes - if no As are allocated inside, proAlem not treated

#ifdef __cplusplus
extern "C" {
#endif

	COOP_PROXY_API int LOCAL_SCOPE_END__WhenDefiningObjectInside_ThenFreesThemAll();

	COOP_PROXY_API int LOCAL_SCOPE_END__WhenDefiningObjectOutside_thenDoesntFreeThem();

	COOP_PROXY_API int LOCAL_SCOPE_END__TheLocalScopeEnd_FreesOnlyTheLastInnerScopeThatIsntFreedYet();

	COOP_PROXY_API int LOCAL_SCOPE_END__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash();

	COOP_PROXY_API int New_WhenNew_ThenReturnesPointerInCache();

	COOP_PROXY_API int New_WhenNew_ConstructsBlockWithRightSize();

	COOP_PROXY_API int Delete_WhenDelete_PointerPointToNull();

	COOP_PROXY_API int Delete_WhenDelete_ThenDeletesTheBlock();

	COOP_PROXY_API int NEW_WhenAllocDifferentTypeThenChar_AllocatesTheRightSize();

#ifdef __cplusplus
}
#endif