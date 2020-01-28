#ifndef _COOP_TEST__SCOPE_TEST__
#define _COOP_TEST__SCOPE_TEST__

#include "object.h"

DEF_CLASS(ScopeTest)
char* buff;
END_DEF(ScopeTest);

FUNCTIONS(ScopeTest, int size)
BASE_FUNCTION_PTR(ScopeTest, ThrowingIfEQ, int size,int eq);
END_FUNCTIONS(ScopeTest);

FUNCTION_H(ScopeTest, ThrowingIfEQ, int size,int eq);

DEF_INIT_CLASS(ScopeTest);


#endif