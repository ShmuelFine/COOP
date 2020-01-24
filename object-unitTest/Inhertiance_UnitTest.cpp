//#include "pch.h"
//#include "LibLoadUtils.h"
//#include "../object/SuperMat4Test.h"
//
//
//class InheritTest : public ::testing::Test
//{
//public:
//	HMODULE gmedLib;
//	DEF_PTR_TO_FUNC(SuperMat4Test_init);
//
//	virtual void SetUp()
//	{
//		gmedLib = LoadLibraryA("object.dll");
//		LOAD_FUNC(SuperMat4Test_init);
//		SuperMat4Test_init_ptr();
//	}
//
//	virtual void TearDown()
//	{
//
//	}
//};
//
//TEST_F(InheritTest, Ctor_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCallen)
//{
//	//Arrange
//	SuperMat4Test mat;
//
//	//Act
//	mat.vTable->_ctor(&mat, 6, 8, 2);
//
//	//Assert
//	
//}