#include "ScopeTest.h"
#include "Globals.h"

DEF_CTOR(ScopeTest, int size)
{
	//NEW(_this->buff, char, size);
	{
	void* returned;
	CALL(AddNewBlock, *TheGlobalCache, (sizeof(char) * size), &returned); 
//	{
//		struct AddNewBlock_t_ * f = (struct AddNewBlock_t_*)callFunction((function*)(*TheGlobalCache).vTable->AddNewBlock);
//	if(f->func(&(*TheGlobalCache), (sizeof(char) * size), &returned)==-1)
//	longjmp(SCOPE_FALLBACK_ADDR[_CurrScope_Idx],1);
//}
	_this->buff = (char*)returned; 
	}
}
END_CTOR

DEF_DTOR(ScopeTest)
{
	DELETE(_this->buff);
}
END_DTOR

FUNCTION_IMPL(ScopeTest, ThrowingIfEQ, int size, int eq)
{
	if (size == eq)
		THROW;
	size += eq;
}
END_FUNCTION_IMPL

INIT_CLASS(ScopeTest)
BASE_BIND(ScopeTest, ThrowingIfEQ);
END_INIT_CLASS