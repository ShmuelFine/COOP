#ifndef __COOP_TESTS__MYCLASS_H__
#define __COOP_TESTS__MYCLASS_H__

#include "COOP.h"
#include "DynamicMemoryManagement.h"
#include "ObjectLifecycleManagement.h"
#include <stdio.h>

DEF_CLASS(myClass);
int hight;
int width;
END_DEF(myClass);

FUNCTIONS(myClass, int hight, int width);
MEM_FUN_DECL(myClass, GetSomthing, int* retVal);
END_FUNCTIONS(myClass);

#endif