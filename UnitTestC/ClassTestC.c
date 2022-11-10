#include "ClassTestC.h"

DEF_CLASS(BaseClassExample);
int hight;
int width;
END_DEF(BaseClassExample);

FUNCTIONS(BaseClassExample, int hight, int width);
MEM_FUN_DECL(BaseClassExample, GetVolume, int* retVal);
END_FUNCTIONS(BaseClassExample);

FUN_IMPL(Class_test)
{
	CREATE(MemoryManager, memManager), sizeof(int) * 20, STACK_BASED_MEMORY);

	CREATE(Shared_ptr, ptr));
	CREATE(Shared_ptr, ptr2));

	CREATE(BaseClassExample, B), 10, 3);
	CREATE(BaseClassExample, C), 3, 10);

	FUN(&ptr, Reset), & B CALL;
	FUN(&ptr2, Reset), & C CALL;

	TEST_ASSERT(ptr.px == &B)
		// Act
		FUN(&ptr, CopyTo), & ptr2 CALL;

	FUN(&ptr2, CopyFrom), & ptr CALL;


	bool out;
	FUN(&ptr, IsEmpty), & out CALL;
	TEST_ASSERT(!(out == 0));

	FUN(&ptr, Release) CALL;


}
END_FUN