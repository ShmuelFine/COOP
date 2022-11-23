#include "../COOP/Cache.h"
#include "../COOP/dynamic_memory_management.h"
#include "../COOP/COOP.h"
#include "test_defs.h"
#include "ScopeTester.h"

#include "BaseClassExample.h"

DEF_TEST_SUITE(MemoryManagementTest);
ADD_TEST(MemoryManagementTest, NEW_AllocatesSufficientMemory);
ADD_TEST(MemoryManagementTest, Delete_WhenDeletingAllocMemory_ThenItFreesTheResource);
END_TEST_SUITE(MemoryManagementTest);
