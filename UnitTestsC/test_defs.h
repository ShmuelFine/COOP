#pragma once
#include <stdbool.h>
#include <stdio.h>

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

