#pragma once
#include "SuperMat4Test.h"
#include <stdbool.h>


COOP_API bool Ctor_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCallen();

COOP_API bool VTable_WhenDeriving_OverriddeesVTablePointer();

COOP_API bool VTable_WhenDriving_CanCallBaseFunctionsViaOwnVTable();


