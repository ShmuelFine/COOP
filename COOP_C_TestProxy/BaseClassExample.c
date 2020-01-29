#include "BaseClassExample.h"

DEF_CTOR(BaseClassExample, int hight, int width)
{
	_this->hight = hight;
	_this->width = width;
}
END_CTOR

DEF_DTOR(BaseClassExample)
{

}
END_DTOR

FUNCTION_IMPL(BaseClassExample, GetVolume, int* retVal)
{
	*retVal = _this->hight * _this->width;
}
END_FUNCTION_IMPL

INIT_CLASS(BaseClassExample)
BIND(BaseClassExample, GetVolume);
END_INIT_CLASS