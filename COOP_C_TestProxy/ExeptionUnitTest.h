#include "../COOP/object.h"
#include "ProxyExportDefs.h"



#ifdef __cplusplus
extern "C"{
#endif

COOP_API int Exception_WhenThrown_ThenGoesStraightToScopeEnd();

COOP_API int Exception_WhenThrownFromWithinFunction_ThenGoesStraightToScopeEnd();

COOP_API int CATCH_WhenExeptionCaught_Does_NOT_ContinueThrowing();

#ifdef __cplusplus
}
#endif