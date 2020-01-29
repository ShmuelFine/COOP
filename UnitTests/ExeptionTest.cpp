#include "pch.h"

#include "ExeptionUnitTest.h"
#include <iostream>
#include <fstream>

TEST(Exceptions, Exception_WhenThrown_ThenGoesStraightToScopeEnd)					{ ASSERT_TRUE(Exception_WhenThrown_ThenGoesStraightToScopeEnd)					; }
TEST(Exceptions, Exception_WhenThrownFromWithinFunction_ThenGoesStraightToScopeEnd)	{ ASSERT_TRUE(Exception_WhenThrownFromWithinFunction_ThenGoesStraightToScopeEnd); }	
TEST(Exceptions, CATCH_WhenExeptionCaught_Does_NOT_ContinueThrowing)				{ ASSERT_TRUE(CATCH_WhenExeptionCaught_Does_NOT_ContinueThrowing)				; }

//
//TEST(EXEPTIONS, WhenexeptionIsThrown_ThenGoesStriehtToScopeEnd)
//{
//	ASSERT_TRUE(WhenexeptionIsThrown_ThenGoesStriehtToScopeEnd());
//}
//
//TEST(EXEPTIONS, WhenexeptionIsThrownWithInAFunctionCall_ThenGoesStriehtToScopeEnd)
//{
//	ASSERT_TRUE(WhenexeptionIsThrownWithInAFunctionCall_ThenGoesStriehtToScopeEnd());
//}
//
//TEST(EXEPTIONS, WhenCatchingExeption_DoesntContinueThrowing)
//{
//	ASSERT_TRUE(WhenCatchingExeption_DoesntContinueThrowing);
//}