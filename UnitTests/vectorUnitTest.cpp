#include "pch.h"
#include "vectorUnitTest.h"

TEST(vector, vector_push_back_SanityTest)
{
	ASSERT_FALSE(vector_push_back_SanityTest());
	//TODO: how to assert that it threw?
	ASSERT_ANY_THROW(vector_at_throws_when_idx_is_outOfRange());
}