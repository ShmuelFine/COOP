#pragma once

#if _GMED_EXPORTING_
#define GMED_API __declspec(dllexport)
#else
#define GMED_API __declspec(dllimport)
#endif

//#define GMED_API __declspec(dllexport)

#define FLOAT_TYPE double
