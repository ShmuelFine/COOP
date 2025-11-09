#ifndef __COOP_TESTS__GRAY_IMAGE_TEST__H__
#define __COOP_TESTS__GRAY_IMAGE_TEST__H__

#include "UnitTestsInfra.h"
#include "COOP.h"

DEF_TEST_SUITE(GrayImageTest);
ADD_TEST(GrayImageTest, create_SanityTest);
ADD_TEST(GrayImageTest, clone_CreatesIndependentDeepCopy);
ADD_TEST(GrayImageTest, clone_FromROI_IsCorrectAndContiguous);
ADD_TEST(GrayImageTest, equals_CorrectlyComparesPixels);
END_TEST_SUITE(GrayImageTest);

#endif