#ifndef __COOP__VECTOR__H_
#define __COOP__VECTOR__H_


#include "coop.h"
#include "vectorIterator.h"

DEF_CLASS(vector);
int size;
int capacity;
int sizeOfElement;
int* data[2];
int activeBufferIndex;
END_DEF(vector);

FUNCTIONS(vector);
MEM_FUN_DECL(vector, push_back, int val);
MEM_FUN_DECL(vector, at, int idx, int* ret_val);
MEM_FUN_DECL(vector, begin, vectorIterator * retVecIt);
MEM_FUN_DECL(vector, end, vectorIterator * retVecIt);
MEM_FUN_DECL(vector, print);
END_FUNCTIONS(vector);


#endif
