#ifndef _COOP_TEST__SCOPE_TEST__
#define _COOP_TEST__SCOPE_TEST__

#include "COOP.h"

DEF_CLASS(ScopeTester)
char* buff;
END_DEF(ScopeTester);

FUNCTIONS(ScopeTester, char * pointedVal)
// This function throws if a == b:
FUNCTION_PTR(ScopeTester, ThrowingIfEQ, int a, int b);
END_FUNCTIONS(ScopeTester);

DEF_INIT_CLASS(ScopeTester);


#endif