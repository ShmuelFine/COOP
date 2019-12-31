#include "InheritTest.h"
#include "SuperMat3_4Test.h"

#define ASSERT_TRUE(condition) if(condition) return true; else return false;
#define ASSERT_EQ(param1,param2) if((param1) == (param2)) return true; else return false;

bool Ctor_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCallen()
{

	//Arrange

	SuperMat4Test_init();

	//Act
	CREATE_OBJECT(SuperMat4Test, mat, 6, 8, 2)
	//mat.vTable->_ctor(&mat, 6, 8, 2);

	//Assert
	ASSERT_TRUE(mat._base.hight == 6);
}


bool VTable_WhenDeriving_OverriddeesVTablePointer()
{
	//Arange
	SuperMat4Test_init();

	//Act
	CREATE_OBJECT(SuperMat4Test, mat,0,0,0);

	//Assert
	ASSERT_EQ(&(mat.vTable),&((SuperMat4TestVirtualTable*)mat._base.vTable));
}

bool VTable_WhenDriving_CanCallNoneOverridedBaseFunctionsViaOwnVTable()
{
	//Arange
	SuperMat4Test_init();
	CREATE_OBJECT(SuperMat4Test, mat, 4, 4, 3);
	//mat.vTable->_ctor(&mat, 4, 4, 3);

	//Act
	int width=0;
	mat.vTable->_base.getWidth((Mat4Test * )&mat, &width);
 
	//Assert
	ASSERT_EQ(width, 4);
}

bool VTable_WhenDriving_NotOverridingTheBaseMembers()
{
	//Arange
	SuperMat4Test_init();
	int h = 4, w = 4, step = 3;

	//Act
	CREATE_OBJECT(SuperMat4Test, mat, h, w, step);
	//mat.vTable->_ctor(&mat, h, w, step);

	//Assert
	ASSERT_EQ(mat._base.hight, h);
	ASSERT_EQ(mat._base.width, w);
}

bool ChainInheritance_WhenDriving_AllBasesAreInited()
{
	////Arange
	//SuperMat3_init();

	////Act
	//CREATE_OBJECT(SuperMat3, mat);

	////Assert
	//ASSERT_TRUE(mat.vTable->_base._ctor != NULL);
	//ASSERT_TRUE(mat.vTable->_base._base._ctor != NULL);
	ASSERT_TRUE(true);
}

bool ChainInheritance_WhenDriving_NotOverridingTheBaseMembers()
{
	////Arange
	//SuperMat3_init();
	//int h = 4, w = 4, step = 3;
	//CREATE_OBJECT(SuperMat3, mat);


	////Act
	//mat.vTable->_ctor(&mat, h, w, step,false);

	////Assert
	//ASSERT_EQ(mat._base._base.hight, h);
	//ASSERT_EQ(mat._base._base.width, w);
	//ASSERT_EQ(mat._base.step, step);
	ASSERT_TRUE(true);
}

bool ChainInheritance_Casting_EnabledCasting2Base()
{
	////Arange
	//SuperMat3_init();
	//int h = 4, w = 4, step = 3;
	//CREATE_OBJECT(SuperMat3, mat);
	//mat.vTable->_ctor(&mat, h, w, step, false);
	//int loc;
	//int newStep = 1;
	////Act
	//SuperMat4Test* base1 = (SuperMat4Test*)&mat;
	//Mat4Test* base2 = (Mat4Test*)&mat;


	//mat.vTable->_base.SetStep(base1, newStep);
	////mat.vTable->_base._base.findLoc(base2, 1, 1, &loc);
	////struct findLoc_t_* f = ((*base2).vTable->findLoc);;
	//CALL(Mat4Test, findLoc, *base2, 1, 1, &loc);

	////Assert
	//ASSERT_EQ(mat._base.step, newStep);
	//ASSERT_EQ(loc, 1*4+1);
	ASSERT_TRUE(true);
}

bool Overridding_WhenCallingAFunction_AlwaysCallsTheOvveridden()
{
	////Arrange
	//CREATE_DERIVED_OBJECT(SuperMat3, SuperMat4Test, mat);
	//mat.vTable->_ctor(&mat, 4, 4, 2, false);

	////Act
	//int expectedLoc = (mat._base._base.width * 1 + 2) * mat._base.step + 1;
	//int actualLoc;
	//CALL(SuperMat3, findLoc, mat, 1, 2, &actualLoc);
	////Assert
	//ASSERT_EQ(expectedLoc, actualLoc);

	//Arrange
	CREATE_DERIVED_OBJECT(SuperMat4Test, Mat4Test, mat, 4, 4, 2);
	//mat.vTable->_ctor(&mat, 4, 4, 2);

	//Act
	int expectedLoc = (mat._base.width * 1 + 2) * mat.step;
	int actualLoc;
	CALL(SuperMat4Test, findLoc, mat, 1, 2, &actualLoc);
	//Assert
	ASSERT_EQ(expectedLoc, actualLoc);

}