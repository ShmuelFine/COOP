#include "SuperMat.h"

DEF_DERIVED_CTOR(SuperMat, Mat, int hight, int width, int step) SUPER, hight, width ME
{
	_this->step = step;
}
END_DERIVED_CTOR

DEF_DERIVED_DTOR(SuperMat, Mat)
{

}
END_DERIVED_DTOR

FUNCTION_IMPL(SuperMat, FindLoc, int row, int col, int* returned)
{
	_BASE(Mat, FindLoc, row, col, returned);
	*returned *= _this->step;
}
END_FUNCTION_IMPL

INIT_DERIVED_CLASS(SuperMat, Mat);
BIND_OVERIDE(SuperMat, Mat, FindLoc);
END_INIT_CLASS