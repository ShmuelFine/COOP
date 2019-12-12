#pragma once
#include "object.h"

DEF_CLASS(A)
   int value;
END_DEF(A)

DEF_CTOR(A)
{
	_this->value = 0;
}

FUNCTIONS(A)
//FUNCTION_PTR(A, Print, int i, int j);
END_FUNCTIONS(A)

DEF_INIT_CLASS(A);


