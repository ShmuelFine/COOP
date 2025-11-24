#ifndef __THRESHOLD_PROCESSOR__H_
#define __THRESHOLD_PROCESSOR__H_

#include "ImageProcessor.h"

// Hysteresis edge classification values
static const uint8_t HYST_EDGE_STRONG = 255;
static const uint8_t HYST_EDGE_WEAK = 128;
static const uint8_t HYST_EDGE_NONE = 0;

DEF_DERIVED_CLASS(ThresholdProcessor, ImageProcessor);
uint8_t low_thresh;
uint8_t high_thresh;
END_DEF_DERIVED(ThresholdProcessor);

DERIVED_FUNCTIONS(ThresholdProcessor, ImageProcessor, uint8_t low_thresh, uint8_t high_thresh);
FUN_OVERRIDE(ImageProcessor, process, GrayImage* img);
END_DERIVED_FUNCTIONS(ThresholdProcessor);

#endif