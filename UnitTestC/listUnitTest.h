#ifndef __COOP_TESTS__LIST_TEST__H__
#define __COOP_TESTS__LIST_TEST__H__

#include "UnitTestsInfra.h"
#include "COOP.h"

DEF_TEST_SUITE(ListTest);
ADD_TEST(ListTest, push_back_SanityTest);
ADD_TEST(ListTest, pop_back_SanityTest);
ADD_TEST(ListTest, push_front_SanityTest);
ADD_TEST(ListTest, pop_front_SanityTest);
ADD_TEST(ListTest, front_back_SanityTest);
ADD_TEST(ListTest, size_empty_clear_SanityTest);
ADD_TEST(ListTest, print_SanityTest);
ADD_TEST(ListTest, front_onEmpty_Throws);
ADD_TEST(ListTest, pop_front_onEmpty_Throws);
ADD_TEST(ListTest, pop_back_onEmpty_Throws);
ADD_TEST(ListTest, dtor_freesAllMemory);
END_TEST_SUITE(ListTest);

#endif
