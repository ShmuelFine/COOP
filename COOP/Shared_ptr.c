#include "Shared_ptr.h"


DEF_CTOR(Shared_ptr, int* ptr_x)
{
	_this->px = ptr_x;
	NEW(_this->pn , int);
	*_this->pn = 1;
}
END_CTOR

DEF_DTOR(Shared_ptr)
{
	//Nothing to do here...
}
END_DTOR

MEM_FUN_IMPL(Shared_ptr, initialize_ref_count, int* ptr_x, int* PRefCount)
{
	if (PRefCount == NULL)
	{

	}
}
END_FUN;