#pragma once
#include "Mat4Test.h"

DEF_DRIVED_CLASS(SuperMat4Test, Mat4Test)

int step;

END_DEF_DRIVED(SuperMat4Test)

DRIVED_FUNCTIONS(SuperMat4Test, Mat4Test, FLOAT_TYPE hight, FLOAT_TYPE width, int step)
END_FUNCTIONS(SuperMat4Test)

DEF_INIT_DRIVED_CLASS(SuperMat4Test, Mat4Test)

