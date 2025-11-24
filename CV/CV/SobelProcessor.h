#ifndef __SOBEL_PROCESSOR__H_
#define __SOBEL_PROCESSOR__H_

#include "ImageProcessor.h"

// Static kernels for Sobel operation
static const int8_t SOBEL_GX_KERNEL[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
static const int8_t SOBEL_GY_KERNEL[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };

//Constants for calculating a fast approximation of the 
//gradient direction (based on tan(22.5) ≈ 41/100)
static const long TAN_APPROX_NUMERATOR = 41L; // The numerator of the approximation (41)
static const long TAN_APPROX_DENOMINATOR = 100L; // The denominator of the approximation (100)

DEF_DERIVED_CLASS(SobelProcessor, ImageProcessor);
GrayImage* out_direction_image;
END_DEF_DERIVED(SobelProcessor);

DERIVED_FUNCTIONS(SobelProcessor, ImageProcessor, GrayImage* out_direction_image);
FUN_OVERRIDE(ImageProcessor, process, GrayImage* img);
END_DERIVED_FUNCTIONS(SobelProcessor);

#endif