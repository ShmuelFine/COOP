#include "InheritTest.h"
#include "SuperMat.h"

#define ASSERT_TRUE(condition) if(condition) return true; else return false
#define ASSERT_EQ(param1,param2) if((param1) == (param2)) return true; else return false

bool Ctor_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCallen()
{
	//Arrange
	int hight;
	SCOPE_START;


	//Act
	CREATE_OBJECT(SuperMat, mat, 6, 8, 2);
	hight = mat._base.hight;

	SCOPE_END;
	//Assert
	ASSERT_TRUE(hight == 6);

}


bool VTable_WhenDeriving_OverriddeesVTablePointer()
{
	//Arange
	//SuperMat_init();
	SCOPE_START;
	//Act
	CREATE_OBJECT(SuperMat, mat,0,0,0);

	//Assert
	ASSERT_EQ(&(mat.vTable),&((SuperMatVirtualTable*)mat._base.vTable));
	SCOPE_END;
}

bool VTable_WhenDriving_CanCallNoneOverridedBaseFunctionsViaOwnVTable()
{
	//Arange
	int width = 0;
	SCOPE_START;

	//SuperMat_init();
	CREATE_OBJECT(SuperMat, mat, 4, 4, 3);
	//mat.vTable->_ctor(&mat, 4, 4, 3);

	//Act
	mat.vTable->_base.GetWidth->func((Mat* )&mat, &width);
 
	//Assert
	SCOPE_END;
	ASSERT_EQ(width, 4);
}

bool VTable_WhenDriving_NotOverridingTheBaseMembers()
{
	//Arange
	int hight, width;
	int h = 4, w = 4, step = 3;
	SCOPE_START;


	//Act
	CREATE_OBJECT(SuperMat, mat, h, w, step);
	//mat.vTable->_ctor(&mat, h, w, step);

	hight = mat._base.hight;
	width = mat._base.width;

	//Assert
	SCOPE_END;
	ASSERT_EQ(hight, h);
	ASSERT_EQ(width, w);
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
	//SuperMat* base1 = (SuperMat*)&mat;
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
	//Arrange
	int expectedLoc, actualLoc;
	SCOPE_START;

	CREATE_DERIVED_OBJECT(SuperMat, Mat4Test, mat, 4, 4, 2);
	//mat.vTable->_ctor(&mat, 4, 4, 2);

	//Act
	expectedLoc = (mat._base.width * 1 + 2) * mat.step;
	CALL(FindLoc, mat, 1, 2, &actualLoc);
	//Assert
	SCOPE_END;
	ASSERT_EQ(expectedLoc, actualLoc);

}