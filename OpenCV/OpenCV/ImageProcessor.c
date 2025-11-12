#include "ImageProcessor.h"

DEF_CTOR(ImageProcessor) {} END_CTOR
DEF_DTOR(ImageProcessor) {} END_DTOR

PURE_VIRTUAL(ImageProcessor, process, GrayImage* img);

INIT_CLASS(ImageProcessor);
BIND(ImageProcessor, process);
END_INIT_CLASS(ImageProcessor)