#include "pch.h"
#include "vectorUnitTest.h"

TEST(vector, vector_push_back_sanity)
{
	ASSERT_FALSE(vector_push_back_SanityTest());

}

TEST(vector, vector_at_throws_when_idx_is_outOfRange_sanity)
{
    int tester;
    auto ret = vector_at_throws_when_idx_is_outOfRange(&tester);
    ASSERT_EQ(IN_THROWING_VALUE, ret);
    ASSERT_EQ(0, tester);
}

TEST(vector, vector_iterator_sanity)
{
    ASSERT_FALSE(vector_iterator_SanityTest());

}

TEST(vector, vector_begin_sanity)
{
    ASSERT_FALSE(vector_begin_SanityTest());

}

TEST(vector, vector_end_sanity)
{
    ASSERT_FALSE(vector_end_SanityTest());

}