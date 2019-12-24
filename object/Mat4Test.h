#pragma once
#include "../object/object.h"

DEF_CLASS(Mat4Test)

int hight;
int width;

END_DEF(Mat4Test)

FUNCTIONS(Mat4Test, int, int)
FUNCTION_PTR(Mat4Test, findLoc, int row, int col,int* loc);
END_FUNCTIONS(Mat4Test)

FUNCTION_H(Mat4Test, findLoc, int row, int col,int * loc);

DEF_INIT_CLASS(Mat4Test)
