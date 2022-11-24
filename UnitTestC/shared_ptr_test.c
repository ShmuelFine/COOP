#include "shared_ptr_test.h"
#include "BaseClassExample.h"
#include "../COOP/Shared_ptr.h"

TEST_FUN_IMPL(SharedPtrTest, check_type_char)
{
	// Arrange
	init_global_memory( sizeof(int) * 20, STACK_BASED_MEMORY );


	CREATE(Shared_ptr, ptr) CALL;
	CREATE(Shared_ptr, ptr2) CALL;

	char* i = NULL;
	NEW(i, char);
	*i = 'a';

	char* j = NULL;
	NEW(j, char);
	*j = 'b';

	FUN(&ptr, Reset), i CALL;//a
	FUN(&ptr2, Reset), j CALL;//b

	NTEST_ASSERT(*GET(ptr2, char) == 'b');

	// Act 
	FUN(&ptr, CopyTo), & ptr2 CALL;
	FUN(&ptr2, CopyFrom), & ptr CALL;

	// Assert
	NTEST_ASSERT(*GET(ptr2, char) == 'a');

	*i = 'c';
	NTEST_ASSERT(*GET(ptr, char) == 'c');
	NTEST_ASSERT(*GET(ptr2, char) == 'c');

	bool out;
	FUN(&ptr, IsEmpty), & out CALL;
	NTEST_ASSERT(!(out == 0));

	FUN(&ptr, Release) CALL;

}END_FUN


TEST_FUN_IMPL(SharedPtrTest, pointing_to_class_test)
{
	init_global_memory(sizeof(int) * 20, STACK_BASED_MEMORY);

	CREATE(Shared_ptr, ptr) CALL;
	CREATE(Shared_ptr, ptr2) CALL;

	CREATE(BaseClassExample, B), 10, 3 CALL;
	CREATE(BaseClassExample, C), 3, 10 CALL;

	FUN(&ptr, Reset), & B CALL;
	FUN(&ptr2, Reset), & C CALL;

	NTEST_ASSERT(ptr.px == &B);
	// Act
	FUN(&ptr, CopyTo), & ptr2 CALL;

	FUN(&ptr2, CopyFrom), & ptr CALL;


	bool out;
	FUN(&ptr, IsEmpty), & out CALL;
	NTEST_ASSERT(!(out == 0));

	FUN(&ptr, Release) CALL;


}
END_FUN

INIT_TEST_SUITE(SharedPtrTest);
BIND_TEST(SharedPtrTest, check_type_char);
BIND_TEST(SharedPtrTest, pointing_to_class_test);
END_INIT_TEST_SUITE(SharedPtrTest);