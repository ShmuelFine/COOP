#include "shared_ptr_test.h"
#include "BaseClassExample.h"
#include "SharedPodPtr.h"

TEST_FUN_IMPL(SharedPtrTest, ctor_sanity)
{
	// Arrange, Act
	CREATE(SharedPodPtr, int_ptr), sizeof(int) CALL;
	*GET(int_ptr, int) = 1;

	// Assert
	NTEST_ASSERT(*GET(int_ptr, char) == 1);

}END_FUN

TEST_FUN_IMPL(SharedPtrTest, copyFrom_sanity)
{
	// Arrange
	CREATE(SharedPodPtr, int_ptr), sizeof(int) CALL;
	*GET(int_ptr, int) = 1;

	CREATE(SharedPodPtr, other_ptr), sizeof(int) CALL;
	*GET(other_ptr, int) = 2;

	// Act 
	MFUN(&int_ptr, copyFrom), & other_ptr CALL;

	// Assert
	NTEST_ASSERT(*GET(other_ptr, char) == 2);
	NTEST_ASSERT(*int_ptr.pn == 2);
	NTEST_ASSERT(*other_ptr.pn == 2);
}END_FUN

FUN_IMPL(do_something_with_shared_ptr)
{
	int numElements = 10;
	CREATE(SharedPodPtr, int_ptr), numElements * (MEM_SIZE_T)sizeof(int) CALL;
	for (int i = 0; i < numElements; i++)
	{
		*(GET(int_ptr, int) + i) = i;
	}
}
END_FUN;

TEST_FUN_IMPL(SharedPtrTest, release_sanity)
{
	// Arrange
	MEM_SIZE_T total_bytes_at_start = 0;
	FUN(get_total_free_bytes) &total_bytes_at_start CALL;
	// TODO: MFUN shoudl not require ),

	// Act
	FUN(do_something_with_shared_ptr) CALL;

	// Assert
	MEM_SIZE_T total_bytes_at_end = 0;
	FUN(get_total_free_bytes) &total_bytes_at_end  CALL;

	NTEST_ASSERT(total_bytes_at_end == total_bytes_at_start);

}END_FUN

TEST_FUN_IMPL(SharedPtrTest, isEmpty_sanity)
{
	// Arrange
	CREATE(SharedPodPtr, emptyPtr), 0 CALL;
	CREATE(SharedPodPtr, nonEmptyPtr), 1 CALL;

	// Act
	bool isEmptyPtrEmpty = false, isNonEmptyPtrEmpty = true;
	MFUN(&emptyPtr, isEmpty), &isEmptyPtrEmpty CALL;
	MFUN(&nonEmptyPtr, isEmpty), &isNonEmptyPtrEmpty CALL;

	// Assert
	NTEST_ASSERT(true == isEmptyPtrEmpty);
	NTEST_ASSERT(false == isNonEmptyPtrEmpty);

}END_FUN


INIT_TEST_SUITE(SharedPtrTest);
BIND_TEST(SharedPtrTest, ctor_sanity);
BIND_TEST(SharedPtrTest, copyFrom_sanity);
BIND_TEST(SharedPtrTest, release_sanity);
BIND_TEST(SharedPtrTest, isEmpty_sanity);
END_INIT_TEST_SUITE(SharedPtrTest);