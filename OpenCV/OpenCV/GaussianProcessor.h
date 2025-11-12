#ifndef __GAUSSIAN_PROCESSOR__H_
#define __GAUSSIAN_PROCESSOR__H_

#include "ImageProcessor.h"

// Static kernels for Gaussian Blur operation
static const uint8_t GAUSSIAN_KERNEL_VALS[3][3] = { {1, 2, 1}, {2, 4, 2}, {1, 2, 1} };
static const int GAUSSIAN_DIVISOR = 16;

DEF_DERIVED_CLASS(GaussianProcessor, ImageProcessor);
END_DEF_DERIVED(GaussianProcessor);

DERIVED_FUNCTIONS(GaussianProcessor, ImageProcessor);
FUN_OVERRIDE(ImageProcessor, process, GrayImage* img);
END_DERIVED_FUNCTIONS(GaussianProcessor);

#endif