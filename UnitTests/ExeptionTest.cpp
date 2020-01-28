#include "pch.h"

#include "../COOP_C_TestProxy/ExeptionTest.h"
#include <iostream>
#include <fstream>



//TEST(EXEPTINS, WhenexeptionIsThrown_ThenDoesntSkipTheScopeEnds)
//{
//
//}
//
TEST(EXEPTIONS, WhenexeptionIsThrown_ThenGoesStriehtToScopeEnd)
{
	ASSERT_TRUE(WhenexeptionIsThrown_ThenGoesStriehtToScopeEnd());
}

TEST(EXEPTIONS, WhenexeptionIsThrownWithInAFunctionCall_ThenGoesStriehtToScopeEnd)
{
	ASSERT_TRUE(WhenexeptionIsThrownWithInAFunctionCall_ThenGoesStriehtToScopeEnd());
}

TEST(EXEPTIONS, WhenCatchingExeption_DoesntContinueThrowing)
{
	ASSERT_TRUE(WhenCatchingExeption_DoesntContinueThrowing);
}