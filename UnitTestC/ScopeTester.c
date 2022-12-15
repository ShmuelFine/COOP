#include "ScopeTester.h"
#include "DynamicMemoryManagement.h"

DEF_CTOR(ScopeTester, char * pointedVal)
{
	_this->charPtr = pointedVal;
	*(_this->charPtr) = 'A';

}
END_CTOR

DEF_DTOR(ScopeTester)
{
	*(_this->charPtr) = 0;
}
END_DTOR

MEM_FUN_IMPL(ScopeTester, ThrowingIfEQ, int a, int b)
{
	if (a == b)
		THROW;
}
END_FUN

INIT_CLASS(ScopeTester)
BIND(ScopeTester, ThrowingIfEQ);
END_INIT_CLASS(ScopeTester)