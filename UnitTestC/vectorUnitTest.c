#include "vectorUnitTest.h"
#include "ExportDefs.h"
#include "vector.h"
#include "ScopeTester.h"

TEST_FUN_IMPL(VectorTest, push_back_SanityTest)
{
	// Arrange
	CREATE(Vector_int, v1) CALL;

	// Act
	int retVal0 = 0;
	int retVal1 = 0;
	int retVal2 = 0;
	int retVal3 = 0;
	int retVal4 = 0;

	MFUN(&v1, push_back), 3 CALL;
	MFUN(&v1, get), 0, & retVal0 CALL;

	MFUN(&v1, push_back), 4 CALL;
	MFUN(&v1, get), 1, & retVal1 CALL;

	MFUN(&v1, push_back), 5 CALL;
	MFUN(&v1, get), 2, & retVal2 CALL;

	MFUN(&v1, push_back), 6 CALL;
	MFUN(&v1, get), 3, & retVal3 CALL;

	MFUN(&v1, push_back), 7 CALL;
	MFUN(&v1, get), 4, & retVal4 CALL;
	//MFUN(&v1, print) CALL;

	// Assert
	NTEST_ASSERT(retVal0 == 3);
	NTEST_ASSERT(retVal1 == 4);
	NTEST_ASSERT(retVal2 == 5);
	NTEST_ASSERT(retVal3 == 6);
	NTEST_ASSERT(retVal4 == 7);

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
	CREATE(Vector_int, v1) CALL;

	MFUN(&v1, push_back), 3 CALL;
	MFUN(&v1, push_back), 4 CALL;
	MFUN(&v1, push_back), 5 CALL;
	MFUN(&v1, push_back), 6 CALL;
	//MFUN(&v1, print) CALL;

	// Act	
	MFUN(&v1, set), 0, 13 CALL;
	MFUN(&v1, set), 1, 14 CALL;
	MFUN(&v1, set), 2, 15 CALL;
	MFUN(&v1, set), 3, 16 CALL;

	// Assert
	int val0; MFUN(&v1, get), 0, &val0 CALL;
	int val1; MFUN(&v1, get), 1, &val1 CALL;
	int val2; MFUN(&v1, get), 2, &val2 CALL;
	int val3; MFUN(&v1, get), v1._base.size - 1, &val3 CALL;
	
	NTEST_ASSERT(val0 == 13);
	NTEST_ASSERT(val1 == 14);
	NTEST_ASSERT(val2 == 15);
	NTEST_ASSERT(val3 == 16);

}END_FUN

INIT_TEST_SUITE(VectorTest)
BIND_TEST(VectorTest, push_back_SanityTest);
BIND_TEST(VectorTest, at_ThrowsWhenIdxIsOutOfRange);
BIND_TEST(VectorTest, set_SanityTest);
END_INIT_TEST_SUITE(VectorTest)