#pragma once
#include <stdbool.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////
/// in the H file:
#define TESTS_LINKED_LIST_TYPE(SUITE_NAME) __single__test_link ##SUITE_NAME ##_t_

#define DEF_TEST_SUITE(SUITE_NAME)\
DEF_CLASS(SUITE_NAME);\
END_DEF(SUITE_NAME);  \
					  \
FUNCTIONS(SUITE_NAME);\
struct TESTS_LINKED_LIST_TYPE(SUITE_NAME) {								\
	int (*_test_func)(SUITE_NAME* _this, int* __is_fail__);		\
	char* name;												\
	struct TESTS_LINKED_LIST_TYPE(SUITE_NAME)* next;						\
}__tests_anchor;											\
MEM_FUN_DECL(SUITE_NAME, __run_all_tests__, struct TESTS_LINKED_LIST_TYPE(SUITE_NAME)* tests_anchor, int* num_passed, int* num_failed)

#define TEST_LINK_NAME(test_name) __ ##test_name ##__LINK_
#define TESTS_ANCHOR(SUITE_NAME) (SUITE_NAME ##VTable.__tests_anchor)

#define ADD_TEST(SUITE_NAME, TEST_NAME)\
MEM_FUN_DECL(SUITE_NAME, TEST_NAME, int * __is_fail__);\
struct TESTS_LINKED_LIST_TYPE(SUITE_NAME) TEST_LINK_NAME(TEST_NAME)

#define END_TEST_SUITE(SUITE_NAME)\
END_FUNCTIONS(SUITE_NAME);

//////////////////////////////////////////////////////////////
/// in the C file:
#define TEST_FUN_IMPL(SUITE_NAME, test_name)\
MEM_FUN_IMPL(SUITE_NAME, test_name, int* __is_fail__)

#define INIT_TEST_SUITE_WITH_SPECIFIC_MEM_SPACE(SUITE_NAME, memSpaceType, memSpaceSize) \
DEF_CTOR(SUITE_NAME) { init_global_memory(memSpaceSize, memSpaceType); } END_CTOR; \
DEF_DTOR(SUITE_NAME) {} END_DTOR; \
MEM_FUN_IMPL(SUITE_NAME, __run_all_tests__, struct TESTS_LINKED_LIST_TYPE(SUITE_NAME)* tests_anchor, int* num_passed, int* num_failed)\
{\
	printf("\n========================\nRunning %s...\n", #SUITE_NAME); fflush(stdout);\
	*num_passed = 0; *num_failed = 0;\
	for (struct TESTS_LINKED_LIST_TYPE(SUITE_NAME)* it = tests_anchor; it != NULL; it = it->next) {\
		printf("... Running %s --> ", it->name); fflush(stdout);\
		int is_fail = false;\
		int __INNER_FUNC_CALL_RET_VALUE__ = it->_test_func(_this, &is_fail);\
		if (IN_THROWING_VALUE == __INNER_FUNC_CALL_RET_VALUE__) { is_fail = true; printf("EXCEPTION THROWN:\t%s\t\n", LAST_EXCEPTION_ERROR_MSG); } \
		if (false != is_fail) { (*num_failed)++; printf("FAILED\n"); fflush(stdout); }\
		else { (*num_passed)++; printf("PASS\n"); fflush(stdout); }\
	}\
}\
END_FUN;\
INIT_CLASS(SUITE_NAME);\
TESTS_ANCHOR(SUITE_NAME).next = NULL;\
BIND(SUITE_NAME, __run_all_tests__);

#define INIT_TEST_SUITE(SUITE_NAME)\
INIT_TEST_SUITE_WITH_SPECIFIC_MEM_SPACE(SUITE_NAME, HEAP_BASED_MEMORY, 10)


#define BIND_TEST(SUITE_NAME, test_name)\
BIND(SUITE_NAME, test_name);\
(V_TABLE_INSTANCE(SUITE_NAME).TEST_LINK_NAME(test_name))._test_func = SUITE_NAME ##VTable.test_name.inner_function;\
(V_TABLE_INSTANCE(SUITE_NAME).TEST_LINK_NAME(test_name)).name = #test_name;\
(V_TABLE_INSTANCE(SUITE_NAME).TEST_LINK_NAME(test_name)).next = (SUITE_NAME ##VTable.__tests_anchor).next;\
TESTS_ANCHOR(SUITE_NAME).next = &(SUITE_NAME ##VTable.TEST_LINK_NAME(test_name));

#define END_INIT_TEST_SUITE(SUITE_NAME)\
END_INIT_CLASS(SUITE_NAME);\

#define IMPORT_TESTS(SUITE_NAME)\
FUN_IMPL(__run__all__tests ##SUITE_NAME)\
{\
CREATE(SUITE_NAME, suite) CALL;\
int num_passed = 0, num_failed = 0;\
struct TESTS_LINKED_LIST_TYPE(SUITE_NAME)* anchor = TESTS_ANCHOR(SUITE_NAME).next;\
MFUN(&suite, __run_all_tests__), anchor, & num_passed, & num_failed CALL;\
printf("PASSED:%d\tFAILED:%d\n", num_passed, num_failed);\
}END_FUN

#define RUN_TESTS(SUITE_NAME) __run__all__tests ##SUITE_NAME();

#define NTEST_ASSERT(x) if (!(x)) {printf("\tFAIL: %s is False\t", #x ); *__is_fail__ = true; RETURN;}
#define EXPECT_THROW { int __exception_tester__ = 0; if (1) { TRY {

#define ASSERT_THROW __exception_tester__++; } CATCH {} END_TRY; } \
if (__exception_tester__ != 0) {printf("\tFAIL: exception was not thrown\t"); *__is_fail__ = true; RETURN;} }
