#include "../COOP/Cache.h"
#include "../COOP/dynamic_memory_management.h"
#include "../COOP/COOP.h"
#include "test_defs.h"
#include "../COOP_C_TestProxy/ScopeTester.h"
#include "../COOP_C_TestProxy/BaseClassExample.h"

FUN_DECL(New_WhenAllocatingBuffer_ThenReturnsRightAmountOfMemroy);
FUN_DECL(New_WhenAllocatingClassInstance_ThenReturnsRightAmountOfMemroy);
FUN_DECL(Delete_WhenDeletingAllocMemory_ThenItFreesTheResource);

