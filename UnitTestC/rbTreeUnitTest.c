#include "rbTreeUnitTest.h"

FUN_DECL(rectComp, void* a, void* b, bool* retBool);
FUN_DECL(intComparison, void* a, void* b, bool* retBool);

FUN_IMPL(rectComp, void* a, void* b, bool* retBool)
{
	Rectangle rectA = *(Rectangle*)a;
	Rectangle rectB = *(Rectangle*)b;
	*retBool = (rectA.hight * rectA.width) < (rectB.hight * rectB.width);
}END_FUN;
FUN_IMPL(intComparison, void* a, void* b, bool* retBool)
{
	*retBool = *(int*)a < *(int*)b;
}END_FUN;

TEST_FUN_IMPL(RedBlackTreeTest, insert_SanityTest)
{
	//also checked with an inorder traversal after each insert (before switch to void *)
	//Arrange
	init_global_memory(sizeof(node) * 100, HEAP_BASED_MEMORY);

	void (*functionPtr)(void*, void*, bool*);
	functionPtr = &intComparison;

	CREATE(redBlackTree, rbTree), functionPtr CALL;

	node* retPtr = NULL, * retPtr1 = NULL, * retPtr2 = NULL, * retPtr3 = NULL,
		* retPtr4 = NULL, * retPtr5 = NULL, * retPtr6 = NULL, * retPtr7 = NULL,
		* retPtr8 = NULL, * retPtr9 = NULL, * rootPtr = NULL, * rootPtr1 = NULL,
		* rootPtr2 = NULL, * rootPtr3 = NULL;
	bool retBool = false, retBool1 = false, retBool2 = false, retBool3 = false,
		retBool4 = false, retBool5 = false, retBool6 = false, retBool7 = false,
		retBool8 = true, retBool9 = true;

	int treeSize;

	int a = 10, b = 20, c = 40, d = 30, e = 50, f = 25, g = 35,
		h = 37, i = 10, j = 37;
	//Act
	MFUN(&rbTree, insert), & a, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & b, & retPtr1, & retBool1 CALL;
	MFUN(&rbTree, insert), & c, & retPtr2, & retBool2 CALL;
	MFUN(&rbTree, getRootNode), & rootPtr1 CALL;
	MFUN(&rbTree, insert), & d, & retPtr3, & retBool3 CALL;
	MFUN(&rbTree, insert), & e, & retPtr4, & retBool4 CALL;
	MFUN(&rbTree, insert), & f, & retPtr6, & retBool6 CALL;
	MFUN(&rbTree, insert), & g, & retPtr5, & retBool5 CALL;
	MFUN(&rbTree, getRootNode), & rootPtr2 CALL;
	MFUN(&rbTree, insert), & h, & retPtr7, & retBool7 CALL;
	MFUN(&rbTree, getRootNode), & rootPtr3 CALL;
	MFUN(&rbTree, insert), & i, & retPtr8, & retBool8 CALL;
	MFUN(&rbTree, insert), & j, & retPtr9, & retBool9 CALL;


	MFUN(&rbTree, getRootNode), & rootPtr CALL;
	MFUN(&rbTree, printInOrderTraversal), rootPtr CALL;

	MFUN(&rbTree, size), & treeSize CALL;

	//Assert
	NTEST_ASSERT(retBool == true);
	NTEST_ASSERT(retBool1 == true);
	NTEST_ASSERT(retBool2 == true);
	NTEST_ASSERT(retBool3 == true);
	NTEST_ASSERT(retBool4 == true);
	NTEST_ASSERT(retBool5 == true);
	NTEST_ASSERT(retBool6 == true);
	NTEST_ASSERT(retBool7 == true);
	NTEST_ASSERT(retBool8 == false);
	NTEST_ASSERT(retBool9 == false);

	NTEST_ASSERT(*(int*)(retPtr->data) == 10);
	NTEST_ASSERT(*(int*)(retPtr1->data) == 20);
	NTEST_ASSERT(*(int*)(retPtr2->data) == 40);
	NTEST_ASSERT(*(int*)(retPtr3->data) == 30);
	NTEST_ASSERT(*(int*)(retPtr4->data) == 50);
	NTEST_ASSERT(*(int*)(retPtr5->data) == 35);
	NTEST_ASSERT(*(int*)(retPtr6->data) == 25);
	NTEST_ASSERT(*(int*)(retPtr7->data) == 37);
	NTEST_ASSERT(*(int*)(retPtr8->data) == 10);
	NTEST_ASSERT(*(int*)(retPtr9->data) == 37);

	//check that rebalancing happens and doesn't happen at the right places
	NTEST_ASSERT(*(int*)(rootPtr1->data) == 20);
	NTEST_ASSERT(*(int*)(rootPtr2->data) == 20);
	NTEST_ASSERT(*(int*)(rootPtr3->data) == 30);

	NTEST_ASSERT(treeSize == 8);

}END_FUN;
TEST_FUN_IMPL(RedBlackTreeTest, find_SanityTest)
{
	init_global_memory(sizeof(node) * 100, HEAP_BASED_MEMORY);

	void (*functionPtr)(void*, void*, bool*);
	functionPtr = &intComparison;

	CREATE(redBlackTree, rbTree), functionPtr CALL;

	bool retBool;
	node* retPtr, * retNode1, * retNode2, * retNode3;
	int a = 10, b = 20, c = 40, d = 30, e = 50, f = 65;
	MFUN(&rbTree, insert), & a, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & b, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & c, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & d, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & e, & retPtr, & retBool CALL;

	CREATE(redBlackTreeIterator, rbIt), retPtr CALL;

	MFUN(&rbTree, find), & b, & rbIt CALL;
	MFUN(&rbIt, getContentsOf), & retNode1 CALL;

	MFUN(&rbTree, find), & e, & rbIt CALL;
	MFUN(&rbIt, getContentsOf), & retNode2 CALL;

	MFUN(&rbTree, find), & f, & rbIt CALL;
	MFUN(&rbIt, getContentsOf), & retNode3 CALL;

	NTEST_ASSERT(*(int*)(retNode1->data) == 20);
	NTEST_ASSERT(*(int*)(retNode2->data) == 50);
	NTEST_ASSERT(retNode3->data == NULL) //value of header node (end());

}END_FUN;
TEST_FUN_IMPL(RedBlackTreeTest, delete_SanityTest)
{
	//also checked the printInOrderTraversal after each erase-before switch to void *
	//and some after
	//ARRANGE
	init_global_memory(sizeof(node) * 100, HEAP_BASED_MEMORY);

	void (*functionPtr)(void*, void*, bool*);
	functionPtr = &intComparison;

	CREATE(redBlackTree, rbTree), functionPtr CALL;


	node* retPtr = NULL, * rootPtr2 = NULL, * rootPtr3 = NULL, * retNode1 = NULL,
		* retNode2 = NULL, * retNode3 = NULL, * retNode4 = NULL, * retNode5 = NULL,
		* rootPtr;
	bool retBool = false;

	int treeSize, treeSize1, numElemsErased1, numElemsErased2, numElemsErased3,
		numElemsErased4;
	int a = 7, b = 3, c = 18, d = 10, e = 22, f = 8, g = 11, h = 26,
		i = 2, j = 6, k = 13, l = 35;
	//Act
	MFUN(&rbTree, insert), & a, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & b, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & c, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & d, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & e, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & f, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & g, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & h, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & i, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & j, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & k, & retPtr, & retBool CALL;

	MFUN(&rbTree, erase), & c, & numElemsErased1 CALL;
	MFUN(&rbTree, getRootNode), & rootPtr CALL;
	NTEST_ASSERT(*(int*)(rootPtr->data) == 10);
	NTEST_ASSERT(*(int*)(rootPtr->right->data) == 13);

	MFUN(&rbTree, erase), & a, & numElemsErased2 CALL;
	MFUN(&rbTree, getRootNode), & rootPtr CALL;
	NTEST_ASSERT(*(int*)(rootPtr->data) == 10);
	NTEST_ASSERT(*(int*)(rootPtr->left->data) == 6);

	MFUN(&rbTree, erase), & d, & numElemsErased3 CALL;
	MFUN(&rbTree, getRootNode), & rootPtr CALL;
	NTEST_ASSERT(*(int*)(rootPtr->data) == 8);
	NTEST_ASSERT(*(int*)(rootPtr->left->data) == 3);
	NTEST_ASSERT(*(int*)(rootPtr->right->data) == 13);
	MFUN(&rbTree, size), & treeSize CALL;

	MFUN(&rbTree, erase), & l, & numElemsErased4 CALL;
	MFUN(&rbTree, getRootNode), & rootPtr CALL;
	NTEST_ASSERT(*(int*)(rootPtr->data) == 8);
	MFUN(&rbTree, size), & treeSize1 CALL;

	MFUN(&rbTree, getRootNode), & rootPtr CALL;
	MFUN(&rbTree, printInOrderTraversal), rootPtr CALL;

	CREATE(redBlackTreeIterator, rbIt), retPtr CALL;

	MFUN(&rbTree, find), & c, & rbIt CALL;
	MFUN(&rbIt, getContentsOf), & retNode1 CALL;

	MFUN(&rbTree, find), & a, & rbIt CALL;
	MFUN(&rbIt, getContentsOf), & retNode2 CALL;

	MFUN(&rbTree, find), & d, & rbIt CALL;
	MFUN(&rbIt, getContentsOf), & retNode3 CALL;

	MFUN(&rbTree, find), & l, & rbIt CALL;
	MFUN(&rbIt, getContentsOf), & retNode4 CALL;

	MFUN(&rbTree, find), & k, & rbIt CALL;
	MFUN(&rbIt, getContentsOf), & retNode5 CALL;

	//Assert
	//check that the correct number of deleted elements is returned
	NTEST_ASSERT(numElemsErased1 == 1);
	NTEST_ASSERT(numElemsErased2 == 1);
	NTEST_ASSERT(numElemsErased3 == 1);
	NTEST_ASSERT(numElemsErased4 == 0);

	//size is correctly maintained
	NTEST_ASSERT(treeSize == 8);
	NTEST_ASSERT(treeSize1 == 8);

	//erased data is no longer in the tree
	NTEST_ASSERT(retNode1->data == NULL);
	NTEST_ASSERT(retNode2->data == NULL);
	NTEST_ASSERT(retNode3->data == NULL);
	NTEST_ASSERT(retNode4->data == NULL);
	NTEST_ASSERT(*(int*)(retNode5->data) == 13);


}END_FUN;
TEST_FUN_IMPL(RedBlackTreeTest, iteration_SanityTest)
{
	init_global_memory(sizeof(node) * 100, HEAP_BASED_MEMORY);

	void (*functionPtr)(void*, void*, bool*);
	functionPtr = &intComparison;

	CREATE(redBlackTree, rbTree), functionPtr CALL;

	bool retBool;
	node* retPtr, * beginNode, * endNode;

	int a = 7, b = 3, c = 18, d = 10, e = 22, f = 8, g = 11, h = 26,
		i = 2, j = 6, k = 13;

	MFUN(&rbTree, insert), & a, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & b, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & c, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & d, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & e, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & f, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & g, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & h, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & i, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & j, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & k, & retPtr, & retBool CALL;

	CREATE(redBlackTreeIterator, beginIt), retPtr CALL;
	MFUN(&rbTree, begin), & beginIt CALL;
	MFUN(&beginIt, getContentsOf), & beginNode CALL;

	CREATE(redBlackTreeIterator, endIt), retPtr CALL;
	MFUN(&rbTree, end), & endIt CALL;
	MFUN(&endIt, getContentsOf), & endNode CALL;

	//should be 2, 3, 6, 7, 8, 10, 11, 13, 18, 22, 26
	bool isAtEnd = false;
	while (!isAtEnd)
	{
		SCOPE_START;
		node* intermediateNode = NULL;
		int intermediateVal = 0;
		MFUN(&beginIt, getContentsOf), & intermediateNode CALL;
		intermediateVal = *(int*)(intermediateNode->data);
		printf("%d ", intermediateVal);

		MFUN(&beginIt, increment) CALL;
		MFUN(&beginIt, equals), endIt, & isAtEnd CALL;
		END_SCOPE;
	}

	//check begin() and end()
	NTEST_ASSERT(*(int*)(beginNode->data) == 2);
	//data of head ptr
	NTEST_ASSERT(endNode->data == NULL);

}END_FUN;
TEST_FUN_IMPL(RedBlackTreeTest, rectangle_as_node_data_insert_SanityTest)
{
	init_global_memory(sizeof(node) * 100, HEAP_BASED_MEMORY);

	void (*functionPtr)(void*, void*, bool*);
	functionPtr = &rectComp;

	bool retBool1 = false, retBool2 = false, retBool3 = false, retBool4 = false,
		retBool5 = false, retBool6 = false, retBool7 = false, retBool8 = false;
	node* retPtr1, * retPtr2, * retPtr3, * retPtr4, * retPtr5, * retPtr6,
		* retPtr7, * retPtr8, * rootPtr1, * rootPtr2, * rootPtr3, * rootPtr4,
		* rootPtr5, * rootPtr6, * rootPtr7;
	CREATE(redBlackTree, rbTree), functionPtr CALL;

	CREATE(Rectangle, rect1), 1, 1 CALL;
	CREATE(Rectangle, rect2), 2, 2 CALL;
	CREATE(Rectangle, rect3), 3, 3 CALL;
	CREATE(Rectangle, rect4), 4, 4 CALL;
	CREATE(Rectangle, rect5), 5, 5 CALL;
	CREATE(Rectangle, rect6), 6, 6 CALL;
	CREATE(Rectangle, rect7), 7, 7 CALL;

	//test insert operation
	MFUN(&rbTree, insert), & rect1, & retPtr1, & retBool1 CALL;

	MFUN(&rbTree, getRootNode), & rootPtr1 CALL;
	NTEST_ASSERT((*(Rectangle*)(rootPtr1->data)).width == 1);
	NTEST_ASSERT(retBool1 == true);
	NTEST_ASSERT((*(Rectangle*)(retPtr1->data)).width == 1);

	MFUN(&rbTree, insert), & rect2, & retPtr2, & retBool2 CALL;

	MFUN(&rbTree, getRootNode), & rootPtr2 CALL;
	NTEST_ASSERT((*(Rectangle*)(rootPtr2->data)).width == 1);
	NTEST_ASSERT((*(Rectangle*)(rootPtr2->right->data)).width == 2);
	NTEST_ASSERT(retBool2 == true);
	NTEST_ASSERT((*(Rectangle*)(retPtr2->data)).width == 2);

	MFUN(&rbTree, insert), & rect3, & retPtr3, & retBool3 CALL;
	MFUN(&rbTree, getRootNode), & rootPtr3 CALL;
	NTEST_ASSERT((*(Rectangle*)(rootPtr3->data)).width == 2);
	NTEST_ASSERT((*(Rectangle*)(rootPtr3->right->data)).width == 3);
	NTEST_ASSERT((*(Rectangle*)(rootPtr3->left->data)).width == 1);
	NTEST_ASSERT(retBool3 == true);
	NTEST_ASSERT((*(Rectangle*)(retPtr3->data)).width == 3);

	MFUN(&rbTree, insert), & rect4, & retPtr4, & retBool4 CALL;
	MFUN(&rbTree, getRootNode), & rootPtr4 CALL;
	NTEST_ASSERT((*(Rectangle*)(rootPtr4->data)).width == 2);
	NTEST_ASSERT((*(Rectangle*)(rootPtr4->right->right->data)).width == 4);
	NTEST_ASSERT((*(Rectangle*)(rootPtr4->left->data)).width == 1);
	NTEST_ASSERT(retBool4 == true);
	NTEST_ASSERT((*(Rectangle*)(retPtr4->data)).width == 4);

	MFUN(&rbTree, insert), & rect5, & retPtr5, & retBool5 CALL;
	MFUN(&rbTree, getRootNode), & rootPtr5 CALL;
	NTEST_ASSERT((*(Rectangle*)(rootPtr5->data)).width == 2);
	NTEST_ASSERT((*(Rectangle*)(rootPtr5->right->data)).width == 4);
	NTEST_ASSERT((*(Rectangle*)(rootPtr5->right->right->data)).width == 5);
	NTEST_ASSERT((*(Rectangle*)(rootPtr5->right->left->data)).width == 3);
	NTEST_ASSERT((*(Rectangle*)(rootPtr5->left->data)).width == 1);
	NTEST_ASSERT(retBool5 == true);
	NTEST_ASSERT((*(Rectangle*)(retPtr5->data)).width == 5);

	MFUN(&rbTree, insert), & rect6, & retPtr6, & retBool6 CALL;
	MFUN(&rbTree, getRootNode), & rootPtr6 CALL;

	NTEST_ASSERT((*(Rectangle*)(rootPtr6->data)).width == 2);
	NTEST_ASSERT(retBool6 == true);
	NTEST_ASSERT((*(Rectangle*)(retPtr6->data)).width == 6);

	MFUN(&rbTree, insert), & rect7, & retPtr7, & retBool7 CALL;
	MFUN(&rbTree, getRootNode), & rootPtr7 CALL;
	NTEST_ASSERT((*(Rectangle*)(rootPtr7->data)).width == 2);
	NTEST_ASSERT((*(Rectangle*)(rootPtr7->left->data)).width == 1);
	NTEST_ASSERT((*(Rectangle*)(rootPtr7->right->data)).width == 4);
	NTEST_ASSERT((*(Rectangle*)(rootPtr7->right->right->data)).width == 6);
	NTEST_ASSERT((*(Rectangle*)(rootPtr7->right->left->data)).width == 3);
	NTEST_ASSERT((*(Rectangle*)(rootPtr7->right->right->right->data)).width == 7);
	NTEST_ASSERT(retBool7 == true);
	NTEST_ASSERT((*(Rectangle*)(retPtr7->data)).width == 7);

	MFUN(&rbTree, insert), & rect3, & retPtr8, & retBool8 CALL;
	NTEST_ASSERT(retBool8 == false);
	NTEST_ASSERT((*(Rectangle*)(retPtr8->data)).width == 3);
	NTEST_ASSERT(retPtr8 == retPtr3);
}END_FUN;
TEST_FUN_IMPL(RedBlackTreeTest, rectangle_as_node_data_find_SanityTest)
{
	init_global_memory(sizeof(node) * 100, HEAP_BASED_MEMORY);

	void (*functionPtr)(void*, void*, bool*);
	functionPtr = &rectComp;

	bool retBool1 = false;
	node* retPtr1, * retNode1, * retNode2, * retNode3, * retNode4;
	CREATE(redBlackTree, rbTree), functionPtr CALL;

	CREATE(Rectangle, rect1), 1, 1 CALL;
	CREATE(Rectangle, rect2), 2, 2 CALL;
	CREATE(Rectangle, rect3), 3, 3 CALL;
	CREATE(Rectangle, rect4), 4, 4 CALL;

	MFUN(&rbTree, insert), & rect1, & retPtr1, & retBool1 CALL;
	MFUN(&rbTree, insert), & rect2, & retPtr1, & retBool1 CALL;
	MFUN(&rbTree, insert), & rect3, & retPtr1, & retBool1 CALL;

	CREATE(redBlackTreeIterator, rbIt), retPtr1 CALL;

	MFUN(&rbTree, find), & rect1, & rbIt CALL;
	MFUN(&rbIt, getContentsOf), & retNode1 CALL;

	MFUN(&rbTree, find), & rect2, & rbIt CALL;
	MFUN(&rbIt, getContentsOf), & retNode2 CALL;

	MFUN(&rbTree, find), & rect3, & rbIt CALL;
	MFUN(&rbIt, getContentsOf), & retNode3 CALL;

	MFUN(&rbTree, find), & rect4, & rbIt CALL;
	MFUN(&rbIt, getContentsOf), & retNode4 CALL;

	NTEST_ASSERT((*(Rectangle*)(retNode1->data)).width == 1);
	NTEST_ASSERT((Rectangle*)(retNode1->data) = &rect1);

	NTEST_ASSERT((*(Rectangle*)(retNode2->data)).width == 2);
	NTEST_ASSERT((Rectangle*)(retNode2->data) = &rect2);

	NTEST_ASSERT((*(Rectangle*)(retNode3->data)).width == 3);
	NTEST_ASSERT((Rectangle*)(retNode3->data) = &rect3);

	NTEST_ASSERT(retNode4->data == NULL); //value of header node (end());
}END_FUN;
TEST_FUN_IMPL(RedBlackTreeTest, rectangle_as_node_data_delete_SanityTest)
{
	init_global_memory(sizeof(node) * 100, HEAP_BASED_MEMORY);

	void (*functionPtr)(void*, void*, bool*);
	functionPtr = &rectComp;

	bool retBool1 = false;
	node* retPtr1, * retNode, * rootPtr1, * rootPtr2, * rootPtr3;
	int numElemsErased1, numElemsErased2, numElemsErased3;

	CREATE(redBlackTree, rbTree), functionPtr CALL;

	CREATE(Rectangle, rect1), 1, 1 CALL;
	CREATE(Rectangle, rect2), 2, 2 CALL;
	CREATE(Rectangle, rect3), 3, 3 CALL;
	CREATE(Rectangle, rect4), 4, 4 CALL;
	CREATE(Rectangle, rect5), 5, 5 CALL;
	CREATE(Rectangle, rect6), 6, 6 CALL;
	CREATE(Rectangle, rect7), 7, 7 CALL;

	MFUN(&rbTree, insert), & rect1, & retPtr1, & retBool1 CALL;
	MFUN(&rbTree, insert), & rect2, & retPtr1, & retBool1 CALL;
	MFUN(&rbTree, insert), & rect3, & retPtr1, & retBool1 CALL;
	MFUN(&rbTree, insert), & rect4, & retPtr1, & retBool1 CALL;
	MFUN(&rbTree, insert), & rect5, & retPtr1, & retBool1 CALL;
	MFUN(&rbTree, insert), & rect6, & retPtr1, & retBool1 CALL;

	MFUN(&rbTree, erase), & rect2, & numElemsErased1 CALL;
	MFUN(&rbTree, getRootNode), & rootPtr1 CALL;
	NTEST_ASSERT((*(Rectangle*)(rootPtr1->data)).width == 4);
	NTEST_ASSERT((*(Rectangle*)(rootPtr1->left->data)).width == 1);
	NTEST_ASSERT((*(Rectangle*)(rootPtr1->right->data)).width == 5);
	NTEST_ASSERT(numElemsErased1 == 1);

	MFUN(&rbTree, erase), & rect5, & numElemsErased2 CALL;
	MFUN(&rbTree, getRootNode), & rootPtr2 CALL;
	NTEST_ASSERT((*(Rectangle*)(rootPtr2->data)).width == 4);
	NTEST_ASSERT((*(Rectangle*)(rootPtr1->left->data)).width == 1);
	NTEST_ASSERT((*(Rectangle*)(rootPtr1->right->data)).width == 6);
	NTEST_ASSERT(numElemsErased2 == 1);

	MFUN(&rbTree, erase), & rect7, & numElemsErased3 CALL;
	MFUN(&rbTree, getRootNode), & rootPtr3 CALL;
	NTEST_ASSERT((*(Rectangle*)(rootPtr3->data)).width == 4);
	NTEST_ASSERT(numElemsErased3 == 0);

	CREATE(redBlackTreeIterator, rbIt), retPtr1 CALL;

	MFUN(&rbTree, find), & rect5, & rbIt CALL;
	MFUN(&rbIt, getContentsOf), & retNode CALL;

	NTEST_ASSERT(retNode->data == NULL); //value of header node (end());
}END_FUN;



INIT_TEST_SUITE(RedBlackTreeTest);
BIND_TEST(RedBlackTreeTest, insert_SanityTest);
BIND_TEST(RedBlackTreeTest, find_SanityTest);
BIND_TEST(RedBlackTreeTest, delete_SanityTest);
BIND_TEST(RedBlackTreeTest, iteration_SanityTest);
BIND_TEST(RedBlackTreeTest, rectangle_as_node_data_insert_SanityTest);
BIND_TEST(RedBlackTreeTest, rectangle_as_node_data_find_SanityTest);
BIND_TEST(RedBlackTreeTest, rectangle_as_node_data_delete_SanityTest);
END_INIT_TEST_SUITE(RedBlackTreeTest);