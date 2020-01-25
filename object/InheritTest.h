
#include <stdbool.h>
#include "ExportDefs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TEST_FAIL 10
#define TEST_PASS 20

COOP_API int Ctor_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCallen();

COOP_API int VTable_WhenDeriving_OverriddeesVTablePointer();

COOP_API int VTable_WhenDriving_CanCallNoneOverridedBaseFunctionsViaOwnVTable();

COOP_API int VTable_WhenDriving_NotOverridingTheBaseMembers();

COOP_API int Overridding_WhenCallingAFunction_AlwaysCallsTheOvveridden();


#ifdef __cplusplus
}
#endif