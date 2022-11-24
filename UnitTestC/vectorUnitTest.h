#ifndef __COOP_TESTS__VECTOR_TEST__H__
#define __COOP_TESTS__VECTOR_TEST__H__

#include "test_defs.h"
#include "../COOP/COOP.h"

DEF_TEST_SUITE(VectorTest);
ADD_TEST(VectorTest, push_back_SanityTest);
ADD_TEST(VectorTest, at_throws_when_idx_is_outOfRange);
ADD_TEST(VectorTest, iteration_SanityTest);
END_TEST_SUITE(VectorTest);

#endif