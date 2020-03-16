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
			DELETE_OBJ(_this->pn);
			DELETE_OBJ(_this->px);
		}
	}
}
END_DTOR

MEM_FUN_IMPL(Shared_ptr, Reset, int* newPtr)
{
	_this->px = newPtr;
	NEW(_this->pn , int);
	*(_this->pn) = 1;
}
END_FUN


MEM_FUN_IMPL(Shared_ptr, CopyTo, Shared_ptr* other)
{
	FUN(other, Release) CALL;

	other->px = _this->px;
	other->pn = _this->pn;
	*(_this->pn)++;

}
END_FUN

MEM_FUN_IMPL(Shared_ptr, CopyFrom, Shared_ptr const* other)
{
	_this->px = other->px;
	_this->pn = other->pn;
	*(_this->pn)++;
}
END_FUN

MEM_FUN_IMPL(Shared_ptr, Release)
{ if (_this->pn!= NULL)
	{ 
	*(_this->pn) -= 1;
	if (*(_this->pn) == 0)
	{ 
		DELETE_OBJ(_this->pn);
		DELETE_OBJ(_this->px);
	}
	_this->pn = NULL;
	_this->px = NULL;
	}
}
END_FUN

MEM_FUN_IMPL(Shared_ptr, IsEmpty, bool* out)
{
	if (_this->px == NULL)
		*out = true;
		
}
END_FUN

INIT_CLASS(Shared_ptr)
BIND(Shared_ptr, Reset);
BIND(Shared_ptr, CopyTo);
BIND(Shared_ptr, CopyFrom);
BIND(Shared_ptr, Release);
BIND(Shared_ptr, IsEmpty);
END_INIT_CLASS