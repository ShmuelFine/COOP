//
//#include "SuperMat4Test.h"
// 
//DEF_DERIVED_CTOR(SuperMat4Test, Mat4Test, int hight, int width, int step) SUPER, hight, width ME
//{
//	_this->step = step;
//}                                    
//END_DERIVED_CTOR
//
//DEF_DERIVED_DTOR(SuperMat4Test, Mat4Test)
//{
//
//}
//END_DERIVED_DTOR
//
//FUNCTION_IMPL(SuperMat4Test, SetStep, int step)
//{
//	_this->step = step;
//}
//END_FUNCTION_IMPL
//
//FUNCTION_IMPL(SuperMat4Test, findLoc, int row, int col ,int *loc)
//{
//	_BASE(Mat4Test,findLoc, row, col, loc);
//	*loc *= _this->step;
//}
//END_FUNCTION_IMPL
//
//INIT_DERIVED_CLASS(SuperMat4Test, Mat4Test)
//	BIND(SuperMat4Test, SetStep);
//	BIND_OVERIDE(SuperMat4Test, Mat4Test, findLoc);
//	//SuperMat4TestVTable.__findLoc.func = &(SuperMat4Test_findLoc);            
//	//SuperMat4TestVTable.__findLoc.next = NULL;                                                            
//	//Mat4TestVTable.__findLoc.next = &(SuperMat4TestVTable.__findLoc);											
//	//SuperMat4TestVTable.findLoc = Mat4TestVTable.findLoc;
//END_INIT_CLASS
//
//
