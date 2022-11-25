#include "vectorUnitTest.h"
#include "../COOP/ExportDefs.h"
#include "../COOP/vector.h"
#include "ScopeTester.h"

TEST_FUN_IMPL(VectorTest, push_back_SanityTest)
{
	// Arrange
	CREATE(Vector_int, v1) CALL;

	// Act
	int retVal = 0;
	int retVal1 = 0;
	int retVal2 = 0;
	int retVal3 = 0;
	int retVal4 = 0;
	FUN(&v1, push_back), 3 CALL;
	FUN(&v1, print) CALL;
	FUN(&v1, at), 0, & retVal CALL;

	FUN(&v1, push_back), 4 CALL;
	FUN(&v1, print) CALL;
	FUN(&v1, at), 1, & retVal1 CALL;

	FUN(&v1, push_back), 5 CALL;
	FUN(&v1, print) CALL;
	FUN(&v1, at), 2, & retVal2 CALL;

	FUN(&v1, push_back), 6 CALL;
	FUN(&v1, print) CALL;
	FUN(&v1, at), 3, & retVal3 CALL;

	FUN(&v1, push_back), 7 CALL;
	FUN(&v1, print) CALL;
	FUN(&v1, at), 4, & retVal4 CALL;

	// Assert
	NTEST_ASSERT(retVal == 3);
	NTEST_ASSERT(retVal1 == 4);
	NTEST_ASSERT(retVal2 == 5);
	NTEST_ASSERT(retVal3 == 6);
	NTEST_ASSERT(retVal4 == 7);

}END_FUN

TEST_FUN_IMPL(VectorTest, at_throws_when_idx_is_outOfRange)
{
	CREATE(Vector_int, v1) CALL;
	
	int retVal = 0;

	EXPECT_THROW;
	FUN(&v1, at), 6, & retVal CALL;
	ASSERT_THROW;

}END_FUN

TEST_FUN_IMPL(VectorTest, VEC_AT__SanityTest)
{
	// Arrange
	CREATE(Vector_int, v1) CALL;

	FUN(&v1, push_back), 3 CALL;
	FUN(&v1, push_back), 4 CALL;
	FUN(&v1, push_back), 5 CALL;
	FUN(&v1, push_back), 6 CALL;

	// Act
	int val0; FUN(&v1, at), 0, &val0 CALL;
	int val1; FUN(&v1, at), 1, &val1 CALL;
	int val2; FUN(&v1, at), 2, &val2 CALL;
	int val3; FUN(&v1, at), v1._base.size - 1, &val3 CALL;
	
	// Assert
	NTEST_ASSERT(val0 == 3);
	NTEST_ASSERT(val1 == 4);
	NTEST_ASSERT(val2 == 5);
	NTEST_ASSERT(val3 == 6);

}END_FUN

INIT_TEST_SUITE(VectorTest)
BIND_TEST(VectorTest, push_back_SanityTest);
BIND_TEST(VectorTest, at_throws_when_idx_is_outOfRange);
BIND_TEST(VectorTest, VEC_AT__SanityTest);
END_INIT_TEST_SUITE(VectorTest)