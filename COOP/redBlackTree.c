#include "RedBlackTree.h"
#include <stdio.h>

DEF_CTOR(RBTreeNode)
{
	_this->data = NULL;
	_this->parent = _this->right = _this->left = NULL;
	_this->isBlack = true;
}
END_CTOR

DEF_DTOR(RBTreeNode)
{
	if (_this->data)
	{
		DELETE(_this->data);
	}
}
END_DTOR

////////////////// AUX FUNCS ///////////////////
RBTreeNode* all_way_left(RBTreeNode* node) {
	while (node->left != NULL)
		node = node->left;
	return node;
}
RBTreeNode* all_way_right(RBTreeNode* node) {
	while (node->right != NULL)
		node = node->right;
	return node;
}

void add_right_child(RBTreeNode* node, RBTreeNode* child)
{
	node->right = child;
	if (child)
		child->parent = node;
}
void add_left_child(RBTreeNode* node, RBTreeNode* child)
{
	node->left = child;
	if (child)
		child->parent = node;
}

void swap_colors(RBTreeNode* a, RBTreeNode* b) {
	bool a_isBlack = a->isBlack;
	a->isBlack = b->isBlack;
	b->isBlack = a_isBlack;
}

#define IS_RED(n) (n && n->isBlack == false)

#define IS_LEFT_CHILD(n) (n && n->parent && n->parent->left == n)
#define IS_RIGHT_CHILD(n) (n && n->parent && n->parent->right == n)

#define PARENT(n)  (n->parent)
#define GRAND_PARENT(n)  (n->parent->parent)
#define UNCLE(n)  (IS_LEFT_CHILD(n) ? n->parent->right : n->parent->left)
#define SIBLING(n) ((NULL == PARENT(n)) ? NULL : IS_LEFT_CHILD(n) ? PARENT(n)->right : PARENT(n)->left)
#define IS_LEAF(n) (!n->left && !n->right)
#define HAS_SINGLE_CHILD(n) ((NULL == n->left) ^ (NULL == n->right))

////////////////////////////////////////////////

DEF_CTOR(RedBlackTreeIterator, RBTreeNode* node_ptr)
{
	_this->nodePtr = node_ptr;
}
END_CTOR

DEF_DTOR(RedBlackTreeIterator)
{
	// nothing needed here
}
END_DTOR

MEM_FUN_IMPL(RedBlackTreeIterator, next)
{
	if (_this->nodePtr->right != NULL)
	{
		_this->nodePtr = all_way_left(_this->nodePtr->right);
	}
	else
	{
		// I have no right child, left nodes were already visited, so go up:
		_this->nodePtr = _this->nodePtr->parent;

		WHILE(
			_this->nodePtr->parent
			&& _this->nodePtr == _this->nodePtr->parent->right)
		{
			_this->nodePtr = _this->nodePtr->parent;
		}END_LOOP;
	}
}
END_FUN;


MEM_FUN_IMPL(RedBlackTreeIterator, equals, RedBlackTreeIterator other, bool* retVal)
{
	*retVal = (_this->nodePtr == other.nodePtr);
}
END_FUN;

INIT_CLASS(RedBlackTreeIterator)
BIND(RedBlackTreeIterator, next);
BIND(RedBlackTreeIterator, equals);
END_INIT_CLASS(RedBlackTreeIterator)


////////////////////////////////////////////////

DEF_CTOR(RedBlackTree, bool (*compFunc)(object*, object*))
{
	_this->root = NULL;
	_this->size = 0;
	_this->comparisonFunctionPtr = compFunc;

}
END_CTOR

DEF_DTOR(RedBlackTree)
{
	if (_this->root)
	{
		MFUN(_this, _destroyRecursive), _this->root CALL;
	}
}
END_DTOR

MEM_FUN_IMPL(RedBlackTree, _destroyRecursive, RBTreeNode* nodePtr)
{
	if (nodePtr)
	{
		MFUN(_this, _destroyRecursive), nodePtr->left CALL;
		MFUN(_this, _destroyRecursive), nodePtr->right CALL;
		DELETE(nodePtr);
	}
}END_FUN;


MEM_FUN_IMPL(RedBlackTree, transplant, RBTreeNode* old_node, RBTreeNode* new_node) {
	if (old_node->parent == NULL) {
		_this->root = new_node;
	}
	else if (old_node == old_node->parent->left) {
		old_node->parent->left = new_node;
	}
	else {
		old_node->parent->right = new_node;
	}

	if (new_node) {
		new_node->parent = old_node->parent;
	}


}END_FUN;

MEM_FUN_IMPL(RedBlackTree, _rotateLeft, RBTreeNode* x)
{
	if (!x || !x->right)
		RETURN;

	RBTreeNode* y = x->right;

	MFUN(_this, transplant), x, y CALL;

	add_right_child(x, y->left);
	add_left_child(y, x);
}

END_FUN;

MEM_FUN_IMPL(RedBlackTree, _rotateRight, RBTreeNode* y)
{
	if (!y || !y->left)
		RETURN;

	RBTreeNode* x = y->left;
	MFUN(_this, transplant), y, x CALL;

	add_left_child(y, x->right);
	add_right_child(x, y);
}END_FUN;


MEM_FUN_IMPL(RedBlackTree, _insertFixUp, RBTreeNode* node)
{
	WHILE(
		(node != _this->root)
		&& // both node and parent are RED:
		IS_RED(node) && IS_RED(node->parent))
	{
		RBTreeNode* uncle = UNCLE(node);
		if (IS_RED(uncle))
		{
			uncle->isBlack = true;
			PARENT(node)->isBlack = true;
			GRAND_PARENT(node)->isBlack = false;

			node = GRAND_PARENT(node);
		}
		else
		{
			if (IS_LEFT_CHILD(PARENT(node)))
			{
				if (IS_RIGHT_CHILD(node))
				{
					MFUN(_this, _rotateLeft), PARENT(node) CALL;
					node = PARENT(node);
				}
				else
				{
					MFUN(_this, _rotateRight), GRAND_PARENT(node) CALL;
					swap_colors(PARENT(node), GRAND_PARENT(node));

					node = PARENT(node);
				}
			}
			else
			{
				if (IS_LEFT_CHILD(node))
				{
					MFUN(_this, _rotateRight), PARENT(node) CALL;
					node = PARENT(node);
				}
				else
				{
					MFUN(_this, _rotateLeft), GRAND_PARENT(node) CALL;
					swap_colors(PARENT(node), GRAND_PARENT(node));

					node = PARENT(node);
				}
			}
		}
	}END_LOOP;

	_this->root->isBlack = true;

}END_FUN;


MEM_FUN_IMPL(RedBlackTree, find_closest, object* val, RBTreeNode** node, bool* isFound)
{
	*node = NULL;
	*isFound = false;

	if (_this->root == NULL)
	{
		RETURN;
	}

	RBTreeNode* curr = _this->root;
	WHILE(curr != NULL) {
		*node = curr;

		bool val_lessThen_curr = _this->comparisonFunctionPtr(val, curr->data);
		bool curr_lessThen_val = _this->comparisonFunctionPtr(curr->data, val);
		bool curr_equals_val = !curr_lessThen_val && !val_lessThen_curr;

		if (val_lessThen_curr)
		{
			if (curr->left == NULL) { RETURN; }
			else { curr = curr->left; }
		}
		else if (curr_equals_val) {
			*isFound = true;
			RETURN;
		}
		else {
			if (curr->right == NULL) { RETURN; }
			else { curr = curr->right; }
		}
	}END_LOOP;
}END_FUN;

MEM_FUN_IMPL(RedBlackTree, find, object* val, RBTreeNode** node)
{
	*node = NULL;

	RBTreeNode* closest; bool isFound = false;
	MFUN(_this, find_closest), val, & closest, & isFound CALL;

	if (isFound)
		*node = closest;

}END_FUN;

MEM_FUN_IMPL(RedBlackTree, insert, object* toInsert, RBTreeNode** OUT_insertedNode, bool* OUT_DidInsertionAddNewElement)
{
	RBTreeNode* closest; bool isFound = false;
	MFUN(_this, find_closest), toInsert, & closest, & isFound CALL;
	if (isFound)
	{
		*OUT_insertedNode = closest;
		*OUT_DidInsertionAddNewElement = false;
		RETURN;
	}

	if (_this->root == NULL)
	{
		ALLOC_INIT_INSTANCE_PTR(RBTreeNode, _this->root) CALL;
		_this->root->data = toInsert;
		*OUT_insertedNode = _this->root;
	}
	else
	{
		RBTreeNode* parent = closest;
		CREATE_PTR(RBTreeNode, newNode) CALL;
		newNode->data = toInsert;
		newNode->parent = parent;
		newNode->isBlack = false;
		*OUT_insertedNode = newNode;
		bool parent_lessThan_toInsert = _this->comparisonFunctionPtr(parent->data, newNode->data);
		if (parent_lessThan_toInsert)
			add_left_child(parent, newNode);
		else
			add_right_child(parent, newNode);

		MFUN(_this, _insertFixUp), newNode CALL;
	}

	_this->size++;
	*OUT_DidInsertionAddNewElement = true;

}END_FUN;

MEM_FUN_IMPL(RedBlackTree, getRootNode, RBTreeNode** retRootnode)
{
	*retRootnode = _this->root;
}END_FUN;

MEM_FUN_IMPL(RedBlackTree, printInOrderTraversal, RBTreeNode* rootNode, void (*printFunc)(object*))
{
	if (rootNode == NULL)
		RETURN;

	MFUN(_this, printInOrderTraversal), rootNode->left, printFunc CALL;
	printFunc(rootNode->data);

	if (rootNode->parent == NULL)
	{
		printf(" root node ");
	}
	else
	{
		printf("Parent: "); printFunc(rootNode->parent->data);
	}

	if (rootNode->right != NULL)
	{
		printf("Right: "); printFunc(rootNode->right->data);
	}

	if (rootNode->left != NULL)
	{
		printf("Left: "); printFunc(rootNode->left->data);
	}

	printf("\n");

	MFUN(_this, printInOrderTraversal), rootNode->right, printFunc CALL;

}END_FUN;
//
//MEM_FUN_IMPL(RedBlackTree, begin, RedBlackTreeIterator* beginNode)
//{
//	RBTreeNode* minNode = _this->head;
//
//	while (minNode->left != NULL)
//	{
//		minNode = minNode->left;
//	}
//
//	CREATE(RedBlackTreeIterator, it), minNode CALL;
//
//	*beginNode = it;
//}END_FUN;
//
//MEM_FUN_IMPL(RedBlackTree, end, RedBlackTreeIterator* endNode)
//{
//	CREATE(RedBlackTreeIterator, it), _this->head CALL;
//
//	*endNode = it;
//}END_FUN;

MEM_FUN_IMPL(RedBlackTree, size, int* _size)
{
	*_size = _this->size;
}END_FUN;

MEM_FUN_IMPL(RedBlackTree, erase, object* val, bool* isErased)
{
	RBTreeNode* node = NULL;
	MFUN(_this, find), val, & node CALL;
	if (node == NULL)
	{
		*isErased = false;
		RETURN;
	}

	MFUN(_this, _deleteNode), node CALL;
	*isErased = true;
	_this->size--;

}END_FUN;

//https://www.geeksforgeeks.org/red-black-tree-set-3-delete-2/
MEM_FUN_IMPL(RedBlackTree, _deleteNode, RBTreeNode* dNode)
{

	if (IS_LEAF(dNode)) {
		if (dNode == _this->root) {
			_this->root = NULL;
		}
		else {
			if (dNode->isBlack) {
				MFUN(_this, fixDoubleBlack), dNode CALL;
			}
			else {
				RBTreeNode* sibling = SIBLING(dNode);
				if (sibling != NULL)
					sibling->isBlack = false;
			}

			MFUN(_this, transplant), dNode, NULL CALL;
		}
		DELETE(dNode);
		RETURN;
	}

	RBTreeNode* replaceNode = NULL;
	MFUN(_this, _findReplacingNode), dNode, & replaceNode CALL;


	if (HAS_SINGLE_CHILD(dNode)) {
		MFUN(_this, transplant), dNode, replaceNode CALL;
		DELETE(dNode);

		if (replaceNode->isBlack && dNode->isBlack) {
			MFUN(_this, fixDoubleBlack), replaceNode CALL;
		}
		else {
			replaceNode->isBlack = true;
		}
		RETURN;
	}

	//// dNode has 2 children, swap values with successor and recurse 
	//object* temp = replaceNode->data;
	//replaceNode->data = dNode->data;
	//dNode->data = temp;
	//MFUN(_this, _deleteNode), replaceNode CALL;
}END_FUN;

MEM_FUN_IMPL(RedBlackTree, fixDoubleBlack, RBTreeNode* node_p)
{
	if (node_p == _this->root)
		RETURN;
	
	ASSERT_NOT_NULL(node_p);
	RBTreeNode* sibling = SIBLING(node_p);

	if (sibling == NULL) {
		MFUN(_this, fixDoubleBlack), PARENT(node_p) CALL;
		RETURN;
	}

	if (IS_RED(sibling)) {

		PARENT(node_p)->isBlack = false;
		sibling->isBlack = true;

		if (IS_LEFT_CHILD(sibling)) { MFUN(_this, _rotateRight), PARENT(node_p) CALL; }
		else { MFUN(_this, _rotateLeft), PARENT(node_p) CALL; }
		MFUN(_this, fixDoubleBlack), node_p CALL;
		RETURN;
	}


	if (IS_RED(sibling->left)) {
		if (IS_LEFT_CHILD(sibling)) {
			sibling->left->isBlack = sibling->isBlack;
			sibling->isBlack = PARENT(node_p)->isBlack;
			MFUN(_this, _rotateRight), PARENT(node_p) CALL;
		}
		else {
			sibling->left->isBlack = PARENT(node_p)->isBlack;
			MFUN(_this, _rotateRight), sibling CALL;
			MFUN(_this, _rotateLeft), PARENT(node_p) CALL;
		}
		PARENT(node_p)->isBlack = true;
	}
	else if (IS_RED(sibling->right))
	{
		if (IS_LEFT_CHILD(sibling)) {
			sibling->right->isBlack = PARENT(node_p)->isBlack;
			MFUN(_this, _rotateLeft), sibling CALL;
			MFUN(_this, _rotateRight), PARENT(node_p)CALL;
		}
		else {
			sibling->right->isBlack = sibling->isBlack;
			sibling->isBlack = PARENT(node_p)->isBlack;
			MFUN(_this, _rotateLeft), PARENT(node_p) CALL;
		}
		PARENT(node_p)->isBlack = true;
	}
	else { // 2 black children 
		sibling->isBlack = false;
		if (PARENT(node_p)->isBlack) {
			MFUN(_this, fixDoubleBlack), PARENT(node_p) CALL;
		}
		else 
		{
			PARENT(node_p)->isBlack = true;
		}
	}
}END_FUN;

MEM_FUN_IMPL(RedBlackTree, _findReplacingNode, RBTreeNode* whom, RBTreeNode** OUT_withWhat)
{
	// 2 children:
	if (whom->left && whom->right)
	{
		*OUT_withWhat = all_way_right(whom->left);
		RETURN;
	}
	// leaf:
	else if (IS_LEAF(whom))
	{
		*OUT_withWhat = NULL;
	}
	// single child:
	else if (whom->left)
		*OUT_withWhat = whom->left;
	else
		*OUT_withWhat = whom->right;
}END_FUN;

INIT_CLASS(RedBlackTree)
BIND(RedBlackTree, _rotateLeft);
BIND(RedBlackTree, _rotateRight);
BIND(RedBlackTree, _insertFixUp);
BIND(RedBlackTree, insert);
BIND(RedBlackTree, getRootNode);
BIND(RedBlackTree, printInOrderTraversal);
//BIND(RedBlackTree, begin);
//BIND(RedBlackTree, end);
BIND(RedBlackTree, size);
BIND(RedBlackTree, find);
BIND(RedBlackTree, erase);
BIND(RedBlackTree, _deleteNode);
BIND(RedBlackTree, fixDoubleBlack);
BIND(RedBlackTree, _findReplacingNode);
BIND(RedBlackTree, _destroyRecursive);
END_INIT_CLASS(RedBlackTree)