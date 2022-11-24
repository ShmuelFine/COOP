#include "test_defs.h"
#include "../COOP/COOP.h"

DEF_TEST_SUITE(SharedPtrTest);
ADD_TEST(SharedPtrTest, check_type_char);
ADD_TEST(SharedPtrTest, pointing_to_class_test);
ADD_TEST(SharedPtrTest, CopyTo__PointsOnTheSameValue);
ADD_TEST(SharedPtrTest, check_type);
ADD_TEST(SharedPtrTest, CREATE_SHARED__macro_test);
END_TEST_SUITE(SharedPtrTest);