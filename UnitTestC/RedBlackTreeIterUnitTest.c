#include "RedBlackTreeIterUnitTest.h"

FUN_DECL(intComp, void* a, void* b, bool* retBool);

FUN_IMPL(intComp, void* a, void* b, bool* retBool)
{
	int dd = *(int*)a;
	int ss = *(int*)b;
	*retBool = *(int*)a < *(int*)b;
}END_FUN;

TEST_FUN_IMPL(RedBlackTreeIteratorTest, redBlacktreeIterator_increment_Sanity)
{
	init_global_memory(sizeof(int) * 100, HEAP_BASED_MEMORY);

	void (*functionPtr)(void*, void*, bool*);
	functionPtr = &intComp;

	CREATE(redBlackTree, rbTree), functionPtr CALL;

	node* retPtr, * retPtr1, * retPtr2, * retPtr3, * retPtr4, * retPtr5, * retPtr6;
	//* retPtr7;
	bool retBool;
	int a = 2, b = 10, c = 4, d = 35;

	MFUN(&rbTree, insert), & a, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & b, & retPtr1, & retBool CALL;
	MFUN(&rbTree, insert), & c, & retPtr6, & retBool CALL;
	MFUN(&rbTree, insert), & d, & retPtr2, & retBool CALL;

	CREATE(redBlackTreeIterator, rbIt), retPtr CALL;

	CREATE(redBlackTreeIterator, rbIt2), retPtr1 CALL;

	MFUN(&rbIt, increment) CALL;
	MFUN(&rbIt, getContentsOf), & retPtr3 CALL;
	NTEST_ASSERT(*(int*)(retPtr3->data) == 4);

	MFUN(&rbIt, increment) CALL;
	MFUN(&rbIt, getContentsOf), & retPtr4 CALL;
	NTEST_ASSERT(*(int*)(retPtr4->data) == 10);


	//test equals
	MFUN(&rbIt, equals), rbIt2, & retBool CALL;
	NTEST_ASSERT(retBool == true);

	MFUN(&rbIt, increment) CALL;
	MFUN(&rbIt, getContentsOf), & retPtr5 CALL;
	NTEST_ASSERT(*(int*)(retPtr5->data) == 35);

	//reaches end()
	MFUN(&rbIt, increment) CALL;
	MFUN(&rbIt, getContentsOf), & retPtr6 CALL;
	NTEST_ASSERT(retPtr6->data == NULL);

}END_FUN

TEST_FUN_IMPL(RedBlackTreeIteratorTest, redBlacktreeIterator_decrement_Sanity)
{
	init_global_memory(sizeof(int) * 100, HEAP_BASED_MEMORY);

	void (*functionPtr)(void*, void*, bool*);
	functionPtr = &intComp;

	CREATE(redBlackTree, rbTree), functionPtr CALL;

	node* retPtr, * retPtr1, * retPtr2, * retPtr3, * retPtr4, * retPtr5, * retPtr6,
		* retPtr7;
	bool retBool;

	int a = 2, b = 10, c = 4, d = 35;

	MFUN(&rbTree, insert), & a, & retPtr, & retBool CALL;
	MFUN(&rbTree, insert), & b, & retPtr1, & retBool CALL;
	MFUN(&rbTree, insert), & c, & retPtr6, & retBool CALL;
	MFUN(&rbTree, insert), & d, & retPtr2, & retBool CALL;

	CREATE(redBlackTreeIterator, rbIt), retPtr2 CALL;

	MFUN(&rbTree, end), & rbIt CALL;
	MFUN(&rbIt, decrement) CALL;
	MFUN(&rbIt, getContentsOf), & retPtr7 CALL;
	NTEST_ASSERT(*(int*)(retPtr7->data) == 35);

	MFUN(&rbIt, decrement) CALL;
	MFUN(&rbIt, getContentsOf), & retPtr3 CALL;
	NTEST_ASSERT(*(int*)(retPtr3->data) == 10);

	MFUN(&rbIt, decrement) CALL;
	MFUN(&rbIt, getContentsOf), & retPtr4 CALL;
	NTEST_ASSERT(*(int*)(retPtr4->data) == 4);

	MFUN(&rbIt, decrement) CALL;
	MFUN(&rbIt, getContentsOf), & retPtr5 CALL;
	NTEST_ASSERT(*(int*)(retPtr5->data) == 2);
}END_FUN

INIT_TEST_SUITE(RedBlackTreeIteratorTest);
BIND_TEST(RedBlackTreeIteratorTest, redBlacktreeIterator_decrement_Sanity);
BIND_TEST(RedBlackTreeIteratorTest, redBlacktreeIterator_increment_Sanity);
END_INIT_TEST_SUITE(RedBlackTreeIteratorTest);