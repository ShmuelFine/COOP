//#include "SuperMat3_4Test.h"
//
//DEF_DERIVED_CTOR(SuperMat3, SuperMat4Test, int hight, int width, int step, bool T) SUPER, hight, width, step ME
//{
//	_this->T = T;
//}
//END_DERIVED_CTOR
//
//DEF_DERIVED_DTOR(SuperMat3,SuperMat4Test)
//{}
//END_DERIVED_DTOR
//
//FUNCTION_IMPL(SuperMat3, Transpose)
//{
//	_this->T = !_this->T;
//}
//END_FUNCTION_IMPL
//
//FUNCTION_IMPL(SuperMat3, findLoc, int row, int col, int* loc)
//{
//	_BASE(SuperMat4Test,findLoc, row, col, loc);
//	*loc += (int)_this->T;
//}
//END_FUNCTION_IMPL
//
//INIT_DERIVED_CLASS(SuperMat3, SuperMat4Test)
//BIND(SuperMat3, Transpose);
//BIND_OVERIDE(SuperMat3, SuperMat4Test, findLoc);
////SuperMat3VTable.__findLoc.func = &(SuperMat3_findLoc);            
////SuperMat3VTable.__findLoc.next = NULL;                                                            
////SuperMat3VTable._base.__findLoc.next = &(SuperMat3VTable.__findLoc);											
////SuperMat3VTable.findLoc = SuperMat3VTable._base.findLoc;
//END_INIT_CLASS