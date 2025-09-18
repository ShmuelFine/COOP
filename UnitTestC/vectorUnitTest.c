#include "VectorUnitTest.h"
#include "ExportDefs.h"
#include "Vector.h"
#include "ScopeTester.h"



TEST_FUN_IMPL(VectorTest, push_back_SanityTest)
{
	// Arrange
	CREATE(Vector_int, vec) CALL;
	int numElements = 54;

	// Act
	FOR(int i = 0; i < numElements; i++)
	{
		MFUN(&vec, push_back), i CALL;
	}END_LOOP;

	// Assert
	MEM_SIZE_T curr_size = 0;
	MFUN(&vec, size), & curr_size CALL;
	NTEST_ASSERT(curr_size == numElements);

	int* data = NULL;
	MFUN(&vec, dataPtr), & data CALL;
	THROW_MSG_UNLESS(data, "Data can't be null");
	FOR(int i = 0; i < numElements; i++)
	{
		NTEST_ASSERT(data[i] == i);
	}END_LOOP;

}END_FUN

TEST_FUN_IMPL(VectorTest, pop_back_SanityTest)
{
	// Arrange
	CREATE(Vector_int, vec) CALL;
	int numElements = 54;
	FOR(int i = 0; i < numElements; i++)
	{
		MFUN(&vec, push_back), i CALL;
	}END_LOOP;

	// Act, Assert
	FOR(int i = 0; i < numElements; i++)
	{
		int val = 0;
		MFUN(&vec, pop_back), & val CALL;
		NTEST_ASSERT(val == (numElements - 1) - i);
	}END_LOOP;

}END_FUN

TEST_FUN_IMPL(VectorTest, at_ThrowsWhenIdxIsOutOfRange)
{
	CREATE(Vector_int, v1) CALL;

	int* retValPtr = NULL;

	EXPECT_THROW;
	MFUN(&v1, at), 6, & retValPtr CALL;
	ASSERT_THROW;

}END_FUN

TEST_FUN_IMPL(VectorTest, set_SanityTest)
{
	// Arrange
	CREATE(Vector_int, vec) CALL;
	int numElements = 54;

	FOR(int i = 0; i < numElements; i++)
	{
		MFUN(&vec, push_back), i CALL;
	}END_LOOP;
	//MFUN(&v1, print) CALL;

	// Act
	FOR(int i = 0; i < numElements; i++)
	{
		MFUN(&vec, set), i, (numElements - 1) - i CALL;
	}END_LOOP;
	// Assert
	int* data = NULL;
	MFUN(&vec, dataPtr), & data CALL;
	THROW_MSG_UNLESS(data, "Data can't be null");
	FOR(int i = 0; i < numElements; i++)
	{
		NTEST_ASSERT(data[i] == (numElements - 1) - i);
	}END_LOOP;

}END_FUN

TEST_FUN_IMPL(VectorTest, get_SanityTest)
{
	// Arrange
	CREATE(Vector_int, vec) CALL;
	int numElements = 54;

	FOR(int i = 0; i < numElements; i++)
	{
		MFUN(&vec, push_back), i CALL;
	}END_LOOP;
	//MFUN(&v1, print) CALL;

	// Act, Assert
	FOR(int i = 0; i < numElements; i++)
	{
		int val = 0;
		MFUN(&vec, get), i, & val CALL;
		NTEST_ASSERT(val == i);
	}END_LOOP;

}END_FUN

TEST_FUN_IMPL(VectorTest, dtor_freesAllMemory)
{
	FUN(init_global_memory) sizeof(int) * 1000, LIMITED_SIZE_MEMORY CALL;

	MEM_SIZE_T free_bytes_at_start = 0, free_bytes_at_end = 0;
	FUN(get_total_free_bytes)& free_bytes_at_start CALL;

	FOR(int k = 0; k < 10; k++) {
		// Arrange
		CREATE(Vector_int, vec) CALL;
		int numElements = 5;

		FOR(int i = 0; i < numElements; i++)
		{
			MFUN(&vec, push_back), i CALL;
		}END_LOOP;

		// Act - the dtor is called here:
	}END_LOOP;

	// Assert
	FUN(get_total_free_bytes)& free_bytes_at_end CALL;
	NTEST_ASSERT(free_bytes_at_end == free_bytes_at_start);


}END_FUN;

TEST_FUN_IMPL(VectorTest, nextPrev_MoveOK_andPrevThrowsAtBegin)
{
	CREATE(Vector_int, vec) CALL;
	/* Helpers */
	
    FOR(int i = 0; i < 5; i++) {
	 MFUN(&vec, push_back), i CALL;
	} END_LOOP;
	

	Iterator *it = NULL;
	Iterator *end = NULL;
	MFUN(&vec, begin), &it CALL;
	MFUN(&vec, end), &end CALL;

	/* prev at begin must throw */
	EXPECT_THROW;
	MFUN(it, prev) CALL;
	ASSERT_THROW;

	/* move 2 steps with next; then check we’re not at end yet */
	MFUN(it, next) CALL;
	MFUN(it, next) CALL;

	bool eq = false;
	MFUN(it, equals), (Iterator*)end, & eq CALL;
	NTEST_ASSERT(!eq);

	/* one more next to index 3, still not end (size=5) */
	MFUN(it, next) CALL;
	MFUN(it, equals), (Iterator*)end, & eq CALL;
	NTEST_ASSERT(!eq);

	DESTROY(it);
	DESTROY(end);
} END_FUN

TEST_FUN_IMPL(VectorTest, getRef_getCref_PointsToCurrent)
{
	CREATE(Vector_int, vec) CALL;
	FOR(int i = 0; i < 5; i++) {
		MFUN(&vec, push_back), i CALL;
	} END_LOOP;

	Iterator *it = NULL;
	MFUN(&vec, begin), &it CALL;

	/* at begin -> value 0 */
	const void* cptr = NULL;
	MFUN(it, get_cref), & cptr CALL;
	NTEST_ASSERT(cptr != NULL);
	NTEST_ASSERT(*(const int*)cptr == 0);

	/* advance to index 2, expect value 2 */
	MFUN(it, advance), (ptrdiff_t)2 CALL;
	void* ptr = NULL;
	MFUN(it, get_ref), & ptr CALL;
	NTEST_ASSERT(ptr != NULL);
	NTEST_ASSERT(*(int*)ptr == 2);

	DESTROY(it);
} END_FUN

TEST_FUN_IMPL(VectorTest, distance_And_Advance_Bounds)
{
	CREATE(Vector_int, vec) CALL;
	FOR(int i = 0; i < 6; i++) {
		MFUN(&vec, push_back), i CALL;
	} END_LOOP;

	Iterator *b = NULL;
	Iterator *e = NULL;
	MFUN(&vec, begin), &b CALL;
	MFUN(&vec, end), &e CALL;

	/* distance(begin, end) == size */
	ptrdiff_t dist = -999;
	MFUN(b, distance), (Iterator*)e, & dist CALL;
	NTEST_ASSERT(dist == 6);

	/* distance after advancing begin by 3 -> should be 3 */
	MFUN(b, advance), (ptrdiff_t)3 CALL;
	dist = -999;
	MFUN(b, distance), (Iterator*)e, & dist CALL;
	NTEST_ASSERT(dist == 3);

	/* advance negative out of range -> throw */
	EXPECT_THROW;
	MFUN(b, advance), (ptrdiff_t)-5 CALL; /* would go to index -2 */
	ASSERT_THROW;

	/* advance past end -> throw (target > size) */
	EXPECT_THROW;
	MFUN(e, advance), (ptrdiff_t)1 CALL;  /* target = size+1 -> invalid */
	ASSERT_THROW;

	DESTROY(b);
	DESTROY(e);
} END_FUN

INIT_TEST_SUITE(VectorTest)
BIND_TEST(VectorTest, push_back_SanityTest);
BIND_TEST(VectorTest, pop_back_SanityTest);
BIND_TEST(VectorTest, at_ThrowsWhenIdxIsOutOfRange);
BIND_TEST(VectorTest, set_SanityTest);
BIND_TEST(VectorTest, get_SanityTest);
BIND_TEST(VectorTest, dtor_freesAllMemory);
BIND_TEST(VectorTest, nextPrev_MoveOK_andPrevThrowsAtBegin);
BIND_TEST(VectorTest, getRef_getCref_PointsToCurrent);
BIND_TEST(VectorTest, distance_And_Advance_Bounds);
END_INIT_TEST_SUITE(VectorTest)