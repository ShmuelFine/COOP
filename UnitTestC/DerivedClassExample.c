#include "DerivedClassExample.h"



DEF_DERIVED_CTOR(DerivedClassExample, BaseClassExample, int hight, int width, int depth) SUPER, hight, width ME
{
	_this->depth = depth;
}
END_DERIVED_CTOR

DEF_DERIVED_DTOR(DerivedClassExample, BaseClassExample)
{

}
END_DERIVED_DTOR

FUN_OVERRIDE_IMPL(DerivedClassExample, BaseClassExample, GetVolume, int* retVal)
{
	*retVal = 0;
	
	FUN_BASE(_this, GetVolume), retVal CALL;

	*retVal= (*retVal) *_this->depth;
	
}
END_FUN

MEM_FUN_IMPL(DerivedClassExample, JustReturn10, int* retVal)
{
	*retVal = 10;
}
END_FUN

INIT_DERIVED_CLASS(DerivedClassExample, BaseClassExample);
BIND_OVERIDE(DerivedClassExample, BaseClassExample, GetVolume);
BIND(DerivedClassExample, JustReturn10);
END_INIT_CLASS(DerivedClassExample)
