#ifndef __COOP__RED_BLACK_TREE_ITERATOR__H_
#define __COOP__RED_BLACK_TREE_ITERATOR__H_

#include "coop.h"

//this is here instead of in redBlackTree.h because of forward declaration issues
//A Red-Black tree node structure
typedef struct node_t
{
	void* data;
	char color;
	bool isHead;
	struct node_t* left, * right, * parent;
}node;

DEF_CLASS(redBlackTreeIterator);
node* nodePtr;
END_DEF(redBlackTreeIterator);

FUNCTIONS(redBlackTreeIterator, node* node_ptr);
MEM_FUN_DECL(redBlackTreeIterator, increment);
MEM_FUN_DECL(redBlackTreeIterator, decrement);
MEM_FUN_DECL(redBlackTreeIterator, equals, redBlackTreeIterator other, bool* retVal);
MEM_FUN_DECL(redBlackTreeIterator, getContentsOf, node** retVal);
END_FUNCTIONS(redBlackTreeIterator);



#endif
