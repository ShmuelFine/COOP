#include "Mat4Test.h"

DEF_CTOR(Mat4Test, int hight, int width)
{
	_this->hight = hight;
	_this->width = width;
}
END_CTOR

DEF_DTOR(Mat4Test)
{

}
END_DTOR

FUNCTION_IMPL(Mat4Test, findLoc, int row, int col, int* loc)
{
	*loc = row * _this->hight + col;
}
END_FUNCTION_IMPL

FUNCTION_IMPL(Mat4Test, getWidth, int* width)
{
	*width = _this->width;
}
END_FUNCTION_IMPL

INIT_CLASS(Mat4Test)
{
	BASE_BIND(Mat4Test, findLoc);
	BIND(Mat4Test, getWidth);
}
END_INIT_CLASS