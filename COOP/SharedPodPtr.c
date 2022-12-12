#include "SharedPodPtr.h"


DEF_CTOR(SharedPodPtr, MEM_SIZE_T size_bytes)
{
	_this->px = NULL;
	_this->pn = NULL;
	if (size_bytes)
	{
		char* data = NULL;
		ALLOC_ARRAY(data, char, size_bytes);
		MFUN(_this, reset), data CALL;
	}
}
END_CTOR

DEF_DTOR(SharedPodPtr)
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

MEM_FUN_IMPL(SharedPodPtr, reset, void* newPtr)
{
	MFUN(_this, release) CALL;

	_this->px = newPtr;
	ALLOC(_this->pn, int);
	*(_this->pn) = 1;

}
END_FUN


MEM_FUN_IMPL(SharedPodPtr, copyFrom, SharedPodPtr const* other)
{
	MFUN(_this, release) CALL;
	_this->px = other->px;
	_this->pn = other->pn;
	(*(_this->pn))++;
}
END_FUN

MEM_FUN_IMPL(SharedPodPtr, release)
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

MEM_FUN_IMPL(SharedPodPtr, isEmpty, bool* out)
{
	*out = (_this->px == NULL);
}
END_FUN

INIT_CLASS(SharedPodPtr)
BIND(SharedPodPtr, reset);
BIND(SharedPodPtr, copyFrom);
BIND(SharedPodPtr, release);
BIND(SharedPodPtr, isEmpty);
END_INIT_CLASS(SharedPodPtr)