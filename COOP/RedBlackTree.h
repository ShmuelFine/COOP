#ifndef __COOP__RED_BLACK_TREE__H_
#define __COOP__RED_BLACK_TREE__H_


#include "coop.h"
#include "RedBlackTreeIterator.h"

//https://gist.github.com/VictorGarritano/5f894be162d39e9bdd5c

//moved to redBlackTreeIterator.h because of forward declaration issues
//A Red-Black tree node structure
//typedef struct node_t
//{
//	int data;
//	char color;
//	bool isHead;
//	struct node_t * left, * right, * parent;
//}node;


DEF_CLASS(redBlackTree);
node* root;
//head=>left child = root, right child = largest value, parent of root
node* head;
int size;
//<
void (*comparisonFunctionPtr)(void*, void*, bool*);
//int elementSize;
END_DEF(redBlackTree);

FUNCTIONS(redBlackTree, /*ctor params:*/void (*compFunc)(void*, void*, bool*));
MEM_FUN_DECL(redBlackTree, /*func. name:*/ LeftRotate, node* x);
MEM_FUN_DECL(redBlackTree, RightRotate, node* y);
//insert helper
MEM_FUN_DECL(redBlackTree, insertFixUp, node* z);
MEM_FUN_DECL(redBlackTree, insert, void* data, node** insertedNode, bool* retBool);
MEM_FUN_DECL(redBlackTree, getRootNode, node** retRootNode);
MEM_FUN_DECL(redBlackTree, inOrderTraversal, node* rootNode);
MEM_FUN_DECL(redBlackTree, begin, redBlackTreeIterator* beginIter);
MEM_FUN_DECL(redBlackTree, end, redBlackTreeIterator* endIter);
MEM_FUN_DECL(redBlackTree, size, int* _size);
MEM_FUN_DECL(redBlackTree, find, void* val, redBlackTreeIterator* foundVal);
MEM_FUN_DECL(redBlackTree, erase, void* val, int* numElemsErased);
//erase helpers
MEM_FUN_DECL(redBlackTree, deleteNode, node* dNode);
MEM_FUN_DECL(redBlackTree, fixDoubleBlack, node* node_p);
MEM_FUN_DECL(redBlackTree, findReplacingNode, node* x, node** withWhat);
//destructor helper
MEM_FUN_DECL(redBlackTree, destroyRecursive, node* nodePtr);
END_FUNCTIONS(redBlackTree);



#endif

