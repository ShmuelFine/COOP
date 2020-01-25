#include "InheritTest.h"
#include "SuperMat.h"


#define ASSERT_TRUE(condition) if(!(condition)) __RET_VAL__ = TEST_FAIL
#define ASSERT_EQ(param1,param2) if((param1) != (param2)) __RET_VAL__ = TEST_FAIL

int Ctor_WhenCallingCtorOfDrivedClass_BaseClassCtorIsCallen()
{
	SCOPE_START;

	//Arrange
	int hight;

	//Act
	CREATE_OBJECT(SuperMat, mat, 6, 8, 2);
	hight = mat._base.hight;

	//Assert
	ASSERT_TRUE(hight == 6);

	SCOPE_END;
}


int VTable_WhenDeriving_OverriddeesVTablePointer()
{
	//Arange
	SCOPE_START;

	//Act
	CREATE_OBJECT(SuperMat, mat,0,0,0);

	//Assert
	ASSERT_EQ(&(mat.vTable),&((SuperMatVirtualTable*)mat._base.vTable));

	SCOPE_END;
}

int VTable_WhenDriving_CanCallNoneOverridedBaseFunctionsViaOwnVTable()
{
	SCOPE_START;

	//Arange
	int width = 0;

	//SuperMat_init();
	CREATE_OBJECT(SuperMat, mat, 4, 4, 3);

	//Act
	mat.vTable->_base.GetWidth->func((Mat* )&mat, &width);
 
	//Assert
	ASSERT_EQ(width, 4);

	SCOPE_END;
}

int VTable_WhenDriving_NotOverridingTheBaseMembers()
{
	SCOPE_START;

	//Arange
	int hight, width;
	int h = 4, w = 4, step = 3;

	//Act
	CREATE_OBJECT(SuperMat, mat, h, w, step);

	hight = mat._base.hight;
	width = mat._base.width;

	//Assert
	ASSERT_EQ(hight, h);
	ASSERT_EQ(width, w);
	
	SCOPE_END;
}

int Overridding_WhenCallingAFunction_AlwaysCallsTheOvveridden()
{
	SCOPE_START;

	//Arrange
	int expectedLoc, actualLoc;

	CREATE_DERIVED_OBJECT(SuperMat, Mat4Test, mat, 4, 4, 2);

	//Act
	expectedLoc = (mat._base.width * 1 + 2) * mat.step;
	CALL(FindLoc, mat, 1, 2, &actualLoc);

	//Assert
	ASSERT_EQ(expectedLoc, actualLoc);

	SCOPE_END;

}