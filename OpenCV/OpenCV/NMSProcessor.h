#ifndef __NMS_PROCESSOR__H_
#define __NMS_PROCESSOR__H_

#include "ImageProcessor.h"

DEF_DERIVED_CLASS(NMSProcessor, ImageProcessor);
GrayImage const* direction_image;
END_DEF_DERIVED(NMSProcessor);

DERIVED_FUNCTIONS(NMSProcessor, ImageProcessor, GrayImage const* direction_image);
FUN_OVERRIDE(ImageProcessor, process, GrayImage* img);
END_DERIVED_FUNCTIONS(NMSProcessor);

#endif