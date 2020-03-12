#ifndef __COOP__VECTOR__H_
#define __COOP__VECTOR__H_

#ifdef __cplusplus
extern "C" {
#endif

#include "coop.h"

	DEF_CLASS(vector);
	// Define member fields here. e.g:
	int size;
	int capacity;
	int sizeOfElement;
	int* data[2];
	int activeBufferIndex;
	//int * newData;
	END_DEF(vector);

	//FUNCTIONS(vector, /*ctor params:*/ int sizeOfElement);
	FUNCTIONS(vector);
	MEM_FUN_DECL(vector, /*func. name:*/ push_back, /*func. params:*/ int val);
	MEM_FUN_DECL(vector, /*func. name:*/ at, /*func. params:*/ int idx, int* ret_val);
	MEM_FUN_DECL(vector, /*func. name:*/ print);
	END_FUNCTIONS(vector);

#ifdef __cplusplus
}
#endif


#endif
