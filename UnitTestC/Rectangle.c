#include "Rectangle.h"

DEF_CTOR(Rectangle, int hight, int width)
{
	_this->hight = hight;
	_this->width = width;
}
END_CTOR

DEF_DTOR(Rectangle)
{
	//Nothing to do here...
}
END_DTOR

MEM_FUN_IMPL(Rectangle, GetArea, int* ret_val)
{
	*ret_val = _this->hight * _this->width;
}
END_FUN;

MEM_FUN_IMPL(Rectangle, GetSideLength, int side_idx, int* ret_val)
{
	*ret_val = side_idx == 0 ? _this->hight : _this->width;
}
END_FUN;

INIT_CLASS(Rectangle)
BIND(Rectangle, GetArea);
BIND(Rectangle, GetSideLength);
END_INIT_CLASS(Rectangle)

