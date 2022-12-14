#ifndef __COOP_TESTS__VECTOR_TEST__H__
#define __COOP_TESTS__VECTOR_TEST__H__

#include "UnitTestsInfra.h"
#include "COOP.h"

DEF_TEST_SUITE(VectorTest);
ADD_TEST(VectorTest, push_back_SanityTest);
ADD_TEST(VectorTest, pop_back_SanityTest);
ADD_TEST(VectorTest, at_ThrowsWhenIdxIsOutOfRange);
ADD_TEST(VectorTest, set_SanityTest);
ADD_TEST(VectorTest, get_SanityTest);
ADD_TEST(VectorTest, dtor_freesAllMemory);
END_TEST_SUITE(VectorTest);

#endif