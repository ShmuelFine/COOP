#include "rbTreeIterUnitTest.h"
#include "redBlackTreeIterator.h"
#include "redBlackTree.h"

FUN_IMPL(redBlacktreeIterator_increment_Sanity)
{
	CREATE(MemoryManager, memManager), sizeof(int) * 100, HEAP_BASED_MEMORY);

	CREATE(redBlackTree, rbTree));

	node *retPtr, *retPtr1, *retPtr2, *retPtr3, *retPtr4, *retPtr5, *retPtr6,
		  *retPtr7;
	bool retBool;

	FUN(&rbTree, insert), 2, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 10, & retPtr1, & retBool CALL
	FUN(&rbTree, insert), 4, & retPtr6, & retBool CALL
	FUN(&rbTree, insert), 35, & retPtr2, & retBool CALL

	CREATE(redBlackTreeIterator, rbIt), retPtr);

	CREATE(redBlackTreeIterator, rbIt2), retPtr1);

	FUN(&rbIt, increment) CALL
	FUN(&rbIt, getContentsOf), & retPtr3 CALL
	ASSERT(retPtr3->data == 4);

	FUN(&rbIt, increment) CALL
	FUN(&rbIt, getContentsOf), & retPtr4 CALL
	ASSERT(retPtr4->data == 10);

	//test equals
	FUN(&rbIt, equals), rbIt2, & retBool CALL
	ASSERT(retBool == true);

	FUN(&rbIt, increment) CALL
	FUN(&rbIt, getContentsOf), & retPtr5 CALL
	ASSERT(retPtr5->data == 35);

	//reaches end()
	FUN(&rbIt, increment) CALL
	FUN(&rbIt, getContentsOf), & retPtr6 CALL
	ASSERT(retPtr6->data == -10);
	
}END_FUN

FUN_IMPL(redBlacktreeIterator_decrement_Sanity)
{
	CREATE(MemoryManager, memManager), sizeof(int) * 100, HEAP_BASED_MEMORY);

	CREATE(redBlackTree, rbTree));

	node* retPtr, * retPtr1, * retPtr2, * retPtr3, * retPtr4, * retPtr5, * retPtr6,
		* retPtr7;
	bool retBool;

	FUN(&rbTree, insert), 2, & retPtr, & retBool CALL
	FUN(&rbTree, insert), 10, & retPtr1, & retBool CALL
	FUN(&rbTree, insert), 4, & retPtr6, & retBool CALL
	FUN(&rbTree, insert), 35, & retPtr2, & retBool CALL

	CREATE(redBlackTreeIterator, rbIt), retPtr2);

	FUN(&rbTree, end), &rbIt CALL
	FUN(&rbIt, decrement) CALL
	FUN(&rbIt, getContentsOf), & retPtr7 CALL
	ASSERT(retPtr7->data == 35);

	FUN(&rbIt, decrement) CALL
	FUN(&rbIt, getContentsOf), & retPtr3 CALL
	ASSERT(retPtr3->data == 10);

	FUN(&rbIt, decrement) CALL
	FUN(&rbIt, getContentsOf), & retPtr4 CALL
	ASSERT(retPtr4->data == 4);

    FUN(&rbIt, decrement) CALL
	FUN(&rbIt, getContentsOf), & retPtr5 CALL
	ASSERT(retPtr5->data == 2);
}END_FUN