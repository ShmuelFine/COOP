#include "rbTreeIterUnitTest.h"
#include "redBlackTreeIterator.h"
#include "redBlackTree.h"

//int comparison function
FUN_IMPL(intComp, void* a, void* b, bool* retBool)
{
	int dd = *(int*)a;
	int ss = *(int*)b;
	*retBool = *(int*)a < *(int*)b;
}END_FUN;

FUN_IMPL(redBlacktreeIterator_increment_Sanity)
{
	CREATE(MemoryManager, memManager), sizeof(int) * 100, HEAP_BASED_MEMORY);

	void (*functionPtr)(void*, void*, bool*);
	functionPtr = &intComp;

	CREATE(redBlackTree, rbTree), functionPtr);

	node *retPtr, *retPtr1, *retPtr2, *retPtr3, *retPtr4, *retPtr5, *retPtr6,
		  *retPtr7;
	bool retBool;
	int a = 2, b = 10, c = 4, d = 35;

	FUN(&rbTree, insert), &a, & retPtr, & retBool CALL
	FUN(&rbTree, insert), &b, & retPtr1, & retBool CALL
	FUN(&rbTree, insert), &c, & retPtr6, & retBool CALL
	FUN(&rbTree, insert), &d, & retPtr2, & retBool CALL

	CREATE(redBlackTreeIterator, rbIt), retPtr);

	CREATE(redBlackTreeIterator, rbIt2), retPtr1);

	FUN(&rbIt, increment) CALL
	FUN(&rbIt, getContentsOf), & retPtr3 CALL
	ASSERT(*(int*)(retPtr3->data) == 4);

	FUN(&rbIt, increment) CALL
	FUN(&rbIt, getContentsOf), & retPtr4 CALL
	ASSERT(*(int*)(retPtr4->data) == 10);


	//test equals
	FUN(&rbIt, equals), rbIt2, & retBool CALL
	ASSERT(retBool == true);

	FUN(&rbIt, increment) CALL
	FUN(&rbIt, getContentsOf), & retPtr5 CALL
	ASSERT(*(int*)(retPtr5->data) == 35);

	//reaches end()
	FUN(&rbIt, increment) CALL
	FUN(&rbIt, getContentsOf), & retPtr6 CALL
	ASSERT(retPtr6->data == NULL);
	
}END_FUN

FUN_IMPL(redBlacktreeIterator_decrement_Sanity)
{
	CREATE(MemoryManager, memManager), sizeof(int) * 100, HEAP_BASED_MEMORY);

	void (*functionPtr)(void*, void*, bool*);
	functionPtr = &intComp;

	CREATE(redBlackTree, rbTree), functionPtr);

	node* retPtr, * retPtr1, * retPtr2, * retPtr3, * retPtr4, * retPtr5, * retPtr6,
		* retPtr7;
	bool retBool;

	int a = 2, b = 10, c = 4, d = 35;

	FUN(&rbTree, insert), & a, & retPtr, & retBool CALL
		FUN(&rbTree, insert), & b, & retPtr1, & retBool CALL
		FUN(&rbTree, insert), & c, & retPtr6, & retBool CALL
		FUN(&rbTree, insert), & d, & retPtr2, & retBool CALL

	CREATE(redBlackTreeIterator, rbIt), retPtr2);

	FUN(&rbTree, end), &rbIt CALL
	FUN(&rbIt, decrement) CALL
	FUN(&rbIt, getContentsOf), & retPtr7 CALL
	ASSERT(*(int*)(retPtr7->data) == 35);

	FUN(&rbIt, decrement) CALL
	FUN(&rbIt, getContentsOf), & retPtr3 CALL
	ASSERT(*(int*)(retPtr3->data) == 10);

	FUN(&rbIt, decrement) CALL
	FUN(&rbIt, getContentsOf), & retPtr4 CALL
	ASSERT(*(int*)(retPtr4->data) == 4);

    FUN(&rbIt, decrement) CALL
	FUN(&rbIt, getContentsOf), & retPtr5 CALL
	ASSERT(*(int*)(retPtr5->data) == 2);
}END_FUN