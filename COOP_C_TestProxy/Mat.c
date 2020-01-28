          

#include "Mat.h"

DEF_CTOR(Mat, int hight, int width)
{
	_this->hight = hight;
	_this->width = width;
}
END_CTOR

DEF_DTOR(Mat)
{

}
END_DTOR

FUNCTION_IMPL(Mat, FindLoc, int row, int col, int* returned)
{
	*returned = _this->width * row + col;
}
END_FUNCTION_IMPL

FUNCTION_IMPL(Mat, GetWidth, int* returned)
{
	*returned = _this->width;
}
END_FUNCTION_IMPL

INIT_CLASS(Mat)
BASE_BIND(Mat, FindLoc);
BASE_BIND(Mat, GetWidth);
END_INIT_CLASS