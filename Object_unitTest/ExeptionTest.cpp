#include "pch.h"


#ifdef _DEBUG
#pragma comment(lib, "object_d.lib")
#else
#pragma comment(lib, "object.lib")
#endif

#include "../object/ExeptionTest.h"
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