#ifndef __BORDER_PROCESSOR__H_
#define __BORDER_PROCESSOR__H_

#include "ImageProcessor.h"

DEF_DERIVED_CLASS(BorderProcessor, ImageProcessor);
END_DEF_DERIVED(BorderProcessor);

DERIVED_FUNCTIONS(BorderProcessor, ImageProcessor);
FUN_OVERRIDE(ImageProcessor, process, GrayImage* img);
END_DERIVED_FUNCTIONS(BorderProcessor);

#endif