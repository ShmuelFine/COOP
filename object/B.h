#pragma once
#include "object.h"
#include "Inherit.h"
#include "A.h" 
int aaa(int a, int b);

INHERIT(B,A)
	int Calculated_value;
END_INHERIT(B)

FUNCTIONS(B)
//FUNCTION_PTR(B, Print, int i, int j);
END_FUNCTIONS(B)

DEF_INIT_CLASS(B);





