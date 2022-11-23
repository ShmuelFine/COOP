#include "CharTest.h"

FUN_IMPL(check_type_char)
{
	// Arrange
	CREATE(MemoryManager, memManager), sizeof(int) * 20, STACK_BASED_MEMORY CALL;


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

	TEST_ASSERT(*GET(ptr2, char) == 'b');

	// Act 
	FUN(&ptr, CopyTo), & ptr2 CALL;
	FUN(&ptr2, CopyFrom), & ptr CALL;

	// Assert
	TEST_ASSERT(*GET(ptr2, char) == 'a');

	*i = 'c';
	TEST_ASSERT(*GET(ptr, char) == 'c');
	TEST_ASSERT(*GET(ptr2, char) == 'c');

	bool out;
	FUN(&ptr, IsEmpty), & out CALL;
	TEST_ASSERT(!(out == 0));

	FUN(&ptr, Release) CALL;

}END_FUN