#include "UnitTestsInfra.h"
#include "DemoClassTest.h"
#include "InheritanceTest.h"
#include "rbTreeIterUnitTest.h"
#include "rbTreeUnitTest.h"
#include "vectorUnitTest.h"
#include "ExeptionUnitTest.h"
#include "MemManagementUnitTest.h"
#include "shared_ptr_test.h"
#include "ScopesUnitTest.h"


IMPORT_TESTS(Infra_ScopesTest);
IMPORT_TESTS(DemoClassTest);
IMPORT_TESTS(VectorTest);
IMPORT_TESTS(MemoryManagementTest);
IMPORT_TESTS(InheritanceTest);
IMPORT_TESTS(ExceptionTests);
IMPORT_TESTS(SharedPtrTest);
//IMPORT_TESTS(RedBlackTreeTest);
//IMPORT_TESTS(RedBlackTreeIteratorTest);

int main() {

	init_global_memory(sizeof(int) * 10, HEAP_BASED_MEMORY);

	RUN_TESTS(Infra_ScopesTest);
	RUN_TESTS(MemoryManagementTest);
	RUN_TESTS(InheritanceTest);
	RUN_TESTS(ExceptionTests);
	RUN_TESTS(SharedPtrTest);
	
	RUN_TESTS(VectorTest);
	//RUN_TESTS(RedBlackTreeTest);
	//RUN_TESTS(RedBlackTreeIteratorTest);

	RUN_TESTS(DemoClassTest);

	return 0;

}

