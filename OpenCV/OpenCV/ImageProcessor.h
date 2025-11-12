#ifndef __COOP__IMAGE_PROCESSOR__H_
#define __COOP__IMAGE_PROCESSOR__H_

#include "COOP.h"
#include "GrayImage.h"

DEF_CLASS(ImageProcessor);
END_DEF(ImageProcessor);

FUNCTIONS(ImageProcessor);
MEM_FUN_DECL(ImageProcessor, process, GrayImage* img);
END_FUNCTIONS(ImageProcessor);

#endif