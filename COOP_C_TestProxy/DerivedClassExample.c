#include "DerivedClassExample.h"

DEF_DERIVED_CTOR(DerivedClassExample, BaseClassExample, int hight, int width, int depth) SUPER, hight, depth ME
{
	_this->depth = depth;
}
END_DERIVED_CTOR

DEF_DERIVED_DTOR(DerivedClassExample, BaseClassExample)
{

}
END_DERIVED_DTOR

OVERRIDE_FUNCTION_IMPL(DerivedClassExample, GetVolume, int* retVal)
{
	SCOPE_START;
	int retVal = 0;
	FUN(GetVolume, _this), &retVal);
	//_BASE(BaseClassExample, GetVolume, retVal);
	retVal *= _this->depth;

	SCOPE_END ;
}
END_FUNCTION_IMPL

INIT_DERIVED_CLASS(DerivedClassExample, BaseClassExample);
BIND_OVERIDE(DerivedClassExample, BaseClassExample, GetVolume);
END_INIT_CLASS