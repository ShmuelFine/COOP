#ifndef __COOP_TESTS__SHARED_PTR_TEST__H__
#define __COOP_TESTS__SHARED_PTR_TEST__H__

#include "UnitTestsInfra.h"
#include "COOP.h"

DEF_TEST_SUITE(SharedPtrTest);
ADD_TEST(SharedPtrTest, ctor_sanity);
ADD_TEST(SharedPtrTest, copyFrom_sanity);
ADD_TEST(SharedPtrTest, release_sanity);
ADD_TEST(SharedPtrTest, isEmpty_sanity);
END_TEST_SUITE(SharedPtrTest);

#endif