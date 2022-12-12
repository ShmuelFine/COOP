#include "Shared_ptr.h"


DEF_CTOR(Shared_ptr)
{
	_this->px = NULL;
	_this->pn = NULL;
}
END_CTOR

DEF_DTOR(Shared_ptr)
{
	if (_this->pn != NULL)
	{
		*(_this->pn) -= 1;
		if (*(_this->pn) == 0)
		{
			FREE(_this->pn);
			FREE(_this->px);
		}
	}
}
END_DTOR

MEM_FUN_IMPL(Shared_ptr, reset, void* newPtr)
{
	_this->px = newPtr;
	ALLOC_VARIABLE(_this->pn, int);
	*(_this->pn) = 1;

}
END_FUN


MEM_FUN_IMPL(Shared_ptr, copyTo, Shared_ptr* other)
{
	MFUN(other, release) CALL;
	other->px = _this->px;
	other->pn = _this->pn;
	(*(_this->pn))++;
}
END_FUN

MEM_FUN_IMPL(Shared_ptr, copyFrom, Shared_ptr const* other)
{
	MFUN(_this, release) CALL;
	_this->px = other->px;
	_this->pn = other->pn;
	(*(_this->pn))++;
}
END_FUN

MEM_FUN_IMPL(Shared_ptr, release)
{
	if (_this->pn != NULL)
	{
		*(_this->pn) -= 1;
		if (*(_this->pn) == 0)
		{
			FREE(_this->pn);
			FREE(_this->px);
		}
		_this->pn = NULL;
		_this->px = NULL;
	}
}
END_FUN

MEM_FUN_IMPL(Shared_ptr, isEmpty, bool* out)
{
	*out = (_this->px == NULL);
}
END_FUN

INIT_CLASS(Shared_ptr)
BIND(Shared_ptr, reset);
BIND(Shared_ptr, copyTo);
BIND(Shared_ptr, copyFrom);
BIND(Shared_ptr, release);
BIND(Shared_ptr, isEmpty);
END_INIT_CLASS(Shared_ptr)