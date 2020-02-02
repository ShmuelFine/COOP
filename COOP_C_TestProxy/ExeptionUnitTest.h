#include "coop.h"
#include "ProxyExportDefs.h"



#ifdef __cplusplus
extern "C"{
#endif

COOP_API FUN_DECL(Exception_WhenThrown_ThenGoesStraightToScopeEnd, int* tester);
COOP_API FUN_DECL(Exception_WhenThrownFromWithinFunction_ThenGoesStraightToScopeEnd, int* tester);
COOP_API FUN_DECL(CATCH_WhenExeptionCaught_Does_NOT_ContinueThrowing);
COOP_API FUN_DECL(Exception_WhenUsingTHROW_MSG_ThenTheMessageIsSaved, const char * whatToThrow, char * outThrowingMsg);

#ifdef __cplusplus
}
#endif