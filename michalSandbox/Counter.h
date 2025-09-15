#ifndef COUNTER_H
#define COUNTER_H

#include "COOP.h"
#include "ObjectLifecycleManagement.h"

DEF_CLASS(Counter);
//declare variables here
 int value;
END_DEF(Counter);

FUNCTIONS(Counter, int intialValue);

	MEM_FUN_DECL(Counter, Inc);
	MEM_FUN_DECL(Counter, Add, int x);
	MEM_FUN_DECL(Counter, Get, int* outValue);

END_FUNCTIONS(Counter);



#endif
