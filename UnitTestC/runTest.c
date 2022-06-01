#include "runTest.h"
int main() {
	INIT_TESTS();
	RUN_TEST(is_the_test_platform_run_as_well);
	RUN_TEST(Class_CallSimpleFunction);
	RUN_TEST(Rectangle_SanityTest);
	RUN_TEST(Inheritence_WhenCallingNonVirtualFunciton_Sanity);
	RUN_TEST(Inheritence_WhenVirtualFunctionOverriden_ThenCallInvokesOverridingFunc);
	RUN_TEST(Inheritence_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCalledAsWell);
	RUN_TEST(redBlacktreeIterator_increment_Sanity);
	RUN_TEST(redBlacktreeIterator_decrement_Sanity);


	RUN_TEST(rbTree_insert_SanityTest);
	RUN_TEST(rbTree_find_SanityTest);
	RUN_TEST(rbTree_delete_SanityTest);
	RUN_TEST(rbTree_iteration_SanityTest);
	RUN_TEST(rbTree_rectangle_as_node_data_insert_SanityTest);
	RUN_TEST(rbTree_rectangle_as_node_data_find_SanityTest);
	RUN_TEST(rbTree_rectangle_as_node_data_delete_SanityTest);

	
}
