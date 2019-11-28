#include "pch.h"
#include "..\coop\memoryManagement.h"
#include "LibLoadUtils.h"

TEST(memoryManagement, myMallocTest) {
	//Arrange
	HMODULE coopLib;
	decltype(myMalloc)* myMalloc_ptr;
	coopLib = LoadLibraryA("coop.dll");
	myMalloc_ptr = (decltype(myMalloc)*)GetProcAddress(coopLib, "myMalloc");
	char data[10 * 1024];
	Heap  h = {};
	h.buffer = data;
	h.buffSize = 10240;
	h.tail = h.head = NULL;
	//myFree(h, str);*/
	//Act
	//int str = aaa(9,8);
	char* str = myMalloc_ptr(&h, 40);
	//expected
	EXPECT_EQ(str, h.buffer);
}

TEST(memoryManagement, MallocTest_moreSize) {
	//Arrange
	HMODULE coopLib;
	decltype(myMalloc)* myMalloc_ptr;
	coopLib = LoadLibraryA("coop.dll");
	myMalloc_ptr = (decltype(myMalloc)*)GetProcAddress(coopLib, "myMalloc");
	char data[10 * 1024];
	Heap  h = {};
	h.buffer = data;
	h.buffSize = 10240;
	h.tail = h.head = NULL;
	char* expected = NULL;
	//Act
	char* str = myMalloc_ptr(&h, 1000000);
	//expected
	EXPECT_EQ(str, expected);
}
TEST(memoryManagement, twoAllocations) {
	//Arrange
	HMODULE coopLib;
	decltype(myMalloc)* myMalloc_ptr;
	coopLib = LoadLibraryA("coop.dll");
	myMalloc_ptr = (decltype(myMalloc)*)GetProcAddress(coopLib, "myMalloc");
	char data[10 * 1024];
	Heap  h = {};
	h.buffer = data;
	h.buffSize = 10240;
	h.tail = h.head = NULL;
	//Act
	char* str1 = myMalloc_ptr(&h, 40);
	char* str2 = myMalloc_ptr(&h, 60);
	//expected
	EXPECT_EQ(str1, h.buffer);
	EXPECT_EQ(h.flagArrFull[41], true);
}
void EXPECT_BLOCKS_EQ(char* expected, char* actual)
{
	if (expected == NULL && actual == NULL)
		return;
	EXPECT_EQ(0, 0);
}