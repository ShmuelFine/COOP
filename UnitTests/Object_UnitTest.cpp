#include "pch.h"


#include "ObjectTest.h"

TEST(Object, New_WhenNew_ThenReturnesPointerInCache)
{
	ASSERT_TRUE(New_WhenNew_ThenReturnesPointerInCache());
}

TEST(Object, New_WhenNew_ConstructsBlockWithRightSize)
{
	ASSERT_TRUE(New_WhenNew_ConstructsBlockWithRightSize());
}

TEST(object, Delete_WhenDelete_PointerPointToNull)
{
	ASSERT_TRUE(Delete_WhenDelete_PointerPointToNull());
}

TEST(object, NEW_WhenAllocDifferentTypeThenChar_AllocatesTheRightSize)
{
	ASSERT_TRUE(NEW_WhenAllocDifferentTypeThenChar_AllocatesTheRightSize());
}

TEST(object, Delete_WhenDelete_ThenDeletesTheBlock)
{
	ASSERT_TRUE(Delete_WhenDelete_ThenDeletesTheBlock());
}
