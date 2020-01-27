
#include <stdbool.h>
#include "ProxyExportDefs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TEST_FAIL 10
#define TEST_PASS 20

COOP_PROXY_API int Ctor_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCallen();

COOP_PROXY_API int VTable_WhenDeriving_OverriddeesVTablePointer();

COOP_PROXY_API int VTable_WhenDriving_CanCallNoneOverridedBaseFunctionsViaOwnVTable();

COOP_PROXY_API int VTable_WhenDriving_NotOverridingTheBaseMembers();

COOP_PROXY_API int Overridding_WhenCallingAFunction_AlwaysCallsTheOvveridden();


#ifdef __cplusplus
}
#endif