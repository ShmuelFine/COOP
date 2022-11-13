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

MEM_FUN_IMPL(BaseClassExample, GetVolume, int* retVal)
{
	*retVal = _this->hight * _this->width;
}
END_FUN

INIT_CLASS(BaseClassExample)
BIND(BaseClassExample, GetVolume);
END_INIT_CLASS