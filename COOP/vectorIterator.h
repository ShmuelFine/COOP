#ifndef __COOP__VECTORITERATOR__H_
#define __COOP__VECTORITERATOR__H_

#ifdef __cplusplus
extern "C" {
#endif

#include "coop.h"

	struct vector_t;
	typedef struct vector_t vector;

	DEF_CLASS(vectorIterator);
	// Define member fields here. e.g:
	int currIdx;
	struct vector_t * vec;
	END_DEF(vectorIterator);

	//FUNCTIONS(vector, /*ctor params:*/ int sizeOfElement);
	FUNCTIONS(vectorIterator, vector * vec, int startIdx);
	MEM_FUN_DECL(vectorIterator, /*func. name:*/ increment);
	MEM_FUN_DECL(vectorIterator, /*func. name:*/ equals, vectorIterator other, bool * retVal);
	MEM_FUN_DECL(vectorIterator, /*func. name:*/ getContentsOf, int * retVal);
	END_FUNCTIONS(vectorIterator);

#ifdef __cplusplus
}
#endif


#endif

