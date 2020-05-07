#include "rbTreeUnitTest.h"
#include "redBlackTree.h"
#include "../COOP_C_TestProxy/Rectangle.h"


//rectangle comparison function
FUN_IMPL(rectComp, void  * a, void * b, bool * retBool)
{
	Rectangle rectA = *(Rectangle*)a;
	Rectangle rectB = *(Rectangle*)b;
	*retBool = (rectA.hight *  rectA.width) < (rectB.hight * rectB.width);
}END_FUN;

//int comparison function
FUN_IMPL(intComparison, void* a, void* b, bool* retBool)
{
	*retBool = *(int*)a < *(int*)b;
}END_FUN;

//tests insert with ints
FUN_IMPL(rbTree_insert_SanityTest)
{
	//also checked with an inorder traversal after each insert (before switch to void *)
	//Arrange
	CREATE(MemoryManager, memManager), sizeof(node) * 100, HEAP_BASED_MEMORY);

	void (*functionPtr)(void *, void * , bool *);
	functionPtr = &intComparison;

	CREATE(redBlackTree, rbTree), functionPtr);

	node * retPtr = NULL, * retPtr1 = NULL, * retPtr2 = NULL, * retPtr3 = NULL, 
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
	FUN(&rbTree, insert), &a, &retPtr, &retBool CALL
	FUN(&rbTree, insert), &b, & retPtr1, & retBool1 CALL
	FUN(&rbTree, insert), &c, & retPtr2, & retBool2 CALL
	FUN(&rbTree, getRootNode), & rootPtr1 CALL
	FUN(&rbTree, insert), &d, & retPtr3, & retBool3 CALL
    FUN(&rbTree, insert), &e, & retPtr4, & retBool4 CALL
	FUN(&rbTree, insert), &f, & retPtr6, & retBool6 CALL
	FUN(&rbTree, insert), &g, & retPtr5, & retBool5 CALL
	FUN(&rbTree, getRootNode), & rootPtr2 CALL
	FUN(&rbTree, insert), &h, & retPtr7, & retBool7 CALL
	FUN(&rbTree, getRootNode), & rootPtr3 CALL
	FUN(&rbTree, insert), &i, & retPtr8, & retBool8 CALL
	FUN(&rbTree, insert), &j, & retPtr9, & retBool9 CALL

	
	FUN(&rbTree, getRootNode), & rootPtr CALL
	FUN(&rbTree, inOrderTraversal), rootPtr CALL

	FUN(&rbTree, size), & treeSize CALL

	//Assert
	ASSERT(retBool == true);
	ASSERT(retBool1 == true);
	ASSERT(retBool2 == true);
	ASSERT(retBool3 == true);
	ASSERT(retBool4 == true);
	ASSERT(retBool5 == true);
	ASSERT(retBool6 == true);
	ASSERT(retBool7 == true);
	ASSERT(retBool8 == false);
	ASSERT(retBool9 == false);

	ASSERT(*(int *)(retPtr->data) == 10);
	ASSERT(*(int*)(retPtr1->data) == 20);
	ASSERT(*(int*)(retPtr2->data) == 40);
	ASSERT(*(int*)(retPtr3->data) == 30);
	ASSERT(*(int*)(retPtr4->data) == 50);
	ASSERT(*(int*)(retPtr5->data) == 35);
	ASSERT(*(int*)(retPtr6->data) == 25);
	ASSERT(*(int*)(retPtr7->data) == 37);
	ASSERT(*(int*)(retPtr8->data) == 10);
	ASSERT(*(int*)(retPtr9->data) == 37);

	//check that rebalancing happens and doesn't happen at the right places
	ASSERT(*(int*)(rootPtr1->data) == 20)
	ASSERT(*(int*)(rootPtr2->data) == 20)
	ASSERT(*(int*)(rootPtr3->data) == 30)

	ASSERT(treeSize == 8)

}END_FUN

//tests find with ints
FUN_IMPL(rbTree_find_SanityTest)
{
	CREATE(MemoryManager, memManager), sizeof(node) * 100, HEAP_BASED_MEMORY);

	void (*functionPtr)(void*, void*, bool*);
	functionPtr = &intComparison;

	CREATE(redBlackTree, rbTree), functionPtr);

	bool retBool;
	node* retPtr, *retNode1, *retNode2, *retNode3;
	int a = 10, b = 20, c = 40, d = 30, e = 50, f = 65;
	FUN(&rbTree, insert), &a, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &b, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &c, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &d, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &e, & retPtr, & retBool CALL

	CREATE(redBlackTreeIterator, rbIt), retPtr);

	FUN(&rbTree, find), &b, & rbIt CALL
	FUN(&rbIt, getContentsOf), & retNode1 CALL

	FUN(&rbTree, find), &e, & rbIt CALL
	FUN(&rbIt, getContentsOf), & retNode2 CALL

	FUN(&rbTree, find), &f, & rbIt CALL
	FUN(&rbIt, getContentsOf), & retNode3 CALL

	ASSERT(*(int*)(retNode1->data) == 20)
	ASSERT(*(int*)(retNode2->data) == 50)
	ASSERT(retNode3->data == NULL) //value of header node (end())

}END_FUN

//tests delete with ints
FUN_IMPL(rbTree_delete_SanityTest)
{
	//also checked the inOrderTraversal after each erase-before switch to void *
	//and some after
	//ARRANGE
	CREATE(MemoryManager, memManager), sizeof(node) * 100, HEAP_BASED_MEMORY);

	void (*functionPtr)(void*, void*, bool*);
	functionPtr = &intComparison;

	CREATE(redBlackTree, rbTree), functionPtr);
	

	node* retPtr = NULL, * rootPtr2 = NULL, * rootPtr3 = NULL, *retNode1 = NULL,
		* retNode2 = NULL, * retNode3 = NULL, * retNode4 = NULL, * retNode5 = NULL,
		*rootPtr;
	bool retBool = false;

	int treeSize, treeSize1, numElemsErased1, numElemsErased2, numElemsErased3,
		numElemsErased4;
	int a = 7, b = 3, c = 18, d = 10, e = 22, f = 8, g = 11, h = 26,
		i = 2, j = 6, k = 13, l = 35;
	//Act
	FUN(&rbTree, insert), &a, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &b, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &c, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &d, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &e, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &f, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &g, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &h, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &i, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &j, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &k, & retPtr, & retBool CALL

	FUN(&rbTree, erase), &c, & numElemsErased1 CALL
	FUN(&rbTree, getRootNode), & rootPtr CALL
	ASSERT(*(int*)(rootPtr->data) == 10)
	ASSERT(*(int*)(rootPtr->right->data) == 13)

	FUN(&rbTree, erase), &a, & numElemsErased2 CALL
	FUN(&rbTree, getRootNode), & rootPtr CALL
	ASSERT(*(int*)(rootPtr->data) == 10)
	ASSERT(*(int*)(rootPtr->left->data) == 6)

	FUN(&rbTree, erase), &d, & numElemsErased3 CALL
	FUN(&rbTree, getRootNode), & rootPtr CALL
	ASSERT(*(int*)(rootPtr->data) == 8)
	ASSERT(*(int*)(rootPtr->left->data) == 3)
	ASSERT(*(int*)(rootPtr->right->data) == 13)
	FUN(&rbTree, size), &treeSize CALL

	FUN(&rbTree, erase), &l, & numElemsErased4 CALL
	FUN(&rbTree, getRootNode), & rootPtr CALL
	ASSERT(*(int*)(rootPtr->data) == 8)
	FUN(&rbTree, size), & treeSize1 CALL

	FUN(&rbTree, getRootNode), & rootPtr CALL
	FUN(&rbTree, inOrderTraversal), rootPtr CALL

	CREATE(redBlackTreeIterator, rbIt), retPtr);

	FUN(&rbTree, find), &c, & rbIt CALL
	FUN(&rbIt, getContentsOf), & retNode1 CALL

	FUN(&rbTree, find), &a, & rbIt CALL
	FUN(&rbIt, getContentsOf), & retNode2 CALL

	FUN(&rbTree, find), &d, & rbIt CALL
	FUN(&rbIt, getContentsOf), & retNode3 CALL

	FUN(&rbTree, find), &l, & rbIt CALL
	FUN(&rbIt, getContentsOf), & retNode4 CALL

	FUN(&rbTree, find), &k, & rbIt CALL
	FUN(&rbIt, getContentsOf), & retNode5 CALL

	//Assert
	//check that the correct number of deleted elements is returned
	ASSERT(numElemsErased1 == 1)
	ASSERT(numElemsErased2 == 1)
	ASSERT(numElemsErased3 == 1)
	ASSERT(numElemsErased4 == 0)

	//size is correctly maintained
	ASSERT(treeSize == 8)
	ASSERT(treeSize1 == 8)

    //erased data is no longer in the tree
	ASSERT(retNode1->data == NULL)
	ASSERT(retNode2->data == NULL)
	ASSERT(retNode3->data == NULL)
	ASSERT(retNode4->data == NULL)
	ASSERT(*(int*)(retNode5->data) == 13)
		

}END_FUN

//tests iteration with ints
FUN_IMPL(rbTree_iteration_SanityTest)
{
	CREATE(MemoryManager, memManager), sizeof(node) * 100, HEAP_BASED_MEMORY);

	void (*functionPtr)(void*, void*, bool*);
	functionPtr = &intComparison;

	CREATE(redBlackTree, rbTree), functionPtr);

	bool retBool;
	node* retPtr, *beginNode, *endNode;

	int a = 7, b = 3, c = 18, d = 10, e = 22, f = 8, g = 11, h = 26,
		i = 2, j = 6, k = 13;

	FUN(&rbTree, insert), &a, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &b, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &c, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &d, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &e, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &f, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &g, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &h, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &i, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &j, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &k, & retPtr, & retBool CALL

	CREATE(redBlackTreeIterator, beginIt), retPtr);
	FUN(&rbTree, begin), &beginIt CALL
	FUN(&beginIt, getContentsOf), & beginNode CALL

	CREATE(redBlackTreeIterator, endIt), retPtr);
	FUN(&rbTree, end), & endIt CALL
	FUN(&endIt, getContentsOf), & endNode CALL

	//should be 2, 3, 6, 7, 8, 10, 11, 13, 18, 22, 26
	bool isAtEnd = false;
	while (!isAtEnd)
	{
		SCOPE_START;
		node* intermediateNode = NULL;
		int intermediateVal = 0;
		FUN(&beginIt, getContentsOf), & intermediateNode CALL
		intermediateVal = *(int*)(intermediateNode->data);
		printf("%d ", intermediateVal);
	
		FUN(&beginIt, increment) CALL
		FUN(&beginIt, equals), endIt, & isAtEnd CALL;
		END_SCOPE;
	}

	//check begin() and end()
	ASSERT(*(int*)(beginNode->data) == 2)
	//data of head ptr
	ASSERT(endNode->data == NULL)

}END_FUN

//tests insert with coop class rectangle
FUN_IMPL(rbTree_rectangle_as_node_data_insert_SanityTest)
{
	CREATE(MemoryManager, memManager), sizeof(node) * 100, HEAP_BASED_MEMORY);

	void (*functionPtr)(void*, void*, bool*);
	functionPtr = &rectComp;

	bool retBool1 = false, retBool2= false, retBool3 = false, retBool4 = false,
		retBool5 = false, retBool6 = false, retBool7 = false, retBool8 = false;
	node* retPtr1, * retPtr2, * retPtr3, * retPtr4, * retPtr5, * retPtr6, 
		* retPtr7, * retPtr8, * rootPtr1, *rootPtr2, *rootPtr3, * rootPtr4,
		* rootPtr5, * rootPtr6, * rootPtr7;
	CREATE(redBlackTree, rbTree), functionPtr);

	CREATE(Rectangle, rect1), 1, 1);
	CREATE(Rectangle, rect2), 2, 2);
	CREATE(Rectangle, rect3), 3, 3);
	CREATE(Rectangle, rect4), 4, 4);
	CREATE(Rectangle, rect5), 5, 5);
	CREATE(Rectangle, rect6), 6, 6);
	CREATE(Rectangle, rect7), 7, 7);

	//test insert operation
	FUN(&rbTree, insert), & rect1, & retPtr1, & retBool1 CALL

	FUN(&rbTree, getRootNode), & rootPtr1 CALL
	ASSERT((*(Rectangle*)(rootPtr1->data)).width == 1);
	ASSERT(retBool1 == true);
	ASSERT((*(Rectangle*)(retPtr1->data)).width == 1);

	FUN(&rbTree, insert), & rect2, & retPtr2, & retBool2 CALL

	FUN(&rbTree, getRootNode), & rootPtr2 CALL
	ASSERT((*(Rectangle*)(rootPtr2->data)).width == 1);
	ASSERT((*(Rectangle*)(rootPtr2->right->data)).width == 2);
	ASSERT(retBool2 == true);
	ASSERT((*(Rectangle*)(retPtr2->data)).width == 2);

	FUN(&rbTree, insert), & rect3, & retPtr3, & retBool3 CALL
	FUN(&rbTree, getRootNode), & rootPtr3 CALL
	ASSERT((*(Rectangle*)(rootPtr3->data)).width == 2);
	ASSERT((*(Rectangle*)(rootPtr3->right->data)).width == 3);
	ASSERT((*(Rectangle*)(rootPtr3->left->data)).width == 1);
	ASSERT(retBool3 == true);
	ASSERT((*(Rectangle*)(retPtr3->data)).width == 3);

	FUN(&rbTree, insert), & rect4, & retPtr4, & retBool4 CALL
	FUN(&rbTree, getRootNode), & rootPtr4 CALL
	ASSERT((*(Rectangle*)(rootPtr4->data)).width == 2);
	ASSERT((*(Rectangle*)(rootPtr4->right->right->data)).width == 4);
	ASSERT((*(Rectangle*)(rootPtr4->left->data)).width == 1);
	ASSERT(retBool4 == true);
	ASSERT((*(Rectangle*)(retPtr4->data)).width == 4);

	FUN(&rbTree, insert), & rect5, & retPtr5, & retBool5 CALL
	FUN(&rbTree, getRootNode), & rootPtr5 CALL
	ASSERT((*(Rectangle*)(rootPtr5->data)).width == 2);
	ASSERT((*(Rectangle*)(rootPtr5->right->data)).width == 4);
	ASSERT((*(Rectangle*)(rootPtr5->right->right->data)).width == 5);
	ASSERT((*(Rectangle*)(rootPtr5->right->left->data)).width == 3);
	ASSERT((*(Rectangle*)(rootPtr5->left->data)).width == 1);
	ASSERT(retBool5 == true);
	ASSERT((*(Rectangle*)(retPtr5->data)).width == 5);

	FUN(&rbTree, insert), & rect6, & retPtr6, & retBool6 CALL
	FUN(&rbTree, getRootNode), & rootPtr6 CALL

	ASSERT((*(Rectangle*)(rootPtr6->data)).width == 2);
	ASSERT(retBool6 == true);
	ASSERT((*(Rectangle*)(retPtr6->data)).width == 6);

	FUN(&rbTree, insert), & rect7, & retPtr7, & retBool7 CALL
	FUN(&rbTree, getRootNode), & rootPtr7 CALL
	ASSERT((*(Rectangle*)(rootPtr7->data)).width == 2);
	ASSERT((*(Rectangle*)(rootPtr7->left->data)).width == 1);
	ASSERT((*(Rectangle*)(rootPtr7->right->data)).width == 4);
	ASSERT((*(Rectangle*)(rootPtr7->right->right->data)).width == 6);
	ASSERT((*(Rectangle*)(rootPtr7->right->left->data)).width == 3);
	ASSERT((*(Rectangle*)(rootPtr7->right->right->right->data)).width == 7);
	ASSERT(retBool7 == true);
	ASSERT((*(Rectangle*)(retPtr7->data)).width == 7);

	FUN(&rbTree, insert), & rect3, & retPtr8, & retBool8 CALL
	ASSERT(retBool8 == false);
	ASSERT((*(Rectangle*)(retPtr8->data)).width == 3);
	ASSERT(retPtr8 == retPtr3);
}END_FUN

//tests find with coop class rectangle
FUN_IMPL(rbTree_rectangle_as_node_data_find_SanityTest)
{
	CREATE(MemoryManager, memManager), sizeof(node) * 100, HEAP_BASED_MEMORY);

	void (*functionPtr)(void*, void*, bool*);
	functionPtr = &rectComp;

	bool retBool1 = false;
	node* retPtr1, * retNode1, * retNode2, * retNode3, *retNode4;
	CREATE(redBlackTree, rbTree), functionPtr);

	CREATE(Rectangle, rect1), 1, 1);
	CREATE(Rectangle, rect2), 2, 2);
	CREATE(Rectangle, rect3), 3, 3);
	CREATE(Rectangle, rect4), 4, 4);

	FUN(&rbTree, insert), & rect1, & retPtr1, & retBool1 CALL
	FUN(&rbTree, insert), & rect2, & retPtr1, & retBool1 CALL
	FUN(&rbTree, insert), & rect3, & retPtr1, & retBool1 CALL

	CREATE(redBlackTreeIterator, rbIt), retPtr1);

	FUN(&rbTree, find), & rect1, & rbIt CALL
	FUN(&rbIt, getContentsOf), & retNode1 CALL

	FUN(&rbTree, find), & rect2, & rbIt CALL
	FUN(&rbIt, getContentsOf), & retNode2 CALL

	FUN(&rbTree, find), & rect3, & rbIt CALL
	FUN(&rbIt, getContentsOf), & retNode3 CALL

	FUN(&rbTree, find), & rect4, & rbIt CALL
	FUN(&rbIt, getContentsOf), & retNode4 CALL

	ASSERT((*(Rectangle*)(retNode1->data)).width == 1)
	ASSERT((Rectangle*)(retNode1->data) = &rect1);

	ASSERT((*(Rectangle*)(retNode2->data)).width == 2)
	ASSERT((Rectangle*)(retNode2->data) = &rect2);

	ASSERT((*(Rectangle*)(retNode3->data)).width == 3)
	ASSERT((Rectangle*)(retNode3->data) = &rect3);
	
	ASSERT(retNode4->data == NULL) //value of header node (end())
}END_FUN

//tests delete with coop class rectangle
FUN_IMPL(rbTree_rectangle_as_node_data_delete_SanityTest)
{
		CREATE(MemoryManager, memManager), sizeof(node) * 100, HEAP_BASED_MEMORY);

		void (*functionPtr)(void*, void*, bool*);
		functionPtr = &rectComp;

		bool retBool1 = false;
		node* retPtr1, * retNode, *rootPtr1, *rootPtr2, *rootPtr3;
		int numElemsErased1, numElemsErased2, numElemsErased3;

		CREATE(redBlackTree, rbTree), functionPtr);

		CREATE(Rectangle, rect1), 1, 1);
		CREATE(Rectangle, rect2), 2, 2);
		CREATE(Rectangle, rect3), 3, 3);
		CREATE(Rectangle, rect4), 4, 4);
		CREATE(Rectangle, rect5), 5, 5);
		CREATE(Rectangle, rect6), 6, 6);
		CREATE(Rectangle, rect7), 7, 7);

		FUN(&rbTree, insert), & rect1, & retPtr1, & retBool1 CALL
		FUN(&rbTree, insert), & rect2, & retPtr1, & retBool1 CALL
		FUN(&rbTree, insert), & rect3, & retPtr1, & retBool1 CALL
		FUN(&rbTree, insert), & rect4, & retPtr1, & retBool1 CALL
		FUN(&rbTree, insert), & rect5, & retPtr1, & retBool1 CALL
		FUN(&rbTree, insert), & rect6, & retPtr1, & retBool1 CALL

		FUN(&rbTree, erase), &rect2, & numElemsErased1 CALL
		FUN(&rbTree, getRootNode), & rootPtr1 CALL
		ASSERT((*(Rectangle*)(rootPtr1->data)).width == 4);
		ASSERT((*(Rectangle*)(rootPtr1->left->data)).width == 1);
		ASSERT((*(Rectangle*)(rootPtr1->right->data)).width == 5);
		ASSERT(numElemsErased1 == 1);

		FUN(&rbTree, erase), & rect5, & numElemsErased2 CALL
		FUN(&rbTree, getRootNode), & rootPtr2 CALL
		ASSERT((*(Rectangle*)(rootPtr2->data)).width == 4);
		ASSERT((*(Rectangle*)(rootPtr1->left->data)).width == 1);
		ASSERT((*(Rectangle*)(rootPtr1->right->data)).width == 6);
		ASSERT(numElemsErased2 == 1);

		FUN(&rbTree, erase), & rect7, & numElemsErased3 CALL
		FUN(&rbTree, getRootNode), & rootPtr3 CALL
		ASSERT((*(Rectangle*)(rootPtr3->data)).width == 4);
		ASSERT(numElemsErased3 == 0);

		CREATE(redBlackTreeIterator, rbIt), retPtr1);

		FUN(&rbTree, find), & rect5, & rbIt CALL
		FUN(&rbIt, getContentsOf), & retNode CALL

		ASSERT(retNode->data == NULL) //value of header node (end())
}END_FUN