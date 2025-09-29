#include "SharedObjPtr.h"

DEF_DERIVED_CTOR(objSPtr, SharedPodPtr, object* obj) SUPER, sizeof(object) ME
{
	_this->objPtr = (object*)_this->_base.px;
}
END_DERIVED_CTOR

DEF_DERIVED_DTOR(objSPtr, SharedPodPtr)
{
	DESTROY(_this->objPtr);
}
END_DERIVED_DTOR


MEM_FUN_IMPL(objSPtr, reset, object* newPtr)
{
	MFUN(_this, release) CALL;
	FUN_BASE(_this, reset), (void*)newPtr CALL;
	_this->objPtr = (object*)_this->_base.px;
}
END_FUN


MEM_FUN_IMPL(objSPtr, copyFrom, objSPtr const* other)
{
	MFUN(_this, release) CALL;
	FUN_BASE(_this, copyFrom), &(other->_base) CALL;
	_this->objPtr = (object*)_this->_base.px;
}
END_FUN

MEM_FUN_IMPL(objSPtr, print)
{
	printf("objSPtr at %p: ", _this);
}
END_FUN

MEM_FUN_IMPL(objSPtr, equals, objSPtr const* other, bool* out_equal)
{
	*out_equal = false;
	IF(other == NULL)
	{
		RETURN; 
	}
	END_IF;

	*out_equal = (_this->objPtr == other->objPtr);
}
END_FUN

FUN_OVERRIDE_IMPL(objSPtr, SharedPodPtr, release)
{
	IF(_this->objPtr)
	{
		DESTROY(_this->objPtr);
	}
	END_IF;
	FUN_BASE(_this, release) CALL;
	_this->objPtr = (object*)_this->_base.px;
}
END_FUN


INIT_DERIVED_CLASS(objSPtr, SharedPodPtr);
BIND(objSPtr, reset);
BIND(objSPtr, copyFrom);
BIND(objSPtr, print);
BIND(objSPtr, equals);
BIND_OVERIDE(objSPtr, SharedPodPtr, release);
END_INIT_CLASS(objSPtr)