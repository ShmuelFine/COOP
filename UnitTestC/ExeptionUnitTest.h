#ifndef __COOP_TESTS__EXCEPTION_UNIT_TEST__H__
#define __COOP_TESTS__EXCEPTION_UNIT_TEST__H__

#include "DynamicMemoryManagement.h"
#include <string.h>
#include "UnitTestsInfra.h"
#include "COOP.h"
#include "ScopeTester.h"

DEF_TEST_SUITE(ExceptionTests);
ADD_TEST(ExceptionTests, WhenThrown_ThenGoesStraightToScopeEnd);
ADD_TEST(ExceptionTests, WhenThrownFromWithinFunction_ThenGoesStraightToScopeEnd);
ADD_TEST(ExceptionTests, CATCH_WhenExeptionCaught_Does_NOT_ContinueThrowing);
ADD_TEST(ExceptionTests, THROW_MSG_SavesTheMessage);
ADD_TEST(ExceptionTests, BREAK_WhenDoneFromNastedLoop_ThenBreaksCorrectly);
ADD_TEST(ExceptionTests, BREAK_WhenDoneFromTRY_Catch_Block_ThenBreaksCorrectly);
ADD_TEST(ExceptionTests, BREAK_WhenDoneFromLoop_ThenFreesObjectsFromInnerScope);
END_TEST_SUITE(ExceptionTests);

#endif