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
	FOR (int i = 0; i < numElements; i++)
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
	FOR (int i = 0; i < numElements; i++)
	{
		NTEST_ASSERT(data[i] == i);
	}END_LOOP;

}END_FUN

TEST_FUN_IMPL(VectorTest, pop_back_SanityTest)
{
	// Arrange
	CREATE(Vector_int, vec) CALL;
	int numElements = 54;
	FOR (int i = 0; i < numElements; i++)
	{
		MFUN(&vec, push_back), i CALL;
	}END_LOOP;

	// Act, Assert
	FOR (int i = 0; i < numElements; i++)
	{
		int val = 0;
		MFUN(&vec, pop_back), &val CALL;
		NTEST_ASSERT(val == (numElements - 1) - i);
	}END_LOOP;

}END_FUN

TEST_FUN_IMPL(VectorTest, at_ThrowsWhenIdxIsOutOfRange)
{
	CREATE(Vector_int, v1) CALL;
	
	int * retValPtr = NULL;

	EXPECT_THROW;
	MFUN(&v1, at), 6, & retValPtr CALL;
	ASSERT_THROW;

}END_FUN

TEST_FUN_IMPL(VectorTest, set_SanityTest)
{
	// Arrange
	CREATE(Vector_int, vec) CALL;
	int numElements = 54;

	FOR (int i = 0; i < numElements; i++)
	{
		MFUN(&vec, push_back), i CALL;
	}END_LOOP;
	//MFUN(&v1, print) CALL;
	
	// Act
	FOR (int i = 0; i < numElements; i++)
	{
		MFUN(&vec, set), i, (numElements - 1) - i CALL;
	}END_LOOP;
	// Assert
	int* data = NULL;
	MFUN(&vec, dataPtr), & data CALL;
	THROW_MSG_UNLESS(data, "Data can't be null");
	FOR (int i = 0; i < numElements; i++)
	{
		NTEST_ASSERT(data[i] == (numElements - 1) - i);
	}END_LOOP;

}END_FUN

TEST_FUN_IMPL(VectorTest, get_SanityTest)
{
	// Arrange
	CREATE(Vector_int, vec) CALL;
	int numElements = 54;

	FOR (int i = 0; i < numElements; i++)
	{
		MFUN(&vec, push_back), i CALL;
	}END_LOOP;
	//MFUN(&v1, print) CALL;

	// Act, Assert
	FOR (int i = 0; i < numElements; i++)
	{
		int val = 0;
		MFUN(&vec, get), i, &val CALL;
		NTEST_ASSERT(val == i);
	}END_LOOP;

}END_FUN

TEST_FUN_IMPL(VectorTest, dtor_freesAllMemory)
{
	FUN(init_global_memory) sizeof(int) * 1000, LIMITED_SIZE_MEMORY CALL;

	MEM_SIZE_T free_bytes_at_start = 0, free_bytes_at_end = 0;
	FUN(get_total_free_bytes) &free_bytes_at_start CALL;

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


INIT_TEST_SUITE(VectorTest)
BIND_TEST(VectorTest, push_back_SanityTest);
BIND_TEST(VectorTest, pop_back_SanityTest);
BIND_TEST(VectorTest, at_ThrowsWhenIdxIsOutOfRange);
BIND_TEST(VectorTest, set_SanityTest);
BIND_TEST(VectorTest, get_SanityTest);
BIND_TEST(VectorTest, dtor_freesAllMemory);
END_INIT_TEST_SUITE(VectorTest)
