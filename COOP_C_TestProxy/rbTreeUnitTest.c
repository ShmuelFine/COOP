#include "rbTreeUnitTest.h"
#include "redBlackTree.h"

FUN_IMPL(rbTree_insert_SanityTest)
{
	//also checked with an inorder traversal after each insert
	//Arrange
	CREATE(MemoryManager, memManager), sizeof(node) * 100, HEAP_BASED_MEMORY);

	CREATE(redBlackTree, rbTree));

	node * retPtr = NULL, * retPtr1 = NULL, * retPtr2 = NULL, * retPtr3 = NULL, 
		* retPtr4 = NULL, * retPtr5 = NULL, * retPtr6 = NULL, * retPtr7 = NULL,
		* retPtr8 = NULL, * retPtr9 = NULL, * rootPtr = NULL, * rootPtr1 = NULL, 
		* rootPtr2 = NULL, * rootPtr3 = NULL;
	bool retBool = false, retBool1 = false, retBool2 = false, retBool3 = false,
		 retBool4 = false, retBool5 = false, retBool6 = false, retBool7 = false,
		 retBool8 = true, retBool9 = true;

	int treeSize;
	
	//Act
	FUN(&rbTree, insert), 10, &retPtr, &retBool CALL
	FUN(&rbTree, insert), 20, & retPtr1, & retBool1 CALL
	FUN(&rbTree, insert), 40, & retPtr2, & retBool2 CALL
	FUN(&rbTree, getRootNode), & rootPtr1 CALL
	FUN(&rbTree, insert), 30, & retPtr3, & retBool3 CALL
    FUN(&rbTree, insert), 50, & retPtr4, & retBool4 CALL
	FUN(&rbTree, insert), 25, & retPtr6, & retBool6 CALL
	FUN(&rbTree, insert), 35, & retPtr5, & retBool5 CALL
	FUN(&rbTree, getRootNode), & rootPtr2 CALL
	FUN(&rbTree, insert), 37, & retPtr7, & retBool7 CALL
	FUN(&rbTree, getRootNode), & rootPtr3 CALL
	FUN(&rbTree, insert), 10, & retPtr8, & retBool8 CALL
	FUN(&rbTree, insert), 37, & retPtr9, & retBool9 CALL

	
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

	ASSERT(retPtr->data == 10);
	ASSERT(retPtr1->data == 20);
	ASSERT(retPtr2->data == 40);
	ASSERT(retPtr3->data == 30);
	ASSERT(retPtr4->data == 50);
	ASSERT(retPtr5->data == 35);
	ASSERT(retPtr6->data == 25);
	ASSERT(retPtr7->data == 37);
	ASSERT(retPtr8->data == 10);
	ASSERT(retPtr9->data == 37);

	//check that rebalancing happens and doesn't happen at the right places
	ASSERT(rootPtr1->data == 20)
	ASSERT(rootPtr2->data == 20)
	ASSERT(rootPtr3->data == 30)

	ASSERT(treeSize == 8)

}END_FUN

FUN_IMPL(rbTree_find_SanityTest)
{
	CREATE(MemoryManager, memManager), sizeof(node) * 100, HEAP_BASED_MEMORY);

	CREATE(redBlackTree, rbTree));

	bool retBool;
	node* retPtr, *retNode1, *retNode2, *retNode3;

	FUN(&rbTree, insert), 10, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 20, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 40, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 30, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 50, & retPtr, & retBool CALL

	CREATE(redBlackTreeIterator, rbIt), retPtr);

	FUN(&rbTree, find), 20, & rbIt CALL
	FUN(&rbIt, getContentsOf), & retNode1 CALL

	FUN(&rbTree, find), 50, & rbIt CALL
	FUN(&rbIt, getContentsOf), & retNode2 CALL

	FUN(&rbTree, find), 65, & rbIt CALL
	FUN(&rbIt, getContentsOf), & retNode3 CALL

	ASSERT(retNode1->data == 20)
	ASSERT(retNode2->data == 50)
	ASSERT(retNode3->data == -10) //value of header node (end())

}END_FUN

FUN_IMPL(rbTree_delete_SanityTest)
{
	//also checked the inOrderTraversal after each erase 
	//ARRANGE
	CREATE(MemoryManager, memManager), sizeof(node) * 100, HEAP_BASED_MEMORY);

	CREATE(redBlackTree, rbTree));
	

	node* retPtr = NULL, * rootPtr2 = NULL, * rootPtr3 = NULL, *retNode1 = NULL,
		* retNode2 = NULL, * retNode3 = NULL, * retNode4 = NULL, * retNode5 = NULL,
		*rootPtr;
	bool retBool = false;

	int treeSize, treeSize1, numElemsErased1, numElemsErased2, numElemsErased3,
		numElemsErased4;

	//Act
	FUN(&rbTree, insert), 7, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 3, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 18, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 10, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 22, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 8, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 11, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 26, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 2, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 6, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 13, & retPtr, & retBool CALL

	
	FUN(&rbTree, erase), 18, & numElemsErased1 CALL
	FUN(&rbTree, erase), 7, & numElemsErased2 CALL
	FUN(&rbTree, erase), 10, & numElemsErased3 CALL
	FUN(&rbTree, size), &treeSize CALL
	FUN(&rbTree, erase), 35, & numElemsErased4 CALL
	FUN(&rbTree, size), & treeSize1 CALL

	CREATE(redBlackTreeIterator, rbIt), retPtr);

	FUN(&rbTree, find), 18, & rbIt CALL
	FUN(&rbIt, getContentsOf), & retNode1 CALL

	FUN(&rbTree, find), 7, & rbIt CALL
	FUN(&rbIt, getContentsOf), & retNode2 CALL

	FUN(&rbTree, find), 10, & rbIt CALL
	FUN(&rbIt, getContentsOf), & retNode3 CALL

	FUN(&rbTree, find), 35, & rbIt CALL
	FUN(&rbIt, getContentsOf), & retNode4 CALL

	FUN(&rbTree, find), 13, & rbIt CALL
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
	ASSERT(retNode1->data == -10)
	ASSERT(retNode2->data == -10)
	ASSERT(retNode3->data == -10)
	ASSERT(retNode4->data == -10)
	ASSERT(retNode5->data == 13)
		

}END_FUN

FUN_IMPL(rbTree_iteration_SanityTest)
{
	CREATE(MemoryManager, memManager), sizeof(node) * 100, HEAP_BASED_MEMORY);

    CREATE(redBlackTree, rbTree));

	bool retBool;
	node* retPtr, *beginNode, *endNode;

	FUN(&rbTree, insert), 7, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 3, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 18, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 10, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 22, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 8, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 11, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 26, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 2, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 6, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 13, & retPtr, & retBool CALL

	CREATE(redBlackTreeIterator, beginIt), retPtr);
	FUN(&rbTree, begin), &beginIt CALL
	FUN(&beginIt, getContentsOf), & beginNode CALL

	CREATE(redBlackTreeIterator, endIt), retPtr);
	FUN(&rbTree, end), & endIt CALL
	FUN(&endIt, getContentsOf), & endNode CALL

	bool isAtEnd = false;
	while (!isAtEnd)
	{
		SCOPE_START;
		node* intermediateNode = NULL;
		int intermediateVal = 0;
		FUN(&beginIt, getContentsOf), & intermediateNode CALL
		intermediateVal = intermediateNode->data;
		printf("%d ", intermediateVal);
	
		FUN(&beginIt, increment) CALL
		FUN(&beginIt, equals), endIt, & isAtEnd CALL;
		END_SCOPE;
	}

	//check begin() and end()
	ASSERT(beginNode->data == 2)
	ASSERT(endNode->data == -10)

}END_FUN