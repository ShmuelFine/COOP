#include "UnitTestsInfra.h"
#include "DemoClassTest.h"
#include "InheritanceTest.h"
#include "VectorUnitTest.h"
#include "ExeptionUnitTest.h"
#include "MemManagementUnitTest.h"
#include "SharedPodPtrUnitTest.h"
#include "ScopesUnitTest.h"
#include "TensorUnitTest.h"
#include "CodeBlocksUnitTest.h"
#include "listUnitTest.h"
#include "IteratorUnitTest.h"
#include "BinaryTreeUnitTest.h"

IMPORT_TESTS(Infra_ScopesTest);
IMPORT_TESTS(DemoClassTest);
IMPORT_TESTS(VectorTest);
IMPORT_TESTS(TensorTest);
IMPORT_TESTS(MemoryManagementTest);
IMPORT_TESTS(InheritanceTest);
IMPORT_TESTS(ExceptionTests);
IMPORT_TESTS(SharedPtrTest);
IMPORT_TESTS(CodeBlocksTest);
IMPORT_TESTS(ListTest);
IMPORT_TESTS(BinaryTreeTest);
IMPORT_TESTS(IteratorForTest);

int main() {

	init_global_memory(sizeof(int) * 10, HEAP_BASED_MEMORY);

	RUN_TESTS(Infra_ScopesTest);
	RUN_TESTS(CodeBlocksTest);
	RUN_TESTS(MemoryManagementTest);
	RUN_TESTS(InheritanceTest);
	RUN_TESTS(ExceptionTests);
	RUN_TESTS(SharedPtrTest);
	
	RUN_TESTS(VectorTest);
	RUN_TESTS(TensorTest);
	RUN_TESTS(BinaryTreeTest);
	RUN_TESTS(IteratorForTest);

	RUN_TESTS(DemoClassTest);

	RUN_TESTS(ListTest);

	return 0;

}

