#include "ClassUnitTest.h"
#include "COOP.h"
#include "Shared_ptr.h"
#include "ScopesUnitTest.h"

FUN_IMPL(check_type_char)
{
	// Arrange
	CREATE(MemoryManager, memManager), sizeof(int) * 20, STACK_BASED_MEMORY);


	CREATE(Shared_ptr, ptr));
	CREATE(Shared_ptr, ptr2));

	char* i = NULL;
	NEW(i, char);
	*i = 'a';

	char* j = NULL;
	NEW(j, char);
	*j = 'b';

	FUN(&ptr, Reset), i CALL;//a
	FUN(&ptr2, Reset), j CALL;//b
	
	ASSERT(*GET(ptr2, char) == 'b')
	
	// Act 
	FUN(&ptr, CopyTo), & ptr2 CALL;
	FUN(&ptr2, CopyFrom), & ptr CALL;

	// Assert
	ASSERT(*GET(ptr2, char) == 'a');

	*i = 'c';
	ASSERT(*GET(ptr, char) == 'c');
	ASSERT(*GET(ptr2, char) == 'c');

	bool out;
	FUN(&ptr, IsEmpty), & out CALL;
	ASSERT(!(out == 0));

	FUN(&ptr, Release) CALL;

}END_FUN