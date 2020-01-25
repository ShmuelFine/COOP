#ifndef __SUPER__MAT__
#define __SUPER__MAT__

#ifdef __cplusplus
extern "C" {
#endif

#include "Mat.h"

	DEF_DERIVED_CLASS(SuperMat, Mat)

	int step;

	END_DEF_DERIVED(SuperMat);

	DERIVED_FUNCTIONS(SuperMat, Mat, int hight, int width, int step)

	OVERIDE_FUNCTION_PTR(FindLoc, int row, int col, int* returned);

	END_FUNCTIONS(SuperMat);

	FUNCTION_H(SuperMat, FindLoc, int row, int col, int* returned);

	DEF_INIT_DERIVED_CLASS(SuperMat, Mat);

#ifdef __cplusplus
}
#endif

#endif