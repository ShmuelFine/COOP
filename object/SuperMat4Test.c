
#include "SuperMat4Test.h"
 
DEF_DERIVED_CTOR(SuperMat4Test, Mat4Test, FLOAT_TYPE hight, FLOAT_TYPE width, int step) SUPER, hight, width ME
{
	_this->step = step;
}
END_DERIVED_CTOR

DEF_DERIVED_DTOR(SuperMat4Test, Mat4Test)
{

}
END_DERIVED_DTOR

FUNCTION_IMPL(SuperMat4Test, SetStep, int step)
{
	_this->step = step;
}

INIT_DERIVED_CLASS(SuperMat4Test, Mat4Test)
BIND(SuperMat4Test, SetStep);
END_INIT_CLASS
