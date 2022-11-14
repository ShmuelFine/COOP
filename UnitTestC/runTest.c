#include "runTest.h"
int main() {
	INIT_TESTS();

	//RUN_TEST(is_the_test_platform_run_as_well);
	//RUN_TEST(Class_CallSimpleFunction);
	//RUN_TEST(Rectangle_SanityTest);
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

	//vectorUnitTest
	//RUN_TEST(vector_push_back_SanityTest);
	//RUN_TEST(vector_iteration_SanityTest);


	//
	////ExeptionUnitTest
	//RUN_TEST(CATCH_WhenExeptionCaught_Does_NOT_ContinueThrowing);
	//RUN_TEST(BREAK_WhenDoneFromNastedLoop_ThenBreaksCorrectly);
	//RUN_TEST(BREAK_WhenDoneFromTRY_Catch_Block_ThenBreaksCorrectly);
	//RUN_TEST(BREAK_WhenDoneFromLoop_ThenFreesObjectsFromInnerScope);

	////MemManagementUnitTest
	//RUN_TEST(New_WhenAllocatingBuffer_ThenReturnsRightAmountOfMemroy);
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

	/////ClassTestC
	//RUN_TEST(Class_test);

	//////HashTableC! 
	////RUN_TEST(check_insert_SanityTest);
	////RUN_TEST(check_insert_when_already_exists_then_throw);
	////RUN_TEST(check_search_when_founded_then_return_instance);
	////RUN_TEST(check_search_when_not_founded_then_return_null);
	////RUN_TEST(check_delete_when_exist_then_delete);

	//Ndarray
	RUN_TEST(Ndarray_init_sanity_case);
	RUN_TEST(Ndarray_init_invalid_dims);
	RUN_TEST(Ndarray_init_invalid_shape);
	RUN_TEST(Ndarray_get_location_3D);
	RUN_TEST(Ndarray_set_sanity_case);
	RUN_TEST(Ndarray_set_index_out_of_range);
	RUN_TEST(Ndarray_at_sanity_case);
	RUN_TEST(Ndarray_at_index_out_of_range);
	RUN_TEST(Ndarray_fill_sanity_case);
	RUN_TEST(Ndarray_contains_when_true);
	RUN_TEST(Ndarray_contains_when_false);
	RUN_TEST(Ndarray_min_sanity_case);
	RUN_TEST(Ndarray_max_sanity_case)
}

