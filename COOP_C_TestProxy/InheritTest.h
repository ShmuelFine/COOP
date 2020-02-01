
#include <stdbool.h>
#include "COOP.h"
#include "ProxyExportDefs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TEST_FAIL 10
#define TEST_PASS 20

COOP_API DECL_FUN(Inheritence_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCalledAsWell);
COOP_API DECL_FUN(Inheritence_WhenVirtualFunctionOverriden_ThenCallInvokesOverridingFunc);


#ifdef __cplusplus
}
#endif