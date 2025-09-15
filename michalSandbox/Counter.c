#include "Counter.h"
#include <stdio.h>


DEF_CTOR(Counter, int intialValue)
{
	_this->value = intialValue;
	RETURN;
}
END_CTOR

DEF_DTOR(Counter)
{
	//Nothing to do here...
	RETURN;
}
END_DTOR

MEM_FUN_IMPL(Counter, Inc)
{
	_this->value += 1;
	RETURN;
}
END_FUN

MEM_FUN_IMPL(Counter, Add, int x)
{
	_this->value += x;
	RETURN;
}
END_FUN

MEM_FUN_IMPL(Counter, Get, int* outValue)
{
	ASSERT_NOT_NULL(outValue);
	*outValue = _this->value;
	RETURN;
}
END_FUN



INIT_CLASS(Counter)
BIND(Counter, Inc);
BIND(Counter, Add);
BIND(Counter, Get);
END_INIT_CLASS(Counter)




