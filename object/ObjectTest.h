#pragma once
#include "Mat.h"
#include "ExportDefs.h"


COOP_API int LOCAL_SCOPE_END__WhenDefiningObjectInside_ThenFreesThemAll();
COOP_API int LOCAL_SCOPE_END__WhenDefiningObjectOutside_thenDoesntFreeThem();
COOP_API int LOCAL_SCOPE_END__TheLocalScopeEnd_FreesOnlyTheLastInnerScopeThatIsntFreedYet();
COOP_API int LOCAL_SCOPE_END__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash();

//inner inner scopes - if no mats are allocated inside, problem not treated