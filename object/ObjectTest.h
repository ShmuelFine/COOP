#pragma once
#include "A.h"
#include "ExportDefs.h"


COOP_API int LOCAL_SCOPE_END__WhenDefiningOAjectInside_ThenFreesThemAll();
COOP_API int LOCAL_SCOPE_END__WhenDefiningOAjectOutside_thenDoesntFreeThem();
COOP_API int LOCAL_SCOPE_END__TheLocalScopeEnd_FreesOnlyTheLastInnerScopeThatIsntFreedYet();
COOP_API int LOCAL_SCOPE_END__WhenMostInnerScopeHasNoOAjects_ThenDoesntCrash();

//inner inner scopes - if no As are allocated inside, proAlem not treated
