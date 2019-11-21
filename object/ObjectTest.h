#pragma once
//#include "Mat.h"
#include "Globals.h"
#include "ExportDefs.h"
#include "object.h"

GMED_API int LOCAL_SCOPE_END__WhenDefiningObjectInside_ThenFreesThemAll();
GMED_API int LOCAL_SCOPE_END__WhenDefiningObjectOutside_thenDoesntFreeThem();
GMED_API int LOCAL_SCOPE_END__TheLocalScopeEnd_FreesOnlyTheLastInnerScopeThatIsntFreedYet();
GMED_API int LOCAL_SCOPE_END__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash();

//inner inner scopes - if no mats are allocated inside, problem not treated