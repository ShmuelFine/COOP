#include "InheritTest.h"

#define ASSERT_TRUE(condition) if(condition) return true; else return false;
#define ASSERT_EQ(param1,param2) if((param1) == (param2)) return true; else return false;

bool Ctor_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCallen()
{
	SuperMat4Test_init();
	//Arrange
	CREATE_OBJECT(SuperMat4Test, mat)


	//Act
	mat.vTable->_ctor(&mat, 6, 8, 2);
	//Assert
	ASSERT_TRUE(mat._BASE.hight == 6);
}


bool VTable_WhenDeriving_OverriddeesVTablePointer()
{
	//Arange
	SuperMat4Test_init();

	//Act
	CREATE_OBJECT(SuperMat4Test, mat);

	//Assert
	ASSERT_EQ(&(mat.vTable),&(mat._BASE.vTable));
}

bool VTable_WhenDriving_CanCallNoneOverridedBaseFunctionsViaOwnVTable()
{
	//Arange
	SuperMat4Test_init();
	CREATE_OBJECT(SuperMat4Test, mat);
	mat.vTable->_ctor(&mat, 4, 4, 3);

	//Act
	int width=0;
	mat.vTable->_BASE.getWidth.func((Mat4Test * )&mat, &width);
 
	//Assert
	ASSERT_EQ(width, 4);
}

bool VTable_WhenDriving_NotOverridingTheBaseMembers()
{
	//Arange
	SuperMat4Test_init();
	int h = 4, w = 4, step = 3;
	CREATE_OBJECT(SuperMat4Test, mat);


	//Act
	mat.vTable->_ctor(&mat, h, w, step);

	//Assert
	ASSERT_EQ(mat._BASE.hight, h);
	ASSERT_EQ(mat._BASE.width, w);
}

bool ChainInheritance_WhenDriving_AllBasesAreInited()
{
	//Arange
	SuperMat3_init();

	//Act
	CREATE_OBJECT(SuperMat3, mat);

	//Assert
	ASSERT_TRUE(mat.vTable->_BASE._ctor != NULL);
	ASSERT_TRUE(mat.vTable->_BASE._BASE._ctor != NULL);
}

bool ChainInheritance_WhenDriving_NotOverridingTheBaseMembers()
{
	//Arange
	SuperMat3_init();
	int h = 4, w = 4, step = 3;
	CREATE_OBJECT(SuperMat3, mat);


	//Act
	mat.vTable->_ctor(&mat, h, w, step,false);

	//Assert
	ASSERT_EQ(mat._BASE._BASE.hight, h);
	ASSERT_EQ(mat._BASE._BASE.width, w);
	ASSERT_EQ(mat._BASE.step, step);
}

bool ChainInheritance_Casting_EnabledCasting2Base()
{
	//Arange
	SuperMat3_init();
	int h = 4, w = 4, step = 3;
	CREATE_OBJECT(SuperMat3, mat);
	mat.vTable->_ctor(&mat, h, w, step, false);

	int loc;
	int newStep = 1;

	//Act
	SuperMat4Test* base1 = &mat;
	Mat4Test* base2 = &mat;


	mat.vTable->_BASE.SetStep(base1, newStep);
	//mat.vTable->_BASE._BASE.findLoc(base2, 1, 1, &loc);
	CALL(Mat4Test, findLoc, *base2, 1, 1, &loc);

	//Assert
	ASSERT_EQ(mat._BASE.step, newStep);
	ASSERT_EQ(loc, 1*4+1);
}

bool Overridding_WhenCallingAFunction_AlwaysCallsTheOvveridden()
{
	//Arrange
	CREATE_DERIVED_OBJECT(SuperMat3, SuperMat4Test, mat);
	mat.vTable->_ctor(&mat, 4, 4, 2, false);

	//Act
	int expectedLoc = (mat._BASE._BASE.width * 1 + 2) * mat._BASE.step + 1;
	int actualLoc;
	//CALL(SuperMat3, findLoc, mat, 1, 2, &actualLoc);
	mat.vTable->findLoc.func(&mat, 1, 2, &actualLoc);
	void (*func)(void) = callFunction(&((mat).vTable->findLoc))->func; 
	func(&(mat), 1,2, &actualLoc);



	//Assert
	ASSERT_EQ(expectedLoc, actualLoc);

}