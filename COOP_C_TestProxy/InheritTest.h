
#include <stdbool.h>
#include "COOP.h"
#include "ProxyExportDefs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TEST_FAIL 10
#define TEST_PASS 20

COOP_API FUN_DECL(Inheritence_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCalledAsWell);
COOP_API FUN_DECL(Inheritence_WhenVirtualFunctionOverriden_ThenCallInvokesOverridingFunc);
COOP_API FUN_DECL(Inheritence_WhenCallingNonVirtualFunciton_Sanity);

#ifdef __cplusplus
}
#endif