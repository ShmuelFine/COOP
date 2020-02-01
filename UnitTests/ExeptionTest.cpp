#include "pch.h"

#include "ExeptionUnitTest.h"
#include <iostream>
#include <fstream>

// Comparing to ERROR_VALUE is equivalent to ASSER_THROW of a kind.
TEST(Exceptions, Exception_WhenThrown_ThenGoesStraightToScopeEnd)					
{
    int tester;
    auto ret = Exception_WhenThrown_ThenGoesStraightToScopeEnd(&tester);
    ASSERT_EQ(IN_THROWING_VALUE, ret);
    ASSERT_EQ(0, tester);
}
TEST(Exceptions, Exception_WhenThrownFromWithinFunction_ThenGoesStraightToScopeEnd)	
{
    int tester;
    auto ret = Exception_WhenThrownFromWithinFunction_ThenGoesStraightToScopeEnd(&tester);
    ASSERT_EQ(IN_THROWING_VALUE, ret);
    ASSERT_EQ(0, tester);
}
TEST(Exceptions, CATCH_WhenExeptionCaught_Does_NOT_ContinueThrowing)				
{
    auto ret = CATCH_WhenExeptionCaught_Does_NOT_ContinueThrowing();
    ASSERT_EQ(SUCCESS_VALUE, ret);
}
