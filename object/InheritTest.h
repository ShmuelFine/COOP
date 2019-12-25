
#include "SuperMat3_4Test.h"
#include <stdbool.h>


COOP_API bool Ctor_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCallen();

COOP_API bool VTable_WhenDeriving_OverriddeesVTablePointer();

COOP_API bool VTable_WhenDriving_CanCallNoneOverridedBaseFunctionsViaOwnVTable();

COOP_API bool VTable_WhenDriving_NotOverridingTheBaseMembers();

COOP_API bool ChainInheritance_WhenDriving_AllBasesAreInited();

COOP_API bool ChainInheritance_WhenDriving_NotOverridingTheBaseMembers();

COOP_API bool ChainInheritance_Casting_EnabledCasting2Base();

COOP_API bool Overridding_WhenCallingAFunction_AlwaysCallsTheOvveridden();


