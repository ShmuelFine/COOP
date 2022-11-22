#pragma once
#include <stdbool.h>
#include <stdio.h>

/// in the H file:
#define DEF_TEST_SUITE(SUITE_NAME)\
DEF_CLASS(SUITE_NAME);\
END_DEF(SUITE_NAME);\
FUNCTIONS(SUITE_NAME);\
MEM_FUN_DECL(SUITE_NAME, _run_all_tests_, int* num_passed, int* num_failed);

#define ADD_TEST(SUITE_NAME, TEST_NAME)\
MEM_FUN_DECL(SUITE_NAME, TEST_NAME, int* out_is_succeed);

#define END_TEST_SUITE_DEF(SUITE_NAME)\
END_FUNCTIONS(SUITE_NAME);

/// in the C file:
#define IMPL_TEST_SUITE(SUITE_NAME)\
DEF_CTOR(SUITE_NAME) {}\
END_CTOR\
DEF_DTOR(SUITE_NAME) {}\
END_DTOR\
MEM_FUN_IMPL(SUITE_NAME, _run_all_tests_, int* num_passed, int* num_failed)\
{}









#define TEST_FAILED 1
#define TEST_PASS 0

#define INIT_TESTS() \
bool isAllPass = true;

#define TEST_ASSERT(x) if (!(x)) { printf("\tFAIL: %s is False\t", #x ); return TEST_FAILED; }


#define RUN_TEST(T) \
{ printf("%s -->\t", #T); \
int res = T();\
printf(res == TEST_PASS ? "PASS" : "FAIL");\
printf("\n");\
if (res != TEST_PASS)\
	isAllPass = false;\
}

#define FINISH_TESTS() \
return isAllPass ? TEST_PASS : TEST_FAILED;

#define RUN_TESTS_SUITE(S) { \
printf("\n**************************************************************************************\n");\
printf("*** STARTING:       %s       \n", #S);\
printf("**************************************************************************************\n");\
int res = S();\
printf("**************************************************************************************\n");\
printf("*** FINISHED        %s  ==>   %s \n", #S, res == TEST_PASS ? "PASS" : "FAIL");\
printf("**************************************************************************************\n");\
if (res != TEST_PASS)\
	isAllPass = false;\
}

#define PRINT_ALL_TESTS_RES()\
printf("\n------- Did all tests pass ? %s ---------- \n", isAllPass ? "PASS" : "FAIL")

