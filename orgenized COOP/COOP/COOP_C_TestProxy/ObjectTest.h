
#include "ProxyExportDefs.h"

//
//
//COOP_API int LOCAL_SCOPE_END__WhenDefiningObjectInside_ThenFreesThemAll();
//COOP_API int LOCAL_SCOPE_END__WhenDefiningObjectOutside_thenDoesntFreeThem();
//COOP_API int LOCAL_SCOPE_END__TheLocalScopeEnd_FreesOnlyTheLastInnerScopeThatIsntFreedYet();
//COOP_API int LOCAL_SCOPE_END__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash();
//
////inner inner scopes - if no As are allocated inside, proAlem not treated

#ifdef __cplusplus
extern "C" {
#endif

	COOP_API int LOCAL_SCOPE_END__WhenDefiningObjectInside_ThenFreesThemAll();

	COOP_API int LOCAL_SCOPE_END__WhenDefiningObjectOutside_thenDoesntFreeThem();

	COOP_API int LOCAL_SCOPE_END__TheLocalScopeEnd_FreesOnlyTheLastInnerScopeThatIsntFreedYet();

	COOP_API int LOCAL_SCOPE_END__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash();

	COOP_API int New_WhenNew_ThenReturnesPointerInCache();

	COOP_API int New_WhenNew_ConstructsBlockWithRightSize();

	COOP_API int Delete_WhenDelete_PointerPointToNull();

	COOP_API int Delete_WhenDelete_ThenDeletesTheBlock();

	COOP_API int NEW_WhenAllocDifferentTypeThenChar_AllocatesTheRightSize();

#ifdef __cplusplus
}
#endif