#ifndef __COOP__FUNCTIONS__H_
#define __COOP__FUNCTIONS__H_

#include "GrayImage.h"
#include <stdint.h>   

FUN_DECL(__gaussian_blur, GrayImage* img);

FUN_DECL(__sobel_filter, GrayImage* img, GrayImage* out_dir);

FUN_DECL(__sobel_convolve_x, GrayImage* img, GrayImage* out_x);

FUN_DECL(__sobel_convolve_y, GrayImage* img, GrayImage* out_y);

FUN_DECL(__sobel_magnitude, GrayImage* img_x, GrayImage* img_y, GrayImage* out_mag);

FUN_DECL(__sobel_filter_split, GrayImage* img);

FUN_DECL(__non_maximum_suppression, GrayImage* img, GrayImage const* img_dir);

FUN_DECL(__hysteresis_thresholding, GrayImage* img, uint8_t low_thresh, uint8_t high_thresh);

FUN_DECL(__zero_border, GrayImage* img);

#endif // __COOP__FUNCTIONS__H_