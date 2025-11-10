#ifndef __COOP_TESTS__GRAY_IMAGE_TEST__H__
#define __COOP_TESTS__GRAY_IMAGE_TEST__H__

#include "UnitTestsInfra.h"
#include "COOP.h"

DEF_TEST_SUITE(GrayImageTest);
ADD_TEST(GrayImageTest, create_SanityTest);
ADD_TEST(GrayImageTest, clone);
ADD_TEST(GrayImageTest, clone_FromROI);
ADD_TEST(GrayImageTest, equals);
ADD_TEST(GrayImageTest, init_copy);
ADD_TEST(GrayImageTest, init_move); 
ADD_TEST(GrayImageTest, save_img_to_bmp);
END_TEST_SUITE(GrayImageTest);

#endif