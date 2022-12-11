#ifndef __COOP_TESTS__MEM_MANAGEMENT_TEST_H__
#define __COOP_TESTS__MEM_MANAGEMENT_TEST_H__

#include "InMemoryCache.h"
#include "DynamicMemoryManagement.h"
#include "COOP.h"
#include "UnitTestsInfra.h"
#include "ScopeTester.h"

#include "BaseClassExample.h"

DEF_TEST_SUITE(MemoryManagementTest);
ADD_TEST(MemoryManagementTest, NEW_AllocatesSufficientMemory);
ADD_TEST(MemoryManagementTest, Delete_WhenDeletingAllocMemory_ThenItFreesTheResource);
END_TEST_SUITE(MemoryManagementTest);

#endif