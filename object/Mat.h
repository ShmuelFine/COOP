#ifndef __OBJECT__H_
#include "object.h"
#endif
DEF_CLASS(Mat)
int hight;
int width;
END_DEF(Mat);

FUNCTIONS(Mat, int hight, int width)

BASE_FUNCTION_PTR(Mat, FindLoc, int row, int col, int* returned);
BASE_FUNCTION_PTR(Mat, GetWidth, int* returned);

END_FUNCTIONS(Mat);

BASE_FUNCTION_H(Mat, FindLoc, int row, int col, int* returned);
BASE_FUNCTION_H(Mat, GetWidth, int* returned);

DEF_INIT_CLASS(Mat);
