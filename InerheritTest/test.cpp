#include "pch.h"
#include "../object/B.h"


 TEST(testMath, myCubeTest){
	//arrenge
	int a = 3;
	int b = 4;
	//act
	int c = aaa(a, b);

  EXPECT_EQ(c, 7);
}