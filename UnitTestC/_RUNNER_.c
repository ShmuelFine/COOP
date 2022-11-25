#include "test_defs.h"
#include "DemoClassTest.h"
#include "InheritanceTest.h"
#include "rbTreeIterUnitTest.h"
#include "rbTreeUnitTest.h"
#include "vectorIteratorUnitTest.h"
#include "vectorUnitTest.h"
#include "ExeptionUnitTest.h"
#include "MemManagementUnitTest.h"
#include "shared_ptr_test.h"
#include "ScopesUnitTest.h"
#include "HashTableUnitTest.h"
#include "NdarrayTest.h"


IMPORT_TESTS(Infra_ScopesTest);
IMPORT_TESTS(NDarray_Tests);
IMPORT_TESTS(DemoClassTest);
IMPORT_TESTS(VectorTest);
IMPORT_TESTS(MemoryManagementTest);
IMPORT_TESTS(InheritanceTest);
IMPORT_TESTS(ExceptionTests);
IMPORT_TESTS(SharedPtrTest);
//IMPORT_TESTS(VectorIteratorTest);
IMPORT_TESTS(RedBlackTreeIteratorTest);
IMPORT_TESTS(RedBlackTreeTest);

int main() {

	init_global_memory(sizeof(int) * 10, HEAP_BASED_MEMORY);

	//RUN_TESTS(Infra_ScopesTest);
	//RUN_TESTS(MemoryManagementTest);
	//RUN_TESTS(InheritanceTest);
	//RUN_TESTS(ExceptionTests);
	//RUN_TESTS(SharedPtrTest);
	//
	//RUN_TESTS(NDarray_Tests);
	//RUN_TESTS(DemoClassTest);
	RUN_TESTS(VectorTest);
	//RUN_TESTS(RedBlackTreeTest);
	//RUN_TESTS(RedBlackTreeIteratorTest);

	return 0;
	//RUN_TESTS(VectorIteratorTest);

	//////HashTableC! 
	////RUN_TEST(check_insert_SanityTest);
	////RUN_TEST(check_insert_when_already_exists_then_throw);
	////RUN_TEST(check_search_when_founded_then_return_instance);
	////RUN_TEST(check_search_when_not_founded_then_return_null);
	////RUN_TEST(check_delete_when_exist_then_delete);


}

