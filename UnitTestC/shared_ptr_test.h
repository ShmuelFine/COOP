#include "UnitTestsInfra.h"
#include "../COOP/COOP.h"

DEF_TEST_SUITE(SharedPtrTest);
ADD_TEST(SharedPtrTest, ctor_sanity);
ADD_TEST(SharedPtrTest, copyFrom_sanity);
ADD_TEST(SharedPtrTest, release_sanity);
ADD_TEST(SharedPtrTest, isEmpty_sanity);
END_TEST_SUITE(SharedPtrTest);