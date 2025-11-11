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
ADD_TEST(GrayImageTest, load_img_from_bmp);
ADD_TEST(GrayImageTest, add_saturate_basic3x3);
ADD_TEST(GrayImageTest, sub_default_zero_3x3);
ADD_TEST(GrayImageTest, sub_abs_matches_absdiff_3x3);
ADD_TEST(GrayImageTest, mul_scalar_round_and_saturate_3x3);
ADD_TEST(GrayImageTest, mul_mat_linear_multiply_2x2);
ADD_TEST(GrayImageTest, gaussian_blur);
ADD_TEST(GrayImageTest, sobel_x_y);
ADD_TEST(GrayImageTest, sobel);
ADD_TEST(GrayImageTest, non_maximum_suppression);
ADD_TEST(GrayImageTest, hysteresis_thresholding);
ADD_TEST(GrayImageTest, canny_test);
END_TEST_SUITE(GrayImageTest);

#endif