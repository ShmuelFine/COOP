#include "ScopeTester.h"
#include "dynamic_memory_management.h"

DEF_CTOR(ScopeTester, char * pointedVal)
{
	_this->buff = pointedVal;
	*(_this->buff) = 'A';

}
END_CTOR

DEF_DTOR(ScopeTester)
{
	*(_this->buff) = 0;
}
END_DTOR

FUNCTION_IMPL(ScopeTester, ThrowingIfEQ, int size, int eq)
{
	SCOPE_START;

	if (size == eq)
		THROW;
	size += eq;

	SCOPE_END;
}
END_FUNCTION_IMPL

INIT_CLASS(ScopeTester)
BASE_BIND(ScopeTester, ThrowingIfEQ);
END_INIT_CLASS