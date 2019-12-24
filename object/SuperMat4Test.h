#pragma once
#include "Mat4Test.h"

DEF_DERIVED_CLASS(SuperMat4Test, Mat4Test)

int step;

END_DEF_DERIVED(SuperMat4Test)

DERIVED_FUNCTIONS(SuperMat4Test, Mat4Test, int hight, int width, int step)
FUNCTION_PTR(SuperMat4Test, SetStep, int step);
END_FUNCTIONS(SuperMat4Test)

FUNCTION_H(SuperMat4Test,SetStep,int step)

DEF_INIT_DERIVED_CLASS(SuperMat4Test, Mat4Test)


