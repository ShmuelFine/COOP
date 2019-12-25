
#include "SuperMat4Test.h"
 
DEF_DERIVED_CTOR(SuperMat4Test, Mat4Test, int hight, int width, int step) SUPER, hight, width ME
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
END_FUNCTION_IMPL

FUNCTION_IMPL(SuperMat4Test, findLoc, int col, int row ,int *loc)
{
	*loc = (_this->_BASE.width * row + col) * _this->step;
}
END_FUNCTION_IMPL

INIT_DERIVED_CLASS(SuperMat4Test, Mat4Test)
	BIND(SuperMat4Test, SetStep);
	BIND_OVERIDE(SuperMat4Test, Mat4Test, findLoc);
END_INIT_CLASS


