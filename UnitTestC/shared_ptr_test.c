#include "shared_ptr_test.h"
#include "BaseClassExample.h"
#include "Shared_ptr.h"

TEST_FUN_IMPL(SharedPtrTest, check_type_char)
{
	// Arrange
	CREATE(Shared_ptr, ptr) CALL;
	CREATE(Shared_ptr, ptr2) CALL;

	char* i = NULL;
	ALLOC_VARIABLE(i, char);
	*i = 'a';

	char* j = NULL;
	ALLOC_VARIABLE(j, char);
	*j = 'b';

	MFUN(&ptr, Reset), i CALL;//a
	MFUN(&ptr2, Reset), j CALL;//b

	NTEST_ASSERT(*GET(ptr2, char) == 'b');

	// Act 
	MFUN(&ptr, CopyTo), & ptr2 CALL;
	MFUN(&ptr2, CopyFrom), & ptr CALL;

	// Assert
	NTEST_ASSERT(*GET(ptr2, char) == 'a');

	*i = 'c';
	NTEST_ASSERT(*GET(ptr, char) == 'c');
	NTEST_ASSERT(*GET(ptr2, char) == 'c');

	bool is_empty = true;
	MFUN(&ptr, IsEmpty), & is_empty CALL;
	NTEST_ASSERT(is_empty == false);

	MFUN(&ptr, Release) CALL;

}END_FUN


TEST_FUN_IMPL(SharedPtrTest, pointing_to_class_test)
{
	// Arrange
	CREATE(Shared_ptr, ptr) CALL;
	CREATE(Shared_ptr, ptr2) CALL;

	CREATE_PTR(BaseClassExample, B), 10, 3 CALL;
	CREATE_PTR(BaseClassExample, C), 3, 10 CALL;

	MFUN(&ptr, Reset), B CALL;
	MFUN(&ptr2, Reset), C CALL;

	NTEST_ASSERT(ptr.px == B);
	// Act
	MFUN(&ptr, CopyTo), & ptr2 CALL;

	MFUN(&ptr2, CopyFrom), & ptr CALL;


	bool is_empty = true;
	MFUN(&ptr, IsEmpty), & is_empty CALL;
	NTEST_ASSERT(is_empty == false);

	MFUN(&ptr, Release) CALL;


}
END_FUN

TEST_FUN_IMPL(SharedPtrTest, CopyTo__PointsOnTheSameValue)
{
	// Arrange
	CREATE(Shared_ptr, ptr) CALL;
	CREATE(Shared_ptr, ptr2) CALL;

	int* i = NULL;
	ALLOC_VARIABLE(i, int);
	ASSERT_NOT_NULL(i);
	*i = 110;

	int* j = NULL;
	ALLOC_VARIABLE(j, int);
	ASSERT_NOT_NULL(j);
	*j = 300;


	MFUN(&ptr, Reset), i CALL;//110
	MFUN(&ptr2, Reset), j CALL;//300

	NTEST_ASSERT(*GET(ptr2, int) == 300);

	// Act
	MFUN(&ptr, CopyTo), & ptr2 CALL;
	MFUN(&ptr2, CopyFrom), & ptr CALL;

	// Assert
	NTEST_ASSERT(*GET(ptr2, int) == 110);

	*i = 220;
	NTEST_ASSERT(*GET(ptr, int) == 220);
	NTEST_ASSERT(*GET(ptr2, int) == 220);

	bool is_empty = true;
	MFUN(&ptr, IsEmpty), & is_empty CALL;
	NTEST_ASSERT(is_empty == false);

	MFUN(&ptr, Release) CALL;


}
END_FUN


TEST_FUN_IMPL(SharedPtrTest, check_type)
{
	// Arrange
	CREATE(Shared_ptr, ptr) CALL;
	CREATE(Shared_ptr, ptr2) CALL;

	float* i = NULL;
	ALLOC_VARIABLE(i, float);
	*i = 10.5f;

	float* j = NULL;
	ALLOC_VARIABLE(j, float);
	*j = 300.3f;

	MFUN(&ptr, Reset), i CALL;//10.5
	MFUN(&ptr2, Reset), j CALL;//300.3

	float tst1 = *GET(ptr2, float);
	bool isOK = (300.3 - 0.01 < tst1) && (tst1 < 300.3 + 0.01);
	NTEST_ASSERT(isOK);
	// Act 
	MFUN(&ptr, CopyTo), & ptr2 CALL;


	MFUN(&ptr2, CopyFrom), & ptr CALL;

	// Assert
	NTEST_ASSERT(*GET(ptr2, float) == 10.5);

	*i = 12.5;
	NTEST_ASSERT(*GET(ptr, float) == 12.5);
	NTEST_ASSERT(*GET(ptr2, float) == 12.5);

	bool is_empty = true;
	MFUN(&ptr, IsEmpty), & is_empty CALL;
	NTEST_ASSERT(is_empty == false);

	MFUN(&ptr, Release) CALL;

}END_FUN


TEST_FUN_IMPL(SharedPtrTest, CREATE_SHARED__macro_test)
{
	// Arrange, Act
	int height = 10;
	int width = 3;
	CREATE_SHARED(BaseClassExample, B_ptr, height, width);
	
	// Assert
	int actual_height = GET(B_ptr, BaseClassExample)->hight;
	int actual_width = GET(B_ptr, BaseClassExample)->width;
	NTEST_ASSERT(actual_height == height);
	NTEST_ASSERT(actual_width == width);

}END_FUN



INIT_TEST_SUITE(SharedPtrTest);
BIND_TEST(SharedPtrTest, check_type_char);
BIND_TEST(SharedPtrTest, CopyTo__PointsOnTheSameValue);
BIND_TEST(SharedPtrTest, pointing_to_class_test);
BIND_TEST(SharedPtrTest, check_type);
BIND_TEST(SharedPtrTest, CREATE_SHARED__macro_test);
END_INIT_TEST_SUITE(SharedPtrTest);