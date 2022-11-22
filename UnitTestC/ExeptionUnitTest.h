// #include "../COOP_C_TestProxy/ScopeTester.h"
#include "../COOP/dynamic_memory_management.h"
#include <string.h>
#include "test_defs.h"
#include "../COOP/COOP.h"
#include "ScopeTester.h"

FUN_DECL(Exception_WhenThrown_ThenGoesStraightToScopeEnd, int* tester);
FUN_DECL(Exception_WhenThrownFromWithinFunction_ThenGoesStraightToScopeEnd, int* tester);
FUN_DECL(CATCH_WhenExeptionCaught_Does_NOT_ContinueThrowing);
FUN_DECL(Exception_WhenUsingTHROW_MSG_ThenTheMessageIsSaved, const char* whatToThrow, char* outThrowingMsg);
FUN_DECL(BREAK_WhenDoneFromNastedLoop_ThenBreaksCorrectly);
FUN_DECL(BREAK_WhenDoneFromTRY_Catch_Block_ThenBreaksCorrectly);
FUN_DECL(BREAK_WhenDoneFromLoop_ThenFreesObjectsFromInnerScope);

