#include "pch.h"
#include "..\coop\memoryManagement.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(memoryManagement, myMallocTest) {
	////Arrange
		char data[10 * 1024];
		Heap h;
		h.buffer = data;
		h.buffSize = 10240;
		bool bb[10 * 1024];
		h.flagArrFull = bb;
		h.tail = h.head = NULL;
	////*char* str = myMalloc(h, 40);
	//Act
		////Act
	int str = aaa(3,5);

	////expected
	EXPECT_EQ(5, 5);
}
//TEST(memoryManagement, myMallocTest) {}
	////Arrange
	//char data[10 * 1024];
	//Heap h;
	//h.buffer = data;
	//h.buffSize = 10240;
	//bool bb[10 * 1024];
	//h.flagArrFull = bb;
	//h.tail = h.head = NULL;
	//*char* str = myMalloc(h, 40);
	//myFree(h, str);*/
	////Act
	//char* str = myMalloc(h, 40);

	////expected
	//EXPECT_EQ(5, 5);
//}