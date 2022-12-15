#ifndef _COOP_TEST__SCOPE_TEST__
#define _COOP_TEST__SCOPE_TEST__

#include "COOP.h"

DEF_CLASS(ScopeTester);
char* charPtr;
END_DEF(ScopeTester);

FUNCTIONS(ScopeTester, char* pointedVal);
// This function throws if a == b:
MEM_FUN_DECL(ScopeTester, ThrowingIfEQ, int a, int b);
END_FUNCTIONS(ScopeTester);


#endif