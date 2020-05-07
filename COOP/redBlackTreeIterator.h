#ifndef __COOP__RED_BLACK_TREE_ITERATOR__H_
#define __COOP__RED_BLACK_TREE_ITERATOR__H_

#ifdef __cplusplus
extern "C" {
#endif

#include "coop.h"

	//this is here instead of in redBlackTree.h because of forward declaration issues
	//A Red-Black tree node structure
	typedef struct node_t
	{
		void * data;
		char color;
		bool isHead;
		struct node_t* left, * right, * parent;
	}node;

	DEF_CLASS(redBlackTreeIterator);
	node * nodePtr;
	END_DEF(redBlackTreeIterator);

	FUNCTIONS(redBlackTreeIterator, node* node_ptr);
	MEM_FUN_DECL(redBlackTreeIterator, /*func. name:*/ increment);
	MEM_FUN_DECL(redBlackTreeIterator, /*func. name:*/ decrement);
	MEM_FUN_DECL(redBlackTreeIterator, /*func. name:*/ equals, redBlackTreeIterator other, bool* retVal);
	MEM_FUN_DECL(redBlackTreeIterator, /*func. name:*/ getContentsOf, node** retVal);
	END_FUNCTIONS(redBlackTreeIterator);

#ifdef __cplusplus
}
#endif


#endif
