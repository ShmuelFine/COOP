#include "runTest.h"

IMPORT_TESTS(NDarray_Tests);
IMPORT_TESTS(DemoClassTest);
IMPORT_TESTS(VectorTest);
IMPORT_TESTS(MemoryManagementTest);

int main() {

	init_global_memory(sizeof(int) * 10, HEAP_BASED_MEMORY);

	RUN_TESTS(NDarray_Tests);
	RUN_TESTS(DemoClassTest);
	RUN_TESTS(VectorTest);
	RUN_TESTS(MemoryManagementTest);

	//////HashTableC! 
	////RUN_TEST(check_insert_SanityTest);
	////RUN_TEST(check_insert_when_already_exists_then_throw);
	////RUN_TEST(check_search_when_founded_then_return_instance);
	////RUN_TEST(check_search_when_not_founded_then_return_null);
	////RUN_TEST(check_delete_when_exist_then_delete);


	//////////////////////////


	//RUN_TEST(Inheritence_WhenCallingNonVirtualFunciton_Sanity);
	//RUN_TEST(Inheritence_WhenVirtualFunctionOverriden_ThenCallInvokesOverridingFunc);
	//RUN_TEST(Inheritence_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCalledAsWell);
	//RUN_TEST(redBlacktreeIterator_increment_Sanity);
	//RUN_TEST(redBlacktreeIterator_decrement_Sanity);

	////rbTreeUnitTest
	//RUN_TEST(rbTree_insert_SanityTest);
	//RUN_TEST(rbTree_find_SanityTest);
	//RUN_TEST(rbTree_delete_SanityTest);
	//RUN_TEST(rbTree_iteration_SanityTest);
	//RUN_TEST(rbTree_rectangle_as_node_data_insert_SanityTest);
	//RUN_TEST(rbTree_rectangle_as_node_data_find_SanityTest);
	//RUN_TEST(rbTree_rectangle_as_node_data_delete_SanityTest);

	////vectorIteratorUnitTest
	//RUN_TEST(vectorIterator_getContentsOf_Sanity);
	//RUN_TEST(vectorIterator_increment_Sanity);

	////ExeptionUnitTest
	//RUN_TEST(CATCH_WhenExeptionCaught_Does_NOT_ContinueThrowing);
	//RUN_TEST(BREAK_WhenDoneFromNastedLoop_ThenBreaksCorrectly);
	//RUN_TEST(BREAK_WhenDoneFromTRY_Catch_Block_ThenBreaksCorrectly);
	//RUN_TEST(BREAK_WhenDoneFromLoop_ThenFreesObjectsFromInnerScope);

	////MemManagementUnitTest
	//RUN_TEST(NEW_AllocatesSufficientMemory);
	//RUN_TEST(New_WhenAllocatingClassInstance_ThenReturnsRightAmountOfMemroy);
	//RUN_TEST(Delete_WhenDeletingAllocMemory_ThenItFreesTheResource);

	////CharTest
	//RUN_TEST(check_type_char);

	////ScopesUnitTest
	//RUN_TEST(SCOPE_END__WhenObjectsDefinedInsideScope_ThenAllGetFreed);
	//RUN_TEST(LOCAL_SCOPE__WhenObjectsDefinedInside_InnerScope_ThenAllGetFreed);
	//RUN_TEST(LOCAL_SCOPE__DoesNotFreeUnrelatedObjects);
	//RUN_TEST(LOCAL_SCOPE__WhenMultipleNestedScopesExist_ThenFreesOnlyTheInnerMost);
	//RUN_TEST(LOCAL_SCOPE__WhenMostInnerScopeHasNoObjects_ThenDoesntCrash);
	//RUN_TEST(LOCAL_SCOPE__LoopScopeSanityTest);
	//RUN_TEST(Shared_ptr__CopyTo__PointsOnTheSameValue);


	////TypeTest
	//RUN_TEST(check_type);


}

