#ifndef _COOP_TEST__SCOPE_TEST__
#define _COOP_TEST__SCOPE_TEST__

#include "object.h"

DEF_CLASS(ScopeTester)
char* buff;
END_DEF(ScopeTester);

FUNCTIONS(ScopeTester, char * pointedVal)
BASE_FUNCTION_PTR(ScopeTester, ThrowingIfEQ, int size,int eq);
END_FUNCTIONS(ScopeTester);

FUNCTION_H(ScopeTester, ThrowingIfEQ, int size,int eq);

DEF_INIT_CLASS(ScopeTester);


#endif