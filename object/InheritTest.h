#pragma once
#include "SuperMat4Test.h"
#include "SuperMat3_4Test.h"
#include <stdbool.h>


COOP_API bool Ctor_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCallen();

COOP_API bool VTable_WhenDeriving_OverriddeesVTablePointer();

COOP_API bool VTable_WhenDriving_CanCallBaseFunctionsViaOwnVTable();

COOP_API bool VTable_WhenDriving_NotOverridingTheBaseMembers();

COOP_API bool ChainInheritance_WhenDriving_AllBasesAreInited();

COOP_API bool ChainInheritance_WhenDriving_NotOverridingTheBaseMembers();

COOP_API bool ChainInheritance_Casting_EnabledCasting2Base();


