#include "object.h"
#include "ExportDefs.h"



#ifdef __cplusplus
extern "C"{
#endif

COOP_API int WhenexeptionIsThrown_ThenGoesStriehtToScopeEnd();

COOP_API int WhenexeptionIsThrownWithInAFunctionCall_ThenGoesStriehtToScopeEnd();

COOP_API int WhenCatchingExeption_DoesntContinueThrowing();

#ifdef __cplusplus
}
#endif