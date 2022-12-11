#ifndef __COOP__RED_BLACK_TREE__H_
#define __COOP__RED_BLACK_TREE__H_


#include "coop.h"

// following https://gist.github.com/VictorGarritano/5f894be162d39e9bdd5c
///////////////////////////////////////////////////
DEF_CLASS(RBTreeNode);
object* data;
bool isBlack;
//bool isHead;
struct RBTreeNode_t* left; 
struct RBTreeNode_t* right;
struct RBTreeNode_t* parent;
END_DEF(RBTreeNode);
FUNCTIONS(RBTreeNode);
END_FUNCTIONS(RBTreeNode);

///////////////////////////////////////////////////

DEF_CLASS(RedBlackTreeIterator);
RBTreeNode* nodePtr;
END_DEF(RedBlackTreeIterator);

FUNCTIONS(RedBlackTreeIterator, RBTreeNode* node_ptr);
MEM_FUN_DECL(RedBlackTreeIterator, next);
MEM_FUN_DECL(RedBlackTreeIterator, equals, RedBlackTreeIterator other, bool* retVal);
END_FUNCTIONS(RedBlackTreeIterator);

///////////////////////////////////////////////////

DEF_CLASS(RedBlackTree);
RBTreeNode* root;

// head->left=root; head->right=largest value, parent of root ???
//RBTreeNode* head;
int size;
bool (*comparisonFunctionPtr)(object*, object*);
END_DEF(RedBlackTree);

FUNCTIONS(RedBlackTree, bool (*compFunc)(object*, object*));
MEM_FUN_DECL(RedBlackTree, transplant, RBTreeNode* old_node, RBTreeNode* new_node);
MEM_FUN_DECL(RedBlackTree, _rotateLeft, RBTreeNode* x);
MEM_FUN_DECL(RedBlackTree, _rotateRight, RBTreeNode* y);

//insert helper
MEM_FUN_DECL(RedBlackTree, _insertFixUp, RBTreeNode* z);
MEM_FUN_DECL(RedBlackTree, insert, object* toInsert, RBTreeNode** OUT_insertedNode, bool* OUT_DidInsertionAddNewElement);
MEM_FUN_DECL(RedBlackTree, getRootNode, RBTreeNode** retRootNode);
MEM_FUN_DECL(RedBlackTree, printInOrderTraversal, RBTreeNode* rootNode, void (*printFunc)(object*));
//MEM_FUN_DECL(RedBlackTree, begin, RedBlackTreeIterator* beginIter);
//MEM_FUN_DECL(RedBlackTree, end, RedBlackTreeIterator* endIter);
MEM_FUN_DECL(RedBlackTree, size, int* _size);
MEM_FUN_DECL(RedBlackTree, find_closest, object* val, RBTreeNode** node, bool * isFound);
MEM_FUN_DECL(RedBlackTree, find, object* val, RBTreeNode** node);
MEM_FUN_DECL(RedBlackTree, erase, object* val, bool* isErased);

//erase helpers
MEM_FUN_DECL(RedBlackTree, _deleteNode, RBTreeNode* dNode);
MEM_FUN_DECL(RedBlackTree, fixDoubleBlack, RBTreeNode* node_p);
MEM_FUN_DECL(RedBlackTree, _findReplacingNode, RBTreeNode* x, RBTreeNode** withWhat);

//destructor helper
MEM_FUN_DECL(RedBlackTree, _destroyRecursive, RBTreeNode* nodePtr);
END_FUNCTIONS(RedBlackTree);



#endif
