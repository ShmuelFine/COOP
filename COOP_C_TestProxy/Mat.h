#ifndef __MAT_H__
#define __MAT_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "object.h"

	DEF_CLASS(Mat)
	int hight;
	int width;
	END_DEF(Mat);

	FUNCTIONS(Mat, int hight, int width)

	BASE_FUNCTION_PTR(Mat, FindLoc, int row, int col, int* returned);
	BASE_FUNCTION_PTR(Mat, GetWidth, int* returned);

	END_FUNCTIONS(Mat);

	FUNCTION_H(Mat, FindLoc, int row, int col, int* returned);
	FUNCTION_H(Mat, GetWidth, int* returned);

	DEF_INIT_CLASS(Mat);

#ifdef __cplusplus
}
#endif

#endif